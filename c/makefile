C = gcc

C_FILE = main.c memory.c value.c chunk.c debug.c vm.c

.PHONY: all
all: main.out
	./main.out

	$(MAKE) clean

main.out: $(C_FILE)
	$(C) $(C_FILE) -o main.out

clean: 
	rm ./*.out