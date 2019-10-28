#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/aes.h>

#include <fcntl.h>
#include <sched.h>
#include <time.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <libelf.h>
#include <gelf.h>

#include <xlate/elf.h>
#include <xlate/eviction.h>
#include <xlate/list.h>
#include <xlate/macros.h>
#include <xlate/page_set.h>
#include <xlate/sort.h>

#include <xlate/x86-64/clflush.h>
#include <xlate/x86-64/time.h>

void set_up_pp(char* libcrypto_path);

void execute_pp();

void clean_up_pp();