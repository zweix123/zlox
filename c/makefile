CC = gcc
DEBUGGER = gdb
TARGET_FILE = target.lox
CFLAGS = -Wall -Werror -O -fno-omit-frame-pointer -ggdb -gdwarf-2 -Wno-unused-function

SRCS := $(wildcard *.c)
DEPS := $(patsubst %.c,%.d,$(SRCS))
OBJS := $(patsubst %.c,%.o,$(SRCS))
EXEC = main.exe

.PHONY: all clean test gdb

all: $(EXEC)

-include $(DEPS)


$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	$(CC) $(CFLAGS) -MM $< > $@

clean:
	rm "$(EXEC)"
	rm *.d
	rm *.o

test: $(EXEC)
	./$(EXEC) $(TARGET_FILE)

gdb: $(EXEC)
	$(DEBUGGER) --args $(EXEC) $(TARGET_FILE)