PROGRAMS = test_vpu

.PHONY: all
all:
	gcc -o test_vpu test_vpu.c -Irockchip-vpu-driver
	./test_vpu

.PHONY: clean
clean:
	$(RM) $(PROGRAMS)
