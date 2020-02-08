#include "l1_pa.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include "tsx.h"

#define L1_ASSOCIATIVITY 8
#define L1_CACHELINE 64
#define L1_STRIDE (L1_CACHELINE * L1_SETS)


#define PTR(set, way, ptr) (void *)(((uintptr_t)l1->memory) + ((set) * L1_CACHELINE) + ((way) * L1_STRIDE) + ((ptr)*sizeof(void *)))
#define LNEXT(p) (*(void **)(p))

void l1_prime_and_abort(l1pp_t l1, uint16_t *results) {
    size_t ncommits = 0;
    size_t naborts = 0;
    unsigned ret;

    while (ncommits < 16 && naborts < 16) {
        segments = l1->nsets;
        seglen = L1_ASSOCIATIVITY;

        while (segments--) {
            if ((ret = xbegin()) == XBEGIN_INIT) {
                for (int i = seglen; i--;) {
                    asm volatile (""::"r" (p):);
                    p = LNEXT(p);
                }


                xend();
                ++ncommits;
            }else if (ret & XABORT_CAPACITY) {
                ++naborts;
            }
        }
    }

    return naborts >= 16;
}