# include "common.hpp"
# include "IpObfuscation.hpp"



int main() {
	char buffer[] = "NtReadVirtualMemory";
	const char* bufferip[] = { "78.116.82.101\0", "97.100.86.105\0", "114.116.117.97\0", "108.77.101.109\0", "111.114.121.127\0"};
	GenerateIpv4String(buffer, strlen(buffer));
	//DeobfuscateIpv4String(bufferip, sizeof(bufferip));
}