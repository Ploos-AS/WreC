#include "wren_backend.h"
#ifdef WREC_WITH_WREN
#include "wren.h"
#include <stdio.h>
#include <string.h>
static WrenVM *vm; static WrenHandle *hello_call; static WrenHandle *event_call;
static const char *bootstrap="class Bot { static hello(nick) { return "Hello from WreC" } static event(name,nick,target) { if (name == "join") return "Welcome from WreC"; return null } }";
int wren_backend_init(void){WrenConfiguration config;wrenInitConfiguration(&config);vm=wrenNewVM(&config);if(!vm)return -1;if(wrenInterpret(vm,"bot",bootstrap)!=WREN_RESULT_SUCCESS)return -1;hello_call=wrenMakeCallHandle(vm,"hello(_)");event_call=wrenMakeCallHandle(vm,"event(_,_,_)");return hello_call&&event_call?0:-1;}
void wren_backend_shutdown(void){if(vm&&hello_call)wrenReleaseHandle(vm,hello_call);if(vm&&event_call)wrenReleaseHandle(vm,event_call);if(vm)wrenFreeVM(vm);hello_call=NULL;event_call=NULL;vm=NULL;}
int wren_backend_command(const char *method,const irc_event *event,char *reply,size_t rs){const char *result;if(!vm||!hello_call||!method||!event||!reply||!rs||strcmp(method,"hello")!=0)return 0;wrenEnsureSlots(vm,2);wrenGetVariable(vm,"bot","Bot",0);wrenSetSlotString(vm,1,event->nick);if(wrenCall(vm,hello_call)!=WREN_RESULT_SUCCESS)return 0;if(wrenGetSlotType(vm,0)!=WREN_TYPE_STRING)return 0;result=wrenGetSlotString(vm,0);snprintf(reply,rs,"%s",result);return 1;}
int wren_backend_event(const char *name,const irc_event *event,char *reply,size_t rs){const char *result;if(!vm||!event_call||!name||!event||!reply||!rs)return 0;wrenEnsureSlots(vm,4);wrenGetVariable(vm,"bot","Bot",0);wrenSetSlotString(vm,1,name);wrenSetSlotString(vm,2,event->nick);wrenSetSlotString(vm,3,event->target);if(wrenCall(vm,event_call)!=WREN_RESULT_SUCCESS)return 0;if(wrenGetSlotType(vm,0)!=WREN_TYPE_STRING)return 0;result=wrenGetSlotString(vm,0);snprintf(reply,rs,"%s",result);return 1;}
#else
int wren_backend_init(void){return -1;} void wren_backend_shutdown(void){}
int wren_backend_command(const char *m,const irc_event *e,char *r,size_t n){(void)m;(void)e;(void)r;(void)n;return 0;}
int wren_backend_event(const char *m,const irc_event *e,char *r,size_t n){(void)m;(void)e;(void)r;(void)n;return 0;}
#endif
