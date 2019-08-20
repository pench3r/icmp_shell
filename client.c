#include <stdio.h>
#include "net/icmp_send.h"
#include "net/icmp_recv.h"
#include "crypto/des.h"

int main(int argc, char *argv[]) {
    char *target_ip;

    if (argc < 3) {
        printf("not found target ip or command");
        exit(-1);
    }
    target_ip = argv[1];
	icmp_send_socket_init(target_ip);

	unsigned char plain_buff[100] = {0};
	memcpy(plain_buff, argv[2], strlen(argv[2]));

	unsigned char send_buff[1500] = {0};
	int send_buff_size = 0;

	encrypt_msg(plain_buff, sizeof(plain_buff), send_buff, &send_buff_size);
	// fwrite(send_buff, 1, send_buff_size, stdout);
	
	send_icmp(send_buff, send_buff_size);

	// printf("begin to recv data\n");

	// recv
	unsigned char recv_cipher_buff[1500];
    int recv_buff_size = 0;
	unsigned char output_buff[1500];
	int output_size = 0;
    icmp_recv_socket_init();
	while(recv_buff_size == 0) {
		memset(recv_cipher_buff, 0, sizeof(recv_cipher_buff));
		recv_buff_size = 0;
    	recv_icmp(recv_cipher_buff, &recv_buff_size);
		if(recv_buff_size == 0)
			continue;
    	// printf("total size: %d\n", recv_buff_size);
    	// fwrite(recv_cipher_buff, 1, recv_buff_size, stdout);
		memset(output_buff, 0, sizeof(output_buff));
		output_size = 0;
		decrypt_msg(recv_cipher_buff, recv_buff_size, output_buff, &output_size);
		printf("%d bytes: %s", output_size, output_buff);
		// fwrite(output_buff, 1, output_size, stdout);
	}

    return 0;
}
