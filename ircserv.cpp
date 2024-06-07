/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:21:32 by olahmami          #+#    #+#             */
/*   Updated: 2024/06/07 19:30:24 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
        return(std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        return (std::cerr << "Socket creation failed" << std::endl, 1);
    
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    u_int16_t port = (u_int16_t)atoi(av[1]);
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        return (std::cerr << "Bind failed" << std::endl, 1);

    if (listen(serverSocket, 5) < 0)
        return (std::cerr << "Listen failed: " << strerror(errno) << std::endl, 1);

    std::cout << "IRC server is waiting for incoming connections" << std::endl;
    
    while (true)
    {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket < 0)
        {
            std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
            continue;
        }
        
        std::cout << "Client connected" << std::endl;
        
        while (true) 
        {
            char buffer[1024] = { 0 };

            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived <= 0)
            {
                std::cout << "Client disconnected." << std::endl;
                break;
            }
            std::cout << "Message from client: " << buffer << std::endl;
        }
        close(clientSocket);
    }
    close(serverSocket);
    return 0;
}