# include "IpObfuscation.hpp"


void GenerateIpv4String( char* pInput, SIZE_T size) {
	char output[32] = { 0 };
	while (size % 4 != 0) {
		pInput[size] = '\x7F';
		size++;
	}
	for (int i = 0; i < size; i++) {
		if ((i + 1) % 4 == 0) {
			sprintf(output, "%d.%d.%d.%d\0", pInput[i-3], pInput[i - 2], pInput[i - 1], pInput[i]);
			printf(" \ %s \ \n", output);
		}
		
	}
	//
}

void DeobfuscateIpv4String(const char* pInput[], SIZE_T size) {
	char* buffer = (char*)calloc(size, sizeof(char));
	int octetCounter = 0;
	for (int i = 0; i < size/sizeof(pInput); i++) {
		char* ipStr = (char*)pInput[i];
		char octet[4] = { 0 };
		int k = 0;
		for (int j = 0; j < strlen(ipStr) + 1; j++) {				// strlen + 1 for null end of string
			if (ipStr[j] == '.' || ipStr[j] == '\0') {
				octet[k] = '\0';
				k = 0;
				int tmp = atoi(octet);
				if (tmp != 0x7F) {
					buffer[octetCounter] = (char)tmp;
				}
				octetCounter++;
			}
			else {
				octet[k] = ipStr[j];
				k++;
			}
		}
	}
	printf("%s", buffer);
}

