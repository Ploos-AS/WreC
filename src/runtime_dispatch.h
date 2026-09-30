#ifndef WREC_RUNTIME_DISPATCH_H
#define WREC_RUNTIME_DISPATCH_H
#include "irc_core.h"
#include <stddef.h>
int wrec_dispatch_runtime(const irc_event *event,char *reply,size_t reply_size);
int wrec_dispatch_event_runtime(const irc_event *event,char *reply,size_t reply_size);
#endif
