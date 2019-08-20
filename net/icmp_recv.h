#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>


#define ICMP_RECV_DATA_EOF 57005
#define ICMP_RECV_DATA 57006

void icmp_recv_socket_init();
void recv_icmp(unsigned char *recv_buff, int *recv_buff_size);

