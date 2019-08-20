#include <stdio.h>
#include "icmp_recv.h"



int main(int argc, char *argv[]) {
	char recv_cipher_buff[1500] = {0};
    int recv_buff_size = 0;
	icmp_recv_socket_init();
	recv_icmp(recv_cipher_buff, &recv_buff_size);
	printf("recv done.\n");
	printf("total size: %d\n", recv_buff_size);
	fwrite(recv_cipher_buff, 1, recv_buff_size, stdout);
    return 0;
}
