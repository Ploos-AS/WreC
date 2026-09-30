#include "runtime_adapter.h"
#include <stdio.h>
#include <string.h>

static int initialized;

int wrec_runtime_init(void) {
    initialized = 1;
    return 0;
}

void wrec_runtime_shutdown(void) {
    initialized = 0;
}

int wrec_runtime_command(const char *method, const irc_event *event, char *reply, size_t reply_size) {
    (void)event;
    if (!initialized || !method || !reply || reply_size == 0) return 0;

    /* M0 adapter contract. The embedded Wren VM replaces this fallback
       without changing dispatcher or IRC core. */
    if (strcmp(method, "hello") == 0) {
        snprintf(reply, reply_size, "Hello from WreC");
        return 1;
    }
    return 0;
}
