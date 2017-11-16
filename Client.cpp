#include<iostream>
#include<WinSock2.h>
#include<string>
#include<map>
#include<vector>

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib") 
#define _WINSOCK_DEPRECATED_NO_WARNINGS


int main(){

	WSADATA wsaData;
	WORD DllVersion = MAKEWORD(2, 2);
	SOCKADDR_IN address;
	SOCKET serverSocket, clientSocket;
	int sendsize = 512;
	char sendbuffer[512];
	int recevsize = 1024;
	char recevbuffer[1024]; 
	

	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
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
	char text[512]; 
	for (int i = 0; i < line.length();i++){
		sendbuffer[i] =line[i];
	}
	
	for (int i =line.length() ;i < sendsize; i++){
		sendbuffer[i] = '#'; 
	}
	send(clientSocket, sendbuffer, sendsize, 0); 
	recv(clientSocket, recevbuffer, recevsize, 0); 
	std::cout << "Text from server: "; 
	for (int i = 0; i < recevsize; i++){
		if (recevbuffer[i] != '#'){
			std::cout << recevbuffer[i]; 
		}
	}

	std::cin.ignore(1);
	return 0;

}
