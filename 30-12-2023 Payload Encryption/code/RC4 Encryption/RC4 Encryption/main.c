#include <stdio.h>
#include "rc4.h"
#include "sysfn032.h"


void rc4() {
	// Initialization
	Rc4Context ctx = { 0 };

	// Key used for encryption
	unsigned char* key = "maldev123";

	rc4Init(&ctx, key, sizeof(key));
	// Encryption //
	// plaintext - The payload to be encrypted
	// ciphertext - A buffer that is used to store the outputted encrypted data
	const unsigned char plaintext[] = "Vuongle.Vigo";
	unsigned char encodedText[100] = { 0 };
	unsigned char decodedText[100] = { 0 };
	rc4Cipher(&ctx, plaintext, encodedText, sizeof(plaintext));
	printf("Encoded text: %s\n", encodedText);


	rc4Init(&ctx, key, sizeof(key));
	// Decryption //
	// ciphertext - Encrypted payload to be decrypted
	// plaintext - A buffer that is used to store the outputted plaintext data
	rc4Cipher(&ctx, encodedText, decodedText, sizeof(encodedText));
	printf("Decoded text: %s\n", decodedText);
}

void sysfn032() {
	unsigned char* key = "maldev123";
	const unsigned char plaintext[] = "Vuongle.Vigo";
	//encryption
	if (Rc4EncryptionViaSystemFunc032(key, plaintext, sizeof(key), sizeof(plaintext))) {
		printf("Encoded text: %s\n", plaintext);
	}
	//decryption
	if (Rc4EncryptionViaSystemFunc032(key, plaintext, sizeof(key), sizeof(plaintext))) {
		printf("Decryption text: %s\n", plaintext);
	}
}



int main() {
	sysfn032();
}