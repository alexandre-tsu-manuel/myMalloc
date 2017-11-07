CC?=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -Werror
LIB=libmalloc.so
TEST_BIN=test
TEST_FILES=main
TEST_OBJ=$(TEST_FILES:%=%.o)
SRC_FILES=malloc
SRC_OBJ=$(SRC_FILES:%=%.o)
LDLIBS=-shared

all: $(LIB)

$(LIB): CFLAGS+=-fno-builtin -fPIC
$(LIB): $(SRC_OBJ)
	$(CC) $(SRC_OBJ) -o $@ $(LDLIBS)

$(TEST_BIN): $(TEST_OBJ)
	$(CC) $(TEST_OBJ) -o $@ $(LDFLAGS)

check: $(LIB) $(TEST_BIN)
	LD_PRELOAD=./$(LIB) ./$(TEST_BIN)

clean:
	rm -f *.o $(TEST_BIN) $(LIB)
