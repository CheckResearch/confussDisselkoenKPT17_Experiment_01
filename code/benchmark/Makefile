BUILD ?= obj
FILES=$(wildcard *.c)
TARGETS=benchmark
OBJS+=$(FILES:.c=.o)
CFLAGS += -D_GNU_SOURCE -g3 -Wall -Wextra -std=gnu11 -O2
CFLAGS += -I../originalcode/xlate/include
LDFLAGS += -flto -O2
LIBS += -lelf -lpthread -lrt -lcrypto

-include ../originalcode/xlate/source/Makefile

dep = $(obj:.o=.d)
-include $(dep)

.PHONY: all clean


obj = $(addprefix $(BUILD)/xlate/, $(obj-y))
obj += $(addprefix $(BUILD)/, $(OBJS))

all: $(BUILD)/benchmark

$(BUILD)/benchmark: $(obj)
	$(CC) $(obj) -o $@ $(LDFLAGS) $(LIBS)

$(BUILD)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD)/xlate/%.o: ../originalcode/xlate/%.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf ${BUILD}
