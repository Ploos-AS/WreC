#ifndef WREC_WREN_BACKEND_H
#define WREC_WREN_BACKEND_H
#include "irc_core.h"
#include "irc_output_sink.h"
#include <stddef.h>
int wren_backend_init(void);
void wren_backend_shutdown(void);
void wren_backend_set_output_sink(const irc_output_sink *sink);
int wren_backend_command(const char *method,const irc_event *event,char *reply,size_t reply_size);
int wren_backend_event(const char *name,const irc_event *event,char *reply,size_t reply_size);
int wren_backend_eval(const char *source);
#endif

int wren_backend_grant_capability(const char *cap);
