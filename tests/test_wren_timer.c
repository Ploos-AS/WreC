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
 if(wren_backend_eval("Timer.every(20, \"tick\")")!=0)return 2;
 if(wrec_runtime_timer_poll(19)!=0)return 3;
 if(wrec_runtime_timer_poll(20)!=1)return 4;
 if(strstr(line,"PRIVMSG #test :tick")==NULL)return 5;
 if(!wrec_runtime_timer_cancel(1))return 6;
 line[0]='\0';
 if(wrec_runtime_timer_poll(40)!=0)return 7;
 wrec_runtime_shutdown();
 puts("WreC Wren timer integration: PASS"); return 0;
}
