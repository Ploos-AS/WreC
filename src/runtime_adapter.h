#ifndef WREC_RUNTIME_ADAPTER_H
#define WREC_RUNTIME_ADAPTER_H
#include "irc_core.h"
#include "events.h"
#include "irc_output_sink.h"
#include <stddef.h>
int wrec_runtime_init(void);
void wrec_runtime_shutdown(void);
int wrec_runtime_command(const char *name,const irc_event *event,char *reply,size_t reply_size);
int wrec_runtime_event(const char *name,const irc_event *event,char *reply,size_t reply_size);
int wrec_runtime_bind_events(event_registry *registry);
void wrec_runtime_set_output_sink(const irc_output_sink *sink);
int wrec_runtime_say(const char *target,const char *text);
int wrec_runtime_notice(const char *target,const char *text);
int wrec_runtime_join(const char *channel);
int wrec_runtime_part(const char *channel,const char *reason);
#endif
