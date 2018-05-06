CFLAGS = -g3 -Wall -I./include
LDLIBS = -l:func.o -l:tests.o
export LDFLAGS = -L./src -L./tests

.PHONY: func tests

main: main.o func tests
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ main.o $(LDLIBS)

func:
	$(MAKE) -C src/

tests:
	$(MAKE) -C tests/

clean:
	rm *.o src/*.o
