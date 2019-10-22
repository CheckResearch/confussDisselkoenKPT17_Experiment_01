FILES=$(wildcard *.c)
TARGETS=$(FILES:.c=)
OBJS=${FILES:.c=.o}
CFLAGS=-g -std=gnu99
LDFLAGS=-g


all: ${TARGETS}

${TARGETS}: %: %.o 
	${CC} ${LDFLAGS} -o $@ $@.o


clean:
	rm -f ${TARGETS} ${OBJS} *.sig out

distclean: clean
	rm Makefile
