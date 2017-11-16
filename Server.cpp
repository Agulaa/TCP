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
	int backlog = 5; 
	int recvsize = 512; 
	char recevbuffer[512];
	int sendsize = 1024; 
	char sendbuffer[1024];
	std::map<char,int>counterLetter; 
	if (WSAStartup(DllVersion, &wsaData) != 0) {
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
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
	clientSocket = accept(serverSocket, (SOCKADDR*)&address, &addrlen); 
	if (clientSocket == INVALID_SOCKET){
		std::cout << "Client failed" << std::endl; 
	}
	else{
		std::cout << "Client conected" << std::endl;
		recv(clientSocket, recevbuffer, recvsize, 0);
		std::cout << "Text from client: ";
		for (int i = 0; i < recvsize; i++){
			if (recevbuffer[i] != '#'){
				std::cout << recevbuffer[i]; 
			}
		}
		// zliczanie liter w teksie 
		for (int i = 0; i < recvsize; i++){
			auto it = counterLetter.find(recevbuffer[i]);
			
				if (it == counterLetter.end()){
					if (recevbuffer[i] != ' '){
						counterLetter.emplace(recevbuffer[i], 1);
					}
				}
				else{
					if (recevbuffer[i] != ' '){
						counterLetter[recevbuffer[i]]++;
					}
				}
			
		}
		// uzuplnianie nowej tablicy
		int i = 0;
		std::vector<char>helpVector;
		for (auto it = counterLetter.begin(); it != counterLetter.end(); ++it){
			helpVector.push_back(it->first);
			helpVector.push_back(it->second + 48);
		}
		int j = 0;
		for (auto x : helpVector){
			if (x != 0){
				sendbuffer[j] = x;
				j++;
			}
		}
		for (int i = helpVector.size(); i < sendsize; i++){
			sendbuffer[i] = '#'; 
		}
		send(clientSocket, sendbuffer, sendsize, 0);

	}


	if (shutdown(clientSocket, SD_BOTH) == 0){
		std::cout << std::endl; 
		std::cout << "Server shut down" << std::endl; 
}


	std::cin.ignore(1); 
	return 0; 
}

