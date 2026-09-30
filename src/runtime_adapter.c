#include "runtime_adapter.h"
#include "wren_backend.h"
#include <stdio.h>
#include <string.h>
static int initialized,wren_active;
int wrec_runtime_init(void) {
#ifdef WREC_WITH_WREN
    wren_active=(wren_backend_init()==0);
#else
    wren_active=0;
#endif
    initialized=1; return 0;
}
void wrec_runtime_shutdown(void){if(wren_active)wren_backend_shutdown();wren_active=0;initialized=0;}
int wrec_runtime_command(const char *method,const irc_event *event,char *reply,size_t rs){if(!initialized||!method||!reply||!rs)return 0;if(wren_active&&wren_backend_command(method,event,reply,rs))return 1;if(strcmp(method,"hello")==0){snprintf(reply,rs,"Hello from WreC");return 1;}return 0;}
int wrec_runtime_event(const char *name,const irc_event *event,char *reply,size_t rs){if(!initialized||!name||!event||!reply||!rs)return 0;if(wren_active&&wren_backend_event(name,event,reply,rs))return 1;return 0;}
