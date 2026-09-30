#ifndef WREC_RUNTIME_ADAPTER_H
#define WREC_RUNTIME_ADAPTER_H
#include "irc_core.h"
#include <stddef.h>
int wrec_runtime_init(void);
void wrec_runtime_shutdown(void);
int wrec_runtime_command(const char *method, const irc_event *event, char *reply, size_t reply_size);
int wrec_runtime_event(const char *name, const irc_event *event, char *reply, size_t reply_size);
#endif
