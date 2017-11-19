#include<iostream>
#include<WinSock2.h>
#include<string>
#include<map>
#include<vector>
#include<algorithm>

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib") 
#define _WINSOCK_DEPRECATED_NO_WARNINGS


int main(){

	WSADATA wsaData;
	WORD word = MAKEWORD(2, 2);
	SOCKADDR_IN address;
	SOCKET serverSocket, clientSocket;
	int sendsize = 512;
	char sendbuffer[512];
	int recevsize = 1024;
	char recevbuffer[1024]; 
	

	if (WSAStartup(word, &wsaData) != 0) {
		std::cout << "Error winsock" << std::endl;
		exit(1);
	}

	//Address to bind connection socket to

	inet_pton(AF_INET, "127.0.0.1", &(address.sin_addr));//address.sin_addr.s_addr = inet_addr("127.0.0.1"); // local host
	address.sin_port = htons(5858); //port("HostToNetworkShort")
	address.sin_family = AF_INET; //IPv4
	int addrlen = sizeof(address); //lenght

	clientSocket = socket(AF_INET, SOCK_STREAM, 0); 
	if (connect(clientSocket, (SOCKADDR*)&address, addrlen) == INVALID_SOCKET){
		std::cout << "Socket error" << std::endl; 
		return 0; 
	}
	std::cout << "Client connected" << std::endl; 
	std::cout << "Podaj tekst do przeslania" << std::endl;
	std::string line;
	std::getline(std::cin, line);
	line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end()); 
	for (int i = 0; i < line.size(); i++){
		sendbuffer[i] = line[i]; 
	}
	for (int i = line.size(); i < sendsize; i++){
		sendbuffer[i] = '\0';
	}
	send(clientSocket, sendbuffer, sizeof(sendbuffer), 0); 
	recv(clientSocket, recevbuffer, sizeof(recevbuffer), 0); 
	std::cout << "Text from server: "; 
	puts(recevbuffer); 
	
	
	closesocket(clientSocket);
	std::cin.ignore(1);
	return 0;

}
