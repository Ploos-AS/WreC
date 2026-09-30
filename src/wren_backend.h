#ifndef WREC_WREN_BACKEND_H
#define WREC_WREN_BACKEND_H

#include "irc_core.h"
#include <stddef.h>

int wren_backend_init(void);
void wren_backend_shutdown(void);
int wren_backend_command(const char *method, const irc_event *event, char *reply, size_t reply_size);

#endif
