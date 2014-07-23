#include "main.h"
#include "send_recv.h"
#include "global_externs.h"

void* start_chat(void *conn_num) {
  printf("[info] Starting chat with connection %d\n",*((int*)conn_num));
  pthread_create(&server_s[*((int*)conn_num)],NULL,server_send,conn_num);
  pthread_create(&server_r[*((int*)conn_num)],NULL,server_recv,conn_num);
  pthread_join(server_s[*((int*)conn_num)],NULL);
  pthread_join(server_r[*((int*)conn_num)],NULL);
  printf("Exiting safely\n");
  shutdown(peer_sd[*((int*)conn_num)], SHUT_RDWR);
  return NULL;
}

void* accept_conn_thread(void* args) {
  int i=0;
  while(EXIT_STATUS) {
    peer_socket_mem[conn_counter[i]]= (socklen_t) sizeof(peer_socket_ipp[conn_counter[i]]);
    peer_sd[conn_counter[i]]=accept(server_sd, (struct sockaddr*)&peer_socket_ipp[conn_counter[i]],
				    peer_socket_mem+conn_counter[i]);
    if (peer_sd[conn_counter[i]]<0) {
      perror("[info] Problem in creating the peer socket");
      shutdown(peer_sd[conn_counter[i]], SHUT_RDWR);
      exit(4);
    } else {
      printf("[info] Peer socket created for connection from ");
      printf("%s on port %u\n",inet_ntoa(peer_socket_ipp[conn_counter[i]].sin_addr),
	     ntohs(peer_socket_ipp[conn_counter[i]].sin_port));
      pthread_create(&in_conn_threads[conn_counter[i]],NULL,start_chat,(void*)&conn_counter[i]);
      i++;
      printf("[info] Number of connections = %d\n",i);
    }
  }
  int j;
  for (j=0;j<i;j++)
    pthread_join(in_conn_threads[j],NULL);
  return NULL;
}
