#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define ICMP_DATA_LEN 56    // send icmp real length
#define ACCESS_KEY 2333     // for authorized
#define MAX_ICMP_LEN 100    // for one icmp data boundary

#define ICMP_SEND_DATA 57006
#define ICMP_SEND_DATA_EOF 57005

void icmp_send_socket_init(char *target_ip);
void send_icmp(char *data, int data_len);
