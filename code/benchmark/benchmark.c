#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

//
// Created by Markus Ketzer.
//
#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

FILE *fptr;

double median(double* array, int count);
double quartile1(double* array, int count);
double quartile3(double* array, int count);
void printResults(char* action, double* timeMeasurements, int timeMeasurementsCount);
void measure_prime_and_probe(int cycleCount);
void measure_prime_and_abort(int cycleCount);
void measure(int cycleCount, char* title, void (*action)());


void prime_and_probe();
void prime_and_abort();

void usage(const char* prog){
    fprintf(stderr, "Usage: %s -c cycles -d directory\n", prog);
    exit(1);
}

int main(int argc, char **argv) {
    int cycles = 100;
    char* xlateDir = "../originalcode/xlate";
    int opt;

    while((opt = getopt(argc, argv, "c:d:")) != -1){
        switch(opt) {
            case 'c':
                cycles = atoi(optarg);
                break;
            case 'd':
                xlateDir = malloc(strlen(optarg));
                strcpy(xlateDir, optarg);
        }
    }

    fptr = fopen("../results.txt", "w");

    chdir(xlateDir);

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

void measure_prime_and_probe(int cycleCount){
    measure(cycleCount, "Prime and Probe L1", prime_and_probe);
}

void measure_prime_and_abort(int cycleCount){
    measure(cycleCount, "Prime and Abort L1", prime_and_abort);
}

void prime_and_probe(){
    int status = system("LD_LIBRARY_PATH=\"./openssl-1.1.1b:$LD_LIBRARY_PATH\" ./obj/aes-pp openssl-1.1.1b/libcrypto.so");
}
void prime_and_abort(){

    int status = system("LD_LIBRARY_PATH=\"./openssl-1.1.1b:$LD_LIBRARY_PATH\" ./obj/aes-pa openssl-1.1.1b/libcrypto.so");
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