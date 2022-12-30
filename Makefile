src = src/main.c
src+= src/stack.c
src+= src/exec.c

obj = $(src:.c=.o)

main: $(obj)
	gcc -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) main

.PHONY: run
run:
	make clean && make && ./main ./challenge.bin
