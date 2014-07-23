#include "main.h"
#include "global_externs.h"

void* server_send(void* s) {
  int conn_name=*((int*)s); int j=0;
  printf("[info] Send activated for connection %d\n",conn_name);
  int *cancel_oldstate=NULL, *cancel_oldtype=NULL;
  char **serv_msg=NULL;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,cancel_oldtype);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,cancel_oldstate);
  int i;
  serv_msg= (char**) malloc(sizeof(char*)*1000);
  for(i=0; i<1000; i++)
    *(serv_msg+i)= (char*) malloc(sizeof(char)*100);
  while(strcmp(*(serv_msg+(j++)),quit_str)!=0) {
    read(0,serv_msg[j],sizeof(char)*100);
    ssize_t server_msg= send(peer_sd[conn_name], *(serv_msg+j),
			     sizeof(char)*100, 0);
    if(server_msg<0) {
      perror("Errrr... in sending, closing connection!");
      shutdown(peer_sd[conn_name], SHUT_RDWR);
      exit(5);
    }
  }
  for(i=0; i<1000; i++)
    free(*(serv_msg+i));
  free(serv_msg);
  pthread_cancel(server_r[conn_name]);
  return NULL;
}

void* server_recv(void* r) {
  int conn_name=*((int*)r); int k=0;
  printf("[info] Recv activated for connection %d\n",conn_name);
  int *cancel_oldstate=NULL, *cancel_oldtype=NULL;
  char **client_msg=NULL;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,cancel_oldtype);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,cancel_oldstate);
  int i;
  client_msg= (char**) malloc(1000*sizeof(char*));
  for (i=0; i<1000; i++) 
      *(client_msg+i)= (char*) malloc(sizeof(char)*100);
  while(strcmp(*(client_msg+(k++)),quit_str)!=0) {
    ssize_t peer_msg= recv(peer_sd[conn_name],*(client_msg+k),sizeof(char)*100,0);
    write(1,client_msg[k],sizeof(char)*100);
    if(peer_msg<0) {
      perror("Errrr.... could not recv the msg properly !");
      printf("Closing the connection !\n");
      shutdown(peer_sd[conn_name], SHUT_RDWR);
      exit(6);
    }
  }
  printf("The client closed the connection\n");
  for(i=0; i<100; i++)
    free(*(client_msg+i));
  free(client_msg);
  pthread_cancel(server_s[conn_name]);
  return NULL;
}
