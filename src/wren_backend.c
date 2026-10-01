#include "wren_backend.h"
#ifdef WREC_WITH_WREN
#include "wren.h"
#include <stdio.h>
#include <string.h>
static WrenVM *vm;
static irc_output_sink wrec_sink;
static WrenHandle *hello_call; static WrenHandle *event_call;
void wren_backend_set_output_sink(const irc_output_sink *sink){if(sink)wrec_sink=*sink;else memset(&wrec_sink,0,sizeof(wrec_sink));}
static void foreign_say(WrenVM *v){const char *t,*x;wrenEnsureSlots(v,2);t=wrenGetSlotString(v,0);x=wrenGetSlotString(v,1);wrenSetSlotBool(v,0,irc_send_privmsg(&wrec_sink,t,x));}
static void foreign_notice(WrenVM *v){const char *t,*x;wrenEnsureSlots(v,2);t=wrenGetSlotString(v,0);x=wrenGetSlotString(v,1);wrenSetSlotBool(v,0,irc_send_notice(&wrec_sink,t,x));}
static void foreign_join(WrenVM *v){const char *c;wrenEnsureSlots(v,1);c=wrenGetSlotString(v,0);wrenSetSlotBool(v,0,irc_send_join(&wrec_sink,c));}
static void foreign_part(WrenVM *v){const char *c,*r;wrenEnsureSlots(v,2);c=wrenGetSlotString(v,0);r=wrenGetSlotString(v,1);wrenSetSlotBool(v,0,irc_send_part(&wrec_sink,c,r));}
static WrenForeignMethodFn bind_foreign(const char *module,const char *class_name,bool is_static,const char *signature){(void)is_static;if(strcmp(module,"bot")!=0||strcmp(class_name,"IRC")!=0)return NULL;if(strcmp(signature,"say(_,_)")==0)return foreign_say;if(strcmp(signature,"notice(_,_)")==0)return foreign_notice;if(strcmp(signature,"join(_)")==0)return foreign_join;if(strcmp(signature,"part(_,_)")==0)return foreign_part;return NULL;}
static const char *bootstrap="foreign class IRC { static say(target, text) {} static notice(target, text) {} static join(channel) {} static part(channel, reason) {} } class Bot { static hello(nick) { return "Hello from WreC" } static event(name,nick,target) { if (name == "join") return "Welcome from WreC"; return null } }";
int wren_backend_init(void){WrenConfiguration config;wrenInitConfiguration(&config);config.bindForeignMethodFn=bind_foreign;vm=wrenNewVM(&config);if(!vm)return -1;if(wrenInterpret(vm,"bot",bootstrap)!=WREN_RESULT_SUCCESS)return -1;hello_call=wrenMakeCallHandle(vm,"hello(_)");event_call=wrenMakeCallHandle(vm,"event(_,_,_)");return hello_call&&event_call?0:-1;}
void wren_backend_shutdown(void){if(vm&&hello_call)wrenReleaseHandle(vm,hello_call);if(vm&&event_call)wrenReleaseHandle(vm,event_call);if(vm)wrenFreeVM(vm);hello_call=NULL;event_call=NULL;vm=NULL;}
int wren_backend_command(const char *method,const irc_event *event,char *reply,size_t rs){const char *result;if(!vm||!hello_call||!method||!event||!reply||!rs||strcmp(method,"hello")!=0)return 0;wrenEnsureSlots(vm,2);wrenGetVariable(vm,"bot","Bot",0);wrenSetSlotString(vm,1,event->nick);if(wrenCall(vm,hello_call)!=WREN_RESULT_SUCCESS)return 0;if(wrenGetSlotType(vm,0)!=WREN_TYPE_STRING)return 0;result=wrenGetSlotString(vm,0);snprintf(reply,rs,"%s",result);return 1;}
int wren_backend_event(const char *name,const irc_event *event,char *reply,size_t rs){const char *result;if(!vm||!event_call||!name||!event||!reply||!rs)return 0;wrenEnsureSlots(vm,4);wrenGetVariable(vm,"bot","Bot",0);wrenSetSlotString(vm,1,name);wrenSetSlotString(vm,2,event->nick);wrenSetSlotString(vm,3,event->target);if(wrenCall(vm,event_call)!=WREN_RESULT_SUCCESS)return 0;if(wrenGetSlotType(vm,0)!=WREN_TYPE_STRING)return 0;result=wrenGetSlotString(vm,0);snprintf(reply,rs,"%s",result);return 1;}
#else
void wren_backend_set_output_sink(const irc_output_sink *sink){(void)sink;}
int wren_backend_init(void){return -1;} void wren_backend_shutdown(void){}
int wren_backend_command(const char *m,const irc_event *e,char *r,size_t n){(void)m;(void)e;(void)r;(void)n;return 0;}
int wren_backend_event(const char *m,const irc_event *e,char *r,size_t n){(void)m;(void)e;(void)r;(void)n;return 0;}
#endif
