C = gcc
GDB = gdb

REF_FILE = memory.c value.c chunk.c debug.c scanner.c compiler.c vm.c
THIRD_REF_FILE = linenoise.c linenoise.h

.PHONY: all
all: 
	echo "nothing"

$(THIRD_REF_FILE):
	if [ ! -f linenoise.c ]; then wget -O linenoise.c https://raw.githubusercontent.com/antirez/linenoise/master/linenoise.c ; fi
	if [ ! -f linenoise.h ]; then wget -O linenoise.h https://raw.githubusercontent.com/antirez/linenoise/master/linenoise.h ; fi

.PHONY: run
run: main.c $(REF_FILE) $(THIRD_REF_FILE)
	$(C) main.c $(REF_FILE) $(THIRD_REF_FILE) -o main.exe
	./main.exe
	$(MAKE) clean

.PHONY: test
test: test.c $(REF_FILE) $(THIRD_REF_FILE)
	$(C) test.c $(REF_FILE) $(THIRD_REF_FILE) -o test.exe
	./test.exe
	$(MAKE) clean

GDB_FILE_NAME = test
.PHONY: gdb
gdb: $(GDB_FILE_NAME).c $(REF_FILE) $(THIRD_REF_FILE)
	$(C) -g $(GDB_FILE_NAME).c $(REF_FILE) $(THIRD_REF_FILE) -o $(GDB_FILE_NAME).exe
	$(GDB) $(GDB_FILE_NAME).exe
	$(MAKE) clean

clean: 
	rm ./*.exe