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
	WORD word = MAKEWORD(2, 2);
	SOCKADDR_IN address;
	SOCKET serverSocket, clientSocket;
	int backlog = 5; 
	int recvsize = 512; 
	char recevbuffer[512];
	int sendsize = 1024; 
	char sendbuffer[1024];
	std::map<char,int>counterLetter; 

	if (WSAStartup(word, &wsaData) != 0) {
		std::cout << "Error winsock" << std::endl; 
		exit(1);
	}

	//Address to bind connection socket to
	inet_pton(AF_INET, "127.0.0.1", &(address.sin_addr));//address.sin_addr.s_addr = inet_addr("127.0.0.1"); // local host
	address.sin_port = htons(5858); //port("HostToNetworkShort
	address.sin_family = AF_INET; //IPv4
	int addrlen = sizeof(address); //lenght
	
    //Server socket 
	serverSocket = socket(AF_INET, SOCK_STREAM, NULL); 
	bind(serverSocket, (SOCKADDR*)&address, addrlen); 
	listen(serverSocket, backlog); 

	//Client socket 
	int result = 0;

	clientSocket = accept(serverSocket, (SOCKADDR*)&address, &addrlen); 
	if (clientSocket == INVALID_SOCKET){
		std::cout << "Client failed" << std::endl; 
	}
	else{
		std::cout << "Client conected" << std::endl;
		
		result=recv(clientSocket, recevbuffer, sizeof(recevbuffer), 0);
		std::cout << "Text from client: ";	
		puts(recevbuffer);
		
				
		// zliczanie liter w teksie 
		for (int i = 0; i < sizeof(recevbuffer); i++){
			auto it = counterLetter.find(recevbuffer[i]);
			if (recevbuffer[i] != '\0'){
				if (it == counterLetter.end()){
					if (recevbuffer[i] != 32 && recevbuffer[i] != 10 && recevbuffer[i] != 13 && recevbuffer[i] != 1){
						counterLetter.emplace(recevbuffer[i], 1);
					}
				}
				else{
					if (recevbuffer[i] != 32 && recevbuffer[i] != 10 && recevbuffer[i] != 13 && recevbuffer[i] != 1){
						counterLetter[recevbuffer[i]]++;
					}
				}
			}
		}
		// uzuplnianie nowej tablicy
	
		std::vector<char>data; 
		for (auto it = counterLetter.begin(); it != counterLetter.end(); ++it){
			data.push_back(it->first);
			data.push_back(it->second + 48);
		}
		
		
		for (int i = 0; i <data.size(); i++){
			sendbuffer[i] = data[i]; 
		}
		for (int i = data.size(); i < sendsize; i++){
			sendbuffer[i] = '\0'; 
		}
		send(clientSocket, sendbuffer, sizeof(sendbuffer), 0);

	}


	
	if (result == 0) {
		closesocket(serverSocket); 
	}


	std::cin.ignore(1); 
	return 0; 
}

