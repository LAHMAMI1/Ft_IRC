// C++ program to illustrate the client application in the 
// socket programming 
#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

int main(int ac, char **av) 
{ 
    if (ac != 3)
    {
        std::cerr << "Usage: ./socketc <message> <port>" << std::endl;
        return 1;
    }
	// creating socket 
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 

	// specifying address 
	sockaddr_in serverAddress; 
	serverAddress.sin_family = AF_INET; 
    u_int16_t port = (u_int16_t)atoi(av[2]);
	serverAddress.sin_port = htons(port); 
	serverAddress.sin_addr.s_addr = INADDR_ANY; 

	// sending connection request 
	connect(clientSocket, (struct sockaddr*)&serverAddress, 
			sizeof(serverAddress)); 

	// sending data 
	const char* message = av[1]; 
	send(clientSocket, message, strlen(message), 0); 

	// closing socket 
	close(clientSocket); 

	return 0; 
}
