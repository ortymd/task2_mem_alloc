CFLAGS = -g3 -Wall -I./include
LDLIBS = -l:func.o
export LDFLAGS = -L./src

.PHONY: func 

main: main.o func
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ main.o $(LDLIBS)


func:
	$(MAKE) -C src/

clean:
	rm *.o src/*.o
