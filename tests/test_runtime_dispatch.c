#include "../src/irc_core.h"
#include "../src/runtime_adapter.h"
#include "../src/runtime_dispatch.h"
#include <stdio.h>
#include <string.h>
int main(void){
 irc_event e; char reply[128]={0};
 if (wrec_runtime_init()!=0) return 1;
 if (irc_parse_line(":alice!u@h PRIVMSG #ploos :!hello\\r\\n",&e)!=1) return 2;
 if (!wrec_dispatch_runtime(&e,reply,sizeof(reply))) return 3;
 if (strcmp(reply,"Hello from WreC")!=0) return 4;
 if(wren_backend_eval("class Bot { static argcmd() { return IRCContext.command() + \":\" + IRCContext.args() } }")!=0)return 5;
 if(wren_backend_eval("Commands.on(\"!args\", \"argcmd\")")!=0)return 6;
 memset(reply,0,sizeof(reply)); if(irc_parse_line(":alice!u@h PRIVMSG #ploos :!args Per Ola\\r\\n",&e)!=1)return 7; if(!wren_backend_command("args",&e,reply,sizeof(reply)))return 8; if(strcmp(reply,"args:Per Ola")!=0)return 9;
 wrec_runtime_shutdown(); puts("runtime IRC command: PASS"); return 0;
}
