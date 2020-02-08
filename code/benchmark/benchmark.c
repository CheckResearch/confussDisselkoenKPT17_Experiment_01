#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include "l1.h"
#include "l1_pa.h"

//
// Created by Markus Ketzer.
//
#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)
#define MAX_SAMPLES 100000

FILE *fptr;
uint16_t *res_pp;
uint16_t *res_pa;
l1pp_t l1_pp;
l1pp_t l1_pa;

double median(double* array, int count);
double quartile1(double* array, int count);
double quartile3(double* array, int count);
void printResults(char* action, double* timeMeasurements, int timeMeasurementsCount);
void measure_prime_and_probe(int cycleCount);
void measure_prime_and_abort(int cycleCount);
void measure(int cycleCount, char* title, void (*action)());


void usage(const char* prog){
    fprintf(stderr, "Usage: %s -c cycles -d directory\n", prog);
    exit(1);
}

char* libcrypto_path;

int main(int argc, char **argv) {
    int cycles = 10;
    libcrypto_path = "openssl-1.0.1e/libcrypto.so";
    int opt;

    while((opt = getopt(argc, argv, "c:l:")) != -1){
        switch(opt) {
            case 'c':
                cycles = atoi(optarg);
                break;
            case 'l':
                libcrypto_path = malloc(strlen(optarg));
                strcpy(libcrypto_path, optarg);
        }
    }

    fptr = fopen("../results.txt", "w");

    measure_prime_and_probe(cycles);
    measure_prime_and_abort(cycles);

    fclose(fptr);

    return 0;
}


void measure(int cycleCount, char* title, void (*action)()){
    double* times = (double *) malloc(cycleCount * sizeof(double));
    for(int i = 0; i < cycleCount; i++){
        clock_t start,end;
        start = clock();
        action();
        end = clock();
        times[i] = (double)(end - start);
    }
    printResults(title, times, cycleCount);
    free(times);
}

void set_up_pp() {
  l1_pp = l1_prepare();

  int nsets = l1_getmonitoredset(l1_pp, NULL, 0);

  int *map = calloc(nsets, sizeof(int));
  l1_getmonitoredset(l1_pp, map, nsets);

  int rmap[L1_SETS];
  for (int i = 0; i < L1_SETS; i++)
    rmap[i] = -1;
  for (int i = 0; i < nsets; i++)
    rmap[map[i]] = i;
  

  res_pp = calloc(samples * nsets, sizeof(uint16_t));
  for (int i = 0; i < samples * nsets; i+= 4096/sizeof(uint16_t))
    res_pp[i] = 1;

    
    free(map);
}

void execute_pp() {
  l1_repeatedprobe(l1_pp, MAX_SAMPLES, res_pp, 0);
}

void clean_up_pp() {
    free(res_pp);
    l1_release(l1_pp);
}

void measure_prime_and_probe(int cycleCount){
    set_up_pp();
    measure(cycleCount, "Prime and Probe L1", execute_pp);
    clean_up_pp();
}

void set_up_pa() {
    l1_pa = l1_prepare();

  int nsets = l1_getmonitoredset(l1_pa, NULL, 0);

  int *map = calloc(nsets, sizeof(int));
  l1_getmonitoredset(l1_pa, map, nsets);

  int rmap[L1_SETS];
  for (int i = 0; i < L1_SETS; i++)
    rmap[i] = -1;
  for (int i = 0; i < nsets; i++)
    rmap[map[i]] = i;

  res_pa = calloc(samples * nsets, sizeof(uint16_t));
  for (int i = 0; i < samples * nsets; i+= 4096/sizeof(uint16_t))
    res_pa[i] = 1;

}

void execute_pa() {
    l1_prime_and_abort(l1_p1, res_pa);
}

void clean_up_pa() {
    free(res_pa);
    l1_release(l1_pa);
}

void measure_prime_and_abort(int cycleCount){
    set_up_pa();
    measure(cycleCount, "Prime and Abort L1", execute_pa);
    clean_up_pa();
}

double median(double* array, int count) {
    int mid = count / 2;
    return count % 2 == 0 ? (array[mid - 1] + array[mid]) / 2 : array[mid];
}

double quartile1(double* array, int count){
    int mid = count / 4;
    return count % 4 == 0 ? (array[mid - 1] + array[mid]) / 2 : array[mid];
}
double quartile3(double* array, int count){
    int mid = (count*3) / 4;
    return count % 4 == 0 ? (array[mid - 1] + array[mid]) / 2 : array[mid];
}

void printResults(char* action, double* timeMeasurements, int timeMeasurementsCount) {
    fprintf(fptr, "%s results: \n", action);
    double sum = timeMeasurements[timeMeasurementsCount - 1];
    for (int i = 0; i < timeMeasurementsCount - 1; i++){
        fprintf(fptr, "%f ms\n", timeMeasurements[i]);
        sum += timeMeasurements[i];
        for(int j = 0; j < timeMeasurementsCount - i - 1; j++) {
            if(timeMeasurements[j] > timeMeasurements[j+1]) {
                long tmp = timeMeasurements[j];
                timeMeasurements[j] = timeMeasurements[j+1];
                timeMeasurements[j+1] = tmp;
            }
        }
    }
    fprintf(fptr, "%f ms\n", timeMeasurements[timeMeasurementsCount]);

    double med = median(timeMeasurements, timeMeasurementsCount);
    double q1 = quartile1(timeMeasurements, timeMeasurementsCount);
    double q3 = quartile3(timeMeasurements, timeMeasurementsCount);
    double avg = sum / timeMeasurementsCount;

    fprintf(fptr, "Min: %f ms, Max: %f ms, Avg: %f ms\n1Q: %f ms, Median: %f ms, 3Q: %f ms\n", timeMeasurements[0], timeMeasurements[timeMeasurementsCount-1], avg, q1, med, q3);
}
