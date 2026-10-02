CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2
BIN := build/wrec
TEST_IRC := build/test_irc
TEST_DISPATCHER := build/test_dispatcher
TEST_EVENTS := build/test_events
TEST_RUNTIME := build/test_runtime
TEST_BOT_STATE := build/test_bot_state
TEST_BOT_CAPS := build/test_bot_caps
TEST_BOT_RUNTIME := build/test_bot_runtime
TEST_BOT_STATE_BACKEND := build/test_bot_state_backend
TEST_BOT_STATE_CODEC := build/test_bot_state_file_codec
TEST_BOT_STATE_CODEC_INVALID := build/test_bot_state_file_codec_invalid
TEST_BOT_RUNTIME_PERSISTENCE := build/test_bot_runtime_persistence
TEST_RUNTIME_DISPATCH := build/test_runtime_dispatch
TEST_TIMERS := build/test_timers
TEST_TIMER_HANDLERS := build/test_timer_handlers
TEST_WREN_TIMER := build/test_wren_timer
SRC := src/bot_state.c src/bot_caps.c src/bot_runtime.c src/bot_state_file.c src/bot_state_file_codec.c src/bot_state_file_save.c src/irc_output.c src/irc_output_sink.c src/main.c src/irc_core.c src/dispatcher.c src/events.c src/runtime_adapter.c src/runtime_dispatch.c src/timers.c src/timer_handlers.c src/wren_backend.c
WREN_DIR ?= vendor/wren
WREN_VM_SRCS := $(WREN_DIR)/src/vm/wren_compiler.c $(WREN_DIR)/src/vm/wren_core.c $(WREN_DIR)/src/vm/wren_debug.c $(WREN_DIR)/src/vm/wren_primitive.c $(WREN_DIR)/src/vm/wren_utils.c $(WREN_DIR)/src/vm/wren_value.c $(WREN_DIR)/src/vm/wren_vm.c
.PHONY: all test test-wren clean
all: $(BIN)
$(BIN): $(SRC)
	@mkdir -p build
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
$(TEST_BOT_STATE): tests/test_bot_state.c src/bot_state.c src/bot_state.h src/irc_core.c src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_state.c src/bot_state.c src/irc_core.c -o $(TEST_BOT_STATE)
$(TEST_BOT_CAPS): tests/test_bot_caps.c src/bot_caps.c src/bot_caps.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_caps.c src/bot_caps.c -o $(TEST_BOT_CAPS)
$(TEST_BOT_RUNTIME): tests/test_bot_runtime.c src/bot_runtime.c src/bot_runtime.h src/bot_state.c src/bot_caps.c
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_runtime.c src/bot_runtime.c src/bot_state.c src/bot_caps.c src/bot_state_file.c src/bot_state_file_codec.c src/bot_state_file_save.c -o $(TEST_BOT_RUNTIME)
$(TEST_IRC): tests/test_irc.c src/irc_core.c src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_irc.c src/irc_core.c src/irc_output.c -o $(TEST_IRC)
$(TEST_DISPATCHER): tests/test_dispatcher.c src/irc_core.c src/dispatcher.c src/irc_core.h src/dispatcher.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_dispatcher.c src/irc_core.c src/dispatcher.c -o $(TEST_DISPATCHER)
$(TEST_EVENTS): tests/test_events.c src/irc_core.c src/events.c src/irc_core.h src/events.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_events.c src/irc_core.c src/events.c -o $(TEST_EVENTS)
$(TEST_RUNTIME): tests/test_runtime.c src/runtime_adapter.c src/wren_backend.c src/runtime_adapter.h src/wren_backend.h src/irc_core.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_runtime.c src/runtime_adapter.c src/wren_backend.c src/events.c src/timers.c src/timer_handlers.c src/irc_output.c src/irc_output_sink.c -o $(TEST_RUNTIME)
$(TEST_RUNTIME_DISPATCH) $(TEST_TIMERS) $(TEST_TIMER_HANDLERS): tests/test_runtime_dispatch.c src/irc_core.c src/runtime_adapter.c src/runtime_dispatch.c src/wren_backend.c
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_runtime_dispatch.c src/irc_core.c src/runtime_adapter.c src/runtime_dispatch.c src/wren_backend.c src/events.c src/timers.c src/timer_handlers.c src/irc_output.c src/irc_output_sink.c -o $(TEST_RUNTIME_DISPATCH)
$(TEST_TIMERS): tests/test_timers.c src/timers.c src/timers.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_timers.c src/timers.c -o $(TEST_TIMERS)
$(TEST_TIMER_HANDLERS): tests/test_timer_handlers.c src/timer_handlers.c src/timer_handlers.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_timer_handlers.c src/timer_handlers.c -o $(TEST_TIMER_HANDLERS)
test: $(BIN) $(TEST_BOT_RUNTIME_PERSISTENCE) $(TEST_BOT_STATE_BACKEND) $(TEST_BOT_STATE_CODEC) $(TEST_BOT_STATE_CODEC_INVALID) $(TEST_IRC) $(TEST_DISPATCHER) $(TEST_EVENTS) $(TEST_RUNTIME) $(TEST_RUNTIME_DISPATCH) $(TEST_BOT_STATE) $(TEST_BOT_CAPS) $(TEST_BOT_RUNTIME)
	@./$(BIN) | grep -q "WreC M0"
	@./$(TEST_IRC)
	@./$(TEST_DISPATCHER)
	@./$(TEST_EVENTS)
	@./$(TEST_RUNTIME)
	@./$(TEST_RUNTIME_DISPATCH)
	@./$(TEST_BOT_STATE)
	@./$(TEST_BOT_CAPS)
	@./$(TEST_BOT_RUNTIME)
	@./$(TEST_BOT_STATE_BACKEND)
	@./$(TEST_BOT_STATE_CODEC)
	@./$(TEST_BOT_STATE_CODEC_INVALID)
	@./$(TEST_BOT_RUNTIME_PERSISTENCE)
	@./$(TEST_TIMERS)
	@./$(TEST_TIMER_HANDLERS)
	@echo "WreC M1 tests: PASS"
$(TEST_WREN_TIMER): tests/test_wren_timer.c src/irc_core.c src/runtime_adapter.c src/runtime_dispatch.c src/wren_backend.c src/timers.c src/timer_handlers.c $(WREN_VM_SRCS)
	@mkdir -p build
	$(CC) $(CFLAGS) -DWREC_WITH_WREN -I$(WREN_DIR)/src/include -I$(WREN_DIR)/src/vm tests/test_wren_timer.c src/irc_core.c src/runtime_adapter.c src/runtime_dispatch.c src/wren_backend.c src/timers.c src/timer_handlers.c $(WREN_VM_SRCS) -lm -o $(TEST_WREN_TIMER)

test-wren:
	@test -f $(WREN_DIR)/src/include/wren.h || (echo "Wren source missing at $(WREN_DIR)"; exit 1)
	@mkdir -p build
	$(CC) $(CFLAGS) -DWREC_WITH_WREN -I$(WREN_DIR)/src/include -I$(WREN_DIR)/src/vm tests/test_runtime_dispatch.c src/irc_core.c src/runtime_adapter.c src/runtime_dispatch.c src/wren_backend.c $(WREN_VM_SRCS) -lm -o build/test_runtime_dispatch_wren
	@./build/test_runtime_dispatch_wren
	@./build/test_wren_timer
	@echo "WreC Wren IRC VM test: PASS"
clean:
	rm -rf build

$(TEST_BOT_STATE_BACKEND): tests/test_bot_state_backend.c src/bot_state.c src/bot_state.h src/bot_state_backend.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_state_backend.c src/bot_state.c -o $(TEST_BOT_STATE_BACKEND)

$(TEST_BOT_STATE_CODEC): tests/test_bot_state_file_codec.c src/bot_state_file_codec.c src/bot_state.c src/bot_state.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_state_file_codec.c src/bot_state_file_codec.c src/bot_state.c -o $(TEST_BOT_STATE_CODEC)
$(TEST_BOT_STATE_CODEC_INVALID): tests/test_bot_state_file_codec_invalid.c src/bot_state_file_codec.c src/bot_state.c src/bot_state.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_state_file_codec_invalid.c src/bot_state_file_codec.c src/bot_state.c -o $(TEST_BOT_STATE_CODEC_INVALID)

$(TEST_BOT_RUNTIME_PERSISTENCE): tests/test_bot_runtime_persistence.c src/bot_runtime.c src/bot_runtime.h src/bot_state.c src/bot_state.h src/bot_caps.c src/bot_caps.h src/bot_state_backend.h src/bot_state_file.c src/bot_state_file.h src/bot_state_file_codec.c src/bot_state_file_codec.h src/bot_state_file_save.c src/bot_state_file_save.h
	@mkdir -p build
	$(CC) $(CFLAGS) tests/test_bot_runtime_persistence.c src/bot_runtime.c src/bot_state.c src/bot_caps.c src/bot_state_file.c src/bot_state_file_codec.c src/bot_state_file_save.c -o $(TEST_BOT_RUNTIME_PERSISTENCE)
