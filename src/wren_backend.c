#include "wren_backend.h"

#ifdef WREC_WITH_WREN
#include "wren.h"
#include <stdio.h>
#include <string.h>

static WrenVM *vm;
static WrenHandle *hello_call;

static const char *bootstrap =
    "class Bot {\n"
    "  static hello(nick) { return \"Hello from WreC\" }\n"
    "}\n";

int wren_backend_init(void) {
    WrenConfiguration config;
    wrenInitConfiguration(&config);
    vm = wrenNewVM(&config);
    if (!vm) return -1;
    if (wrenInterpret(vm, "bot", bootstrap) != WREN_RESULT_SUCCESS) return -1;
    hello_call = wrenMakeCallHandle(vm, "hello(_)");
    return hello_call ? 0 : -1;
}

void wren_backend_shutdown(void) {
    if (vm && hello_call) wrenReleaseHandle(vm, hello_call);
    if (vm) wrenFreeVM(vm);
    hello_call = NULL;
    vm = NULL;
}

int wren_backend_command(const char *method, const irc_event *event, char *reply, size_t reply_size) {
    const char *result;
    if (!vm || !hello_call || !method || !event || !reply || reply_size == 0) return 0;
    if (strcmp(method, "hello") != 0) return 0;

    wrenEnsureSlots(vm, 2);
    wrenGetVariable(vm, "bot", "Bot", 0);
    wrenSetSlotString(vm, 1, event->nick);
    if (wrenCall(vm, hello_call) != WREN_RESULT_SUCCESS) return 0;
    if (wrenGetSlotType(vm, 0) != WREN_TYPE_STRING) return 0;
    result = wrenGetSlotString(vm, 0);
    snprintf(reply, reply_size, "%s", result);
    return 1;
}
#else
int wren_backend_init(void) { return -1; }
void wren_backend_shutdown(void) {}
int wren_backend_command(const char *method, const irc_event *event, char *reply, size_t reply_size) {
    (void)method; (void)event; (void)reply; (void)reply_size;
    return 0;
}
#endif
