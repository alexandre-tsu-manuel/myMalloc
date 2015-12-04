CC ?= clang
CFLAGS = -Wall -Werror -Wextra -std=c99
LDFLAGS = $(CFLAGS)
LDFLAGS_LIB = $(CFLAGS) -fno-builtin -shared -O0 -g -fPIC
SRC_LIB = malloc.c
OBJ_LIB = $(SRC_LIB:%=%.o)
SRC = $(LIB_SRC) main.c
OBJ = $(SRC:%=%.o)
LIB = libmalloc.so
BIN = test

all: $(BIN)
$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(OUT)

library: $(LIB)
$(LIB): $(OBJ_LIB)
	$(CC) $(LDFLAGS_LIB) $(OBJ_LIB) -o $(LIB)

clean:
	@rm -f $(OBJ) $(BIN) $(LIB)
	@echo "project cleaned"

run:
	@./$(BIN)
