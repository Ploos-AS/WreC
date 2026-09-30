CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BIN := build/wrec
TEST_BIN := build/test_irc
SRC := src/main.c src/irc_core.c

.PHONY: all test clean

all: $(BIN)

$(BIN): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

$(TEST_BIN): tests/test_irc.c src/irc_core.c src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_irc.c src/irc_core.c -o $(TEST_BIN)

test: $(BIN) $(TEST_BIN)
	@./$(BIN) | grep -q "WreC M0"
	@./$(TEST_BIN)
	@echo "WreC M0 tests: PASS"

clean:
	rm -rf build
