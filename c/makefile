C = gcc

C_FILE = main.c memory.c value.c chunk.c debug.c scanner.c compiler.c vm.c
TEST_C_FILE = test.c linenoise.c memory.c value.c chunk.c debug.c scanner.c compiler.c vm.c

.PHONY: all
all: main.out
	./main.out

	$(MAKE) clean

main.out: $(C_FILE)
	$(C) $(C_FILE) -o main.out

.PHONY: build
build: 
	if [ ! -f linenoise.c ]; then wget -O linenoise.c https://raw.githubusercontent.com/antirez/linenoise/master/linenoise.c ; fi
	if [ ! -f linenoise.h ]; then wget -O linenoise.h https://raw.githubusercontent.com/antirez/linenoise/master/linenoise.h ; fi


.PHONY: test
test: test.out
	./test.out
	
	$(MAKE) clean
test.out: $(TEST_C_FILE)
	$(C) $(TEST_C_FILE) -o test.out

clean: 
	rm ./*.out