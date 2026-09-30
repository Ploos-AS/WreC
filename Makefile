CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BIN := build/wrec
TEST_IRC := build/test_irc
TEST_DISPATCHER := build/test_dispatcher
TEST_RUNTIME := build/test_runtime
SRC := src/main.c src/irc_core.c src/dispatcher.c src/runtime_adapter.c src/wren_backend.c
WREN_DIR ?= vendor/wren
WREN_VM_SRCS := $(WREN_DIR)/src/vm/wren_compiler.c $(WREN_DIR)/src/vm/wren_core.c $(WREN_DIR)/src/vm/wren_debug.c $(WREN_DIR)/src/vm/wren_primitive.c $(WREN_DIR)/src/vm/wren_utils.c $(WREN_DIR)/src/vm/wren_value.c $(WREN_DIR)/src/vm/wren_vm.c

.PHONY: all test test-wren clean
all: $(BIN)
$(BIN): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
$(TEST_IRC): tests/test_irc.c src/irc_core.c src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_irc.c src/irc_core.c -o $(TEST_IRC)
$(TEST_DISPATCHER): tests/test_dispatcher.c src/irc_core.c src/dispatcher.c src/irc_core.h src/dispatcher.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_dispatcher.c src/irc_core.c src/dispatcher.c -o $(TEST_DISPATCHER)
$(TEST_RUNTIME): tests/test_runtime.c src/runtime_adapter.c src/wren_backend.c src/runtime_adapter.h src/wren_backend.h src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_runtime.c src/runtime_adapter.c src/wren_backend.c -o $(TEST_RUNTIME)
test: $(BIN) $(TEST_IRC) $(TEST_DISPATCHER) $(TEST_RUNTIME)
	@./$(BIN) | grep -q "WreC M0"
	@./$(TEST_IRC)
	@./$(TEST_DISPATCHER)
	@./$(TEST_RUNTIME)
	@echo "WreC M0 tests: PASS"
test-wren:
	@test -f $(WREN_DIR)/src/include/wren.h || (echo "Wren source missing at $(WREN_DIR)"; exit 1)
	@mkdir -p build
	$(CC) $(CFLAGS) -DWREC_WITH_WREN -I$(WREN_DIR)/src/include -I$(WREN_DIR)/src/vm tests/test_runtime.c src/runtime_adapter.c src/wren_backend.c $(WREN_VM_SRCS) -lm -o build/test_runtime_wren
	@./build/test_runtime_wren
	@echo "WreC Wren VM test: PASS"
clean:
	rm -rf build
