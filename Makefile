UNAME_S := $(shell uname -s)
CFLAGS = -O2 -Wall -Wextra

ifeq ($(UNAME_S), Darwin)
    # macOS
    CC = clang
    CFLAGS += -march=native
else ifeq ($(UNAME_S), Linux)
    CC = gcc
    CFLAGS += -march=native
else
    CC = cc
endif

.PHONY: bin
bin:
	mkdir -p bin

.PHONY: transpose
transpose: bin
	$(CC) $(CFLAGS) -o ./bin/transpose ./matrix/transpose.c