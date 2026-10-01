#include "../src/runtime_adapter.h"
#include <stdio.h>
#include <string.h>
static char line[1024]; static int sink(const char*s,void*u){(void)u;snprintf(line,sizeof(line),"%s",s);return 1;}
int main(void){irc_output_sink out={sink,NULL};if(wrec_runtime_init()!=0)return 1;wrec_runtime_set_output_sink(&out);
if(!wrec_runtime_say("#ploos","hello")||strcmp(line,"PRIVMSG #ploos :hello\r\n"))return 2;
if(!wrec_runtime_notice("alice","hi")||strcmp(line,"NOTICE alice :hi\r\n"))return 3;
if(!wrec_runtime_join("#ploos")||strcmp(line,"JOIN #ploos\r\n"))return 4;
if(!wrec_runtime_part("#ploos","bye")||strcmp(line,"PART #ploos :bye\r\n"))return 5;
puts("WreC runtime IRC output: PASS");wrec_runtime_shutdown();return 0;}