CC=gcc
CFLAGS=-I ./crypto/ -I ./net/

client: client.c ./crypto/des.c ./net/icmp_send.c ./net/icmp_recv.c
	$(CC) -o client client.c ./crypto/des.c ./net/icmp_send.c ./net/icmp_recv.c

server: server.c ./crypto/des.c ./net/icmp_send.c ./net/icmp_recv.c
	$(CC) -o server server.c ./crypto/des.c ./net/icmp_send.c ./net/icmp_recv.c
