VPATH = c
all:
	$(MAKE) -C c
clean:
	$(MAKE) -C c clean
run:
	$(MAKE) -C c run
test:
	$(MAKE) -C c test
gdb:
	$(MAKE) -C c gdb