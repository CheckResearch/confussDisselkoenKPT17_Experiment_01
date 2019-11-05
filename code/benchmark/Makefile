FILES=$(wildcard *.c)
TARGETS=$(FILES:.c=)
OBJS=${FILES:.c=.o}
CFLAGS=-g -std=gnu99
LDFLAGS=-g


all: benchmark

benchmark: ${OBJS}
	${CC} -o $@ ${OBJS}

%.o: %.c
	$(CC) -c $<


clean:
	rm -f ${TARGETS} ${OBJS} *.sig out

distclean: clean
	rm Makefile
