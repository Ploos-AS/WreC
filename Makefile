CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BIN := build/wrec
SRC := src/main.c

.PHONY: all test clean

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

test: $(BIN)
	@./$(BIN) | grep -q "WreC M0"
	@echo "WreC smoke test: PASS"

clean:
	rm -rf build
