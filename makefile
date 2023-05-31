VPATH = c
all:
	$(MAKE) -C c
clean:
	$(MAKE) -C c clean
test:
	$(MAKE) -C c test
gdb:
	$(MAKE) -C c gdb