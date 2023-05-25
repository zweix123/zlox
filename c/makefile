C = gcc
DEBUGGER = gdb

REF_FILE = memory.c value.c chunk.c debug.c scanner.c compiler.c vm.c object.c table.c
THIRD_REF_FILE = linenoise.c 

.PHONY: build
build: $(THIRD_REF_FILE)

$(THIRD_REF_FILE):
	if [ ! -f linenoise.c ]; then wget -O linenoise.c https://raw.githubusercontent.com/antirez/linenoise/master/linenoise.c ; fi
	if [ ! -f linenoise.h ]; then wget -O linenoise.h https://raw.githubusercontent.com/antirez/linenoise/master/linenoise.h ; fi

.PHONY: run
run: main.c $(REF_FILE) $(THIRD_REF_FILE)
	$(C) main.c $(REF_FILE) $(THIRD_REF_FILE) -o main.exe
	./main.exe
	$(MAKE) clean

.PHONY: test
test: main.c $(REF_FILE) $(THIRD_REF_FILE)
	$(C) main.c $(REF_FILE) $(THIRD_REF_FILE) -o main.exe
	./main.exe example.lox
	$(MAKE) clean

.PHONY: gdb
gdb: main.c $(REF_FILE) $(THIRD_REF_FILE)
	$(C) -g main.c $(REF_FILE) $(THIRD_REF_FILE) -o main.exe
	$(DEBUGGER) --args main.exe example.lox
	$(MAKE) clean

clean: 
	rm ./*.exe