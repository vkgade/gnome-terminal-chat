#include "main.h"
#include "commands.h"
#include "incoming_conn.h"
#include "send_recv.h"

char* cmd_list[]= {"/connect","/exit"};
int cmd_size=2;
int EXIT_STATUS=1;
int conn_counter[LISTEN_BACKLOG];
pthread_t server_s[LISTEN_BACKLOG], server_r[LISTEN_BACKLOG];
char quit_str[]= "quit\n";
pthread_t in_conn_threads[LISTEN_BACKLOG];
int server_sd, peer_sd[LISTEN_BACKLOG];
struct sockaddr_in server_ipp;
socklen_t server_ipp_mem;
struct sockaddr_in peer_socket_ipp[LISTEN_BACKLOG];
socklen_t peer_socket_mem[LISTEN_BACKLOG];


int main(int args,char* argv[]) {
  if (args!=2) {
    printf("[Exit] Usage: tserver <listen_port#>\n");
    exit(0);
  }

  unsigned short int PORT_NO = atoi(argv[1]);
  server_sd= socket(AF_INET, SOCK_STREAM, 0);
  if (server_sd <0) {
    perror("[Exit] Could not create a listen socket");
    exit(1);
  } 
  printf("[info] User socket Id %d\n",server_sd);
 
  server_ipp.sin_family= AF_INET;
  server_ipp.sin_addr.s_addr= INADDR_ANY;
  server_ipp.sin_port= htons(PORT_NO);
  server_ipp_mem=(socklen_t) sizeof(struct sockaddr_in);
  int server_sock_bind= bind(server_sd, 
			     (struct sockaddr*)&server_ipp, 
			     server_ipp_mem);
  if(server_sock_bind<0) {
    perror("[Exit] Could not bind the listen socket to an address");
    shutdown(server_sd, SHUT_RDWR);
    exit(2);
  }

  int server_listen= listen(server_sd, LISTEN_BACKLOG);
  if(server_listen<0) {
    perror("[Exit] Some server port listening problem");
    shutdown(server_sd, SHUT_RDWR);
    exit(3);
  }
  
  printf("[info] Online. Now you are listening on port number %u\n",PORT_NO);
  set_conn_counter();
  pthread_t accept_conn_thrd;
  pthread_create(&accept_conn_thrd, NULL, accept_conn_thread, NULL);

  int i;
  while(EXIT_STATUS) {
    char* cmd= malloc(sizeof(char)*20);
    scanf("%s",cmd);
    for(i=0;i<cmd_size;i++) 
      if(strcmp(cmd,cmd_list[i])==0) 
	cmd_fun(i);
  }
  
  shutdown(server_sd, SHUT_RDWR);
  return 0;
}
  
  
