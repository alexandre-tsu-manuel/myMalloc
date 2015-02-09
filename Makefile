CC=clang
CFLAGSTEST= -Wall -Werror -Wextra -std=c99
CFLAGS= -Wall -Wextra -std=c99 -fno-builtin -O0 -g -fPIC
OUT=output

SRCFILES=$(wildcard *.c)

all: main

main:
	$(CC) $(SRCFILES) -o $(OUT) $(CFLAGSTEST)

libmalloc.so: malloc.o
	$(CC) -shared -o libmalloc.so malloc.o

clean:
	@rm -f *~ *.o *#
	@rm -f $(OUT)
	@rm -f libmalloc.so
	@echo "project cleaned"

run:
	@./$(OUT)
