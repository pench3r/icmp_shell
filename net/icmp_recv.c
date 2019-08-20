#include <stdio.h>
#include "icmp_recv.h"


// socket recv buff size is 60k
static int recv_buff_size = 60 * 1024;
static int icmp_recv_sockfd;

void icmp_recv_socket_init() {
    icmp_recv_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    setsockopt(icmp_recv_sockfd, SOL_SOCKET, SO_RCVBUF, &recv_buff_size, sizeof(int));
}

void recv_icmp(unsigned char *recv_buff, int *recv_buff_size) {
	int recv_bytes, ip_len;
    struct ip *ip;
    struct icmp *icmp;
    struct iovec recv_iov;
    struct msghdr recv_msg;
    struct sockaddr_in recv_sa;
    char recv_control_buff[1500];
	char tmp_network_recv_buff[1500] = {0};
    // init iovec and msghdr
    recv_iov.iov_base = tmp_network_recv_buff;
    recv_iov.iov_len = sizeof(tmp_network_recv_buff);

    recv_msg.msg_name = &recv_sa;
    recv_msg.msg_iov = &recv_iov;
    recv_msg.msg_iovlen = 1;
    recv_msg.msg_control = recv_control_buff;
    recv_msg.msg_namelen = sizeof(recv_sa);
    recv_msg.msg_controllen = sizeof(recv_control_buff);
	char *recv_buff_ptr = recv_buff;
	int recv_size = 0;

	do {
    	// recv icmp
    	recv_bytes = recvmsg(icmp_recv_sockfd, &recv_msg, 0);

    	// parse packet
    	ip = (struct ip *)tmp_network_recv_buff;
    	ip_len = ip->ip_hl << 2;
    	icmp = (struct icmp *)(tmp_network_recv_buff + ip_len);
		// check first icmp request
		if (icmp->icmp_id == 2333 && icmp->icmp_type == ICMP_ECHO) {
			if ((icmp->icmp_seq == ICMP_RECV_DATA_EOF) && (icmp->icmp_code > 0)) {
				memcpy(recv_buff_ptr, icmp->icmp_data, icmp->icmp_code);
				recv_size += icmp->icmp_code;
			} else if (icmp->icmp_seq == ICMP_RECV_DATA) {
				memcpy(recv_buff_ptr, icmp->icmp_data, 56);
				recv_buff_ptr += 56;
				recv_size += 56;
			}	
		}
		// there have many icmp request, so clean buff at this point
		// clean recv icmp data buff
		memset(icmp->icmp_data, 0, 56);
	// check recv icmp seq num
	} while(icmp->icmp_seq != ICMP_RECV_DATA_EOF && icmp->icmp_id == 2333 && icmp->icmp_type == ICMP_ECHO);
	// recv data size
	*recv_buff_size = recv_size;
}


// int main(int argc, char *argv[]) {
//     unsigned char icmp_recv_buff[1500];
//     int recv_buff_size;
// 	icmp_recv_socket_init();
// 
// 	recv_icmp(icmp_recv_buff, &recv_buff_size);
// 	printf("recv done.\n");
// 	printf("total size: %d\n", recv_buff_size);
// 	fwrite(icmp_recv_buff, 1, recv_buff_size, stdout);
//     return 0;
// }
