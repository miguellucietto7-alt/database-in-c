# ===========================================
# Compiler
# ===========================================

CC := gcc

# ===========================================
# Directories
# ===========================================

SRC_DIR := src
INC_DIR := include
TEST_DIR := tests
BIN_DIR := bin

LIB_DIR := ../../mylib/c
READ_DIR := $(LIB_DIR)/read
UTILS_DIR := $(LIB_DIR)/utils

# ===========================================
# Flags
# ===========================================

CFLAGS := -Wall -Wextra -Wpedantic -ggdb -std=c17 -g \
		   -I$(INC_DIR) \
		   -I$(READ_DIR) \
		   -I$(UTILS_DIR)

# ===========================================
# Source files
# ===========================================

SRC := $(wildcard $(SRC_DIR)/*.c)

LIBSRC := \
	$(READ_DIR)/read.c \
	$(UTILS_DIR)/utils.c

# ===========================================
# Build any test
#
# Exemplo:
# make db
# make parser
# make select
# ===========================================

%:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC) $(LIBSRC) $(TEST_DIR)/$@.c -o $(BIN_DIR)/$@

# ===========================================
# Address Sanitizer
#
# Exemplo:
# make asan TEST=db
# ./bin/db
# ===========================================

asan:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) \
	-fsanitize=address \
	-fno-omit-frame-pointer \
	$(SRC) \
	$(LIBSRC) \
	$(TEST_DIR)/$(TEST).c \
	-o $(BIN_DIR)/$(TEST)

# ===========================================
# Valgrind
#
# Exemplo:
# make valgrind TEST=db
# ===========================================

valgrind:
	valgrind \
	--leak-check=full \
	--show-leak-kinds=all \
	./$(BIN_DIR)/$(TEST)

# ===========================================
# Clean
# ===========================================

clean:
	rm -rf $(BIN_DIR)

# ===========================================
# Rebuild
# ===========================================

rebuild: clean