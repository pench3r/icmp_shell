#ifndef _DES_H_
#define _DES_H_

#define ENCRYPTION_MODE 1
#define DECRYPTION_MODE 0

// Declare action parameters
#define ACTION_GENERATE_KEY "-g"
#define ACTION_ENCRYPT "-e"
#define ACTION_DECRYPT "-d"

// DES key is 8 bytes long
#define DES_KEY_SIZE 8


typedef struct {
	unsigned char k[8];
	unsigned char c[4];
	unsigned char d[4];
} key_set;

void generate_key(unsigned char* key);
void generate_sub_keys(unsigned char* main_key, key_set* key_sets);
void process_message(unsigned char* message_piece, unsigned char* processed_piece, key_set* key_sets, int mode);

void des_process(unsigned char *data, int data_len, int mode, unsigned char *output, int *output_len);

void encrypt_msg(unsigned char *plain, int plain_len, unsigned char *cipher, int *cipher_len);

void decrypt_msg(unsigned char *cipher, int cipher_len, unsigned char *plain, int *plain_len);

#endif
