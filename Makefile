PROGRAMS = test_vpu

.PHONY: all
all:
	gcc -o test_ioctls test_ioctls.c -Irockchip-vpu-driver
	./test_ioctls

.PHONY: clean
clean:
	$(RM) $(PROGRAMS)
