#include <stdio.h>
#include "icmp_send.h"

static struct sockaddr_in target_sa;
static int icmp_send_sockfd;

// icmp checksum
static unsigned short cal_chksum(unsigned short *addr, int len)
{
    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;

    while(nleft > 1)
    {   
        sum += *w++;
        nleft -= 2;
    }   
    if( nleft == 1)
    {
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;

    return answer;
}

// sending icmp with fix length
static void send_icmp_with_data(int seq, char *data, int data_len){
	struct icmp *icmp;
    char icmp_send_buff[MAX_ICMP_LEN] = {0};
	int send_data_len;
	// init icmp header and data
	int icmp_data_len = data_len;
	icmp = (struct icmp *)icmp_send_buff;
	icmp->icmp_type = ICMP_ECHO;
	icmp->icmp_code = 0;
	icmp->icmp_id = ACCESS_KEY;
	icmp->icmp_seq = seq;
	// with last block length in icmp->icmp_code
	if (seq == ICMP_SEND_DATA_EOF)
		icmp->icmp_code = icmp_data_len;
	// memset(icmp->icmp_data, 0x41, icmp_data_len);
	memcpy(icmp->icmp_data, data, icmp_data_len);

	send_data_len = 8 + icmp_data_len;
	icmp->icmp_cksum = 0;
	icmp->icmp_cksum = cal_chksum((unsigned short *)icmp, send_data_len);

	// send icmp request
	sendto(icmp_send_sockfd, icmp_send_buff, send_data_len, 0, (struct sockaddr *)&target_sa, sizeof(target_sa));
}

// expose interface
void icmp_send_socket_init(char *target_ip) {
    // socket recv buff size is 60k
    int recv_buff_size = 60 * 1024;
    icmp_send_sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    setsockopt(icmp_send_sockfd, SOL_SOCKET, SO_RCVBUF, &recv_buff_size, sizeof(int));

    // init target sockaddr_in
    memset(&target_sa, 0, sizeof(target_sa));
    target_sa.sin_family = AF_INET;
    target_sa.sin_addr.s_addr = inet_addr(target_ip);
}

void send_icmp(char *data, int data_len) {
	char *send_data = data;
	int total_data_len = data_len;
	int reminder = total_data_len % ICMP_DATA_LEN;
	int data_block_num = total_data_len / ICMP_DATA_LEN;
	if (reminder > 0)
		data_block_num += 1;
	// for send last msg with EOF flag
	int last_block = data_block_num - 1;
	for(int send_index=0; send_index < data_block_num; ++send_index) {
		// printf("send %d: %s\n", send_index, send_data);
		if(send_index == last_block) {
			// send last block with EOF, break out loop
			send_icmp_with_data(ICMP_SEND_DATA_EOF, send_data, reminder);
			break;
		}	
		send_icmp_with_data(ICMP_SEND_DATA, send_data, ICMP_DATA_LEN);
		send_data += ICMP_DATA_LEN;
	}
}
