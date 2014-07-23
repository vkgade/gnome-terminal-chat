#include "main.h"
#include "global_externs.h"

void cmd_exit(void ) {
  EXIT_STATUS=0;
}

void estb_conn(char* remote_address, unsigned short int port) {
  ;
}

void cmd_fun(int cmd_num) {
  if (cmd_num==0) {
    char *remote_addr= malloc(sizeof(char)*16);
    unsigned short int remote_port;
    printf("[cmd][enter ip] ");
    scanf("%s",&remote_addr);
    printf("\n[cmd][enter port] ");
    scanf("%u",&remote_port);
    printf("\n[info] Trying to connect to %s on port %u\n",remote_addr,remote_port);
    estb_conn(remote_addr,remote_port);
  }
  if (cmd_num==1) 
    cmd_exit();
}

void set_conn_counter(void ) {
  int i;
  for (i=0;i<LISTEN_BACKLOG;i++)
    conn_counter[i]=i;
}
