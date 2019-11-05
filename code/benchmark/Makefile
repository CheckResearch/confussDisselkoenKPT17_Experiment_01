FILES=$(wildcard *.c)
TARGETS=benchmark
OBJS=${FILES:.c=.o}
CFLAGS=-I../originalcode/xlate/include
LDFLAGS=-flto -O2
LIBS=-lelf -lpthread -lrt

-include ../originalcode/xlate/source/Makefile

.PHONY: all clean

all: benchmark

benchmark: ${OBJS}
	${CC} -o $@ ${OBJS}

%.o: %.c
	$(CC) -c $< $(LDFLAGS) $(LIBS)


clean:
	rm -f ${TARGETS} ${OBJS} *.sig out
