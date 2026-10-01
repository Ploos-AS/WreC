#include "runtime_adapter.h"
#include "wren_backend.h"
#include <stdio.h>
#include <string.h>
static int initialized,wren_active;
static bot_timer_registry wrec_timers;
void wrec_runtime_timer_init(void){bot_timer_registry_init(&wrec_timers);}
bot_timer_id wrec_runtime_timer_after(uint64_t d,bot_timer_fn fn,void *u){return bot_timer_add(&wrec_timers,d,0,0,fn,u);}
bot_timer_id wrec_runtime_timer_every(uint64_t i,bot_timer_fn fn,void *u){return bot_timer_add(&wrec_timers,i,i,1,fn,u);}
int wrec_runtime_timer_cancel(bot_timer_id id){return bot_timer_cancel(&wrec_timers,id);}
size_t wrec_runtime_timer_poll(uint64_t now){return bot_timer_poll(&wrec_timers,now);}
int wrec_runtime_init(void){wrec_runtime_timer_init();
#ifdef WREC_WITH_WREN
wren_active=(wren_backend_init()==0);
#else
wren_active=0;
#endif
initialized=1;return 0;}
void wrec_runtime_shutdown(void){if(wren_active)wren_backend_shutdown();wren_active=0;initialized=0;}
int wrec_runtime_command(const char *method,const irc_event *event,char *reply,size_t rs){if(!initialized||!method||!reply||!rs)return 0;if(wren_active&&wren_backend_command(method,event,reply,rs))return 1;if(strcmp(method,"hello")==0){snprintf(reply,rs,"Hello from WreC");return 1;}return 0;}
int wrec_runtime_event(const char *name,const irc_event *event,char *reply,size_t rs){if(!initialized||!name||!event||!reply||!rs)return 0;if(wren_active&&wren_backend_event(name,event,reply,rs))return 1;return 0;}
static int wrec_event_bridge(const irc_event *event,void *user){char reply[512];const char *name=(const char *)user;if(!event||!name)return 0;return wrec_runtime_event(name,event,reply,sizeof(reply));}
int wrec_runtime_bind_events(event_registry *registry){static const char *names[]={"join","part","nick","quit","notice"};static const irc_event_type types[]={IRC_EVENT_JOIN,IRC_EVENT_PART,IRC_EVENT_NICK,IRC_EVENT_QUIT,IRC_EVENT_NOTICE};size_t i;if(!registry)return 0;for(i=0;i<5;++i)if(!event_registry_register(registry,types[i],wrec_event_bridge,(void *)names[i]))return 0;return 1;}
static irc_output_sink wrec_sink;
void wrec_runtime_set_output_sink(const irc_output_sink *sink){if(sink)wrec_sink=*sink;else memset(&wrec_sink,0,sizeof(wrec_sink));wren_backend_set_output_sink(sink);}
int wrec_runtime_say(const char *target,const char *text){return irc_send_privmsg(&wrec_sink,target,text);}
int wrec_runtime_notice(const char *target,const char *text){return irc_send_notice(&wrec_sink,target,text);}
int wrec_runtime_join(const char *channel){return irc_send_join(&wrec_sink,channel);}
int wrec_runtime_part(const char *channel,const char *reason){return irc_send_part(&wrec_sink,channel,reason);}
