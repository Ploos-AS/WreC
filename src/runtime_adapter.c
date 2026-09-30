#include "runtime_adapter.h"
#include "wren_backend.h"
#include <stdio.h>
#include <string.h>

static int initialized;
static int wren_active;

int wrec_runtime_init(void) {
#ifdef WREC_WITH_WREN
    wren_active = (wren_backend_init() == 0);
#else
    wren_active = 0;
#endif
    initialized = 1;
    return 0;
}

void wrec_runtime_shutdown(void) {
    if (wren_active) wren_backend_shutdown();
    wren_active = 0;
    initialized = 0;
}

int wrec_runtime_command(const char *method, const irc_event *event, char *reply, size_t reply_size) {
    if (!initialized || !method || !reply || reply_size == 0) return 0;

    if (wren_active && wren_backend_command(method, event, reply, reply_size)) return 1;

    /* Dependency-free M0 fallback keeps the core buildable without Wren. */
    if (strcmp(method, "hello") == 0) {
        snprintf(reply, reply_size, "Hello from WreC");
        return 1;
    }
    return 0;
}
