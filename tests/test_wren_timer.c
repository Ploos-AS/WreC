#include "../src/runtime_adapter.h"
#include "../src/wren_backend.h"
#include <stdio.h>
#include <string.h>
static char line[256];
static int capture(const char *s,void *u){char *dst=(char *)u;snprintf(dst,256,"%s",s);return 1;}
int main(void){
 irc_output_sink sink={capture,line};
 if(wrec_runtime_init()!=0)return 1;
 wrec_runtime_set_output_sink(&sink);
 if(wren_backend_eval("Timer.after(10, \"tick\")")!=0)return 2;
 if(wrec_runtime_timer_poll(9)!=0)return 3;
 if(wrec_runtime_timer_poll(10)!=1)return 4;
 if(strstr(line,"PRIVMSG #test :tick")==NULL)return 5;
 line[0]='\0';
 if(wren_backend_eval("Timer.every(20, \"tick\")")!=0)return 6;
 if(wren_backend_eval("Events.on(\"join\", \"registeredJoin\")")!=0)return 7;
 irc_event ev={0}; char event_reply[128]={0}; ev.type=IRC_EVENT_JOIN; snprintf(ev.nick,sizeof(ev.nick),"alice"); snprintf(ev.target,sizeof(ev.target),"#test"); if(!wren_backend_event("join",&ev,event_reply,sizeof(event_reply)))return 14; if(strcmp(event_reply,"Registered JOIN")!=0)return 13;
 if(wrec_runtime_timer_poll(19)!=0)return 8;
 if(wrec_runtime_timer_poll(20)!=1)return 9;
 if(wrec_runtime_timer_poll(40)!=1)return 10;
 if(!wrec_runtime_timer_cancel(2))return 11;
 line[0]='\0';
 if(wrec_runtime_timer_poll(60)!=0)return 12;
 wrec_runtime_shutdown();
 puts("WreC Wren timer lifecycle: PASS"); return 0;
}