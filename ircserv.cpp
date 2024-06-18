/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olahmami <olahmami@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 18:21:32 by olahmami          #+#    #+#             */
/*   Updated: 2024/06/18 16:42:02 by olahmami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ircserv.hpp"

int main(int ac, char **av)
{
    // Check if the correct number of arguments is provided
    if (ac != 3)
        return(std::cerr << "Usage: ./ircserv <port> <password>" << std::endl, 1);

    // Create a socket for the server
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        return (std::cerr << "Socket creation failed" << std::endl, 1);

    // Set the socket to non-blocking mode
    fcntl(serverSocket, F_SETFL, O_NONBLOCK);

    // Set up the server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    u_int16_t port = (u_int16_t)atoi(av[1]);
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0)
        return (std::cerr << "Bind failed" << std::endl, 1);

    // Start listening for incoming connections
    if (listen(serverSocket, 5) < 0)
        return (std::cerr << "Listen failed: " << strerror(errno) << std::endl, 1);

    // Create an epoll instance
    int epollSocket = epoll_create1(0);
    if (epollSocket < 0)
        return (std::cerr << "Epoll creation failed" << std::endl, 1);

    // Add the server socket to the epoll instance
    struct epoll_event evServer;
    evServer.events = EPOLLIN;
    evServer.data.fd = serverSocket;
    if (epoll_ctl(epollSocket, EPOLL_CTL_ADD, serverSocket, &evServer) < 0)
        return (std::cerr << "Epoll control server failed" << std::endl, 1);

    std::cout << "IRC server is waiting for incoming connections" << std::endl;

    // Set the maximum number of events to be returned by epoll_wait
    int maxEvents = 9;
    std::vector<struct epoll_event> events(maxEvents);

    while (true)
    {
        // Wait for events to occur
        int numEvents = epoll_wait(epollSocket, events.data(), maxEvents, -1);
        if (numEvents < 0)
        {
            std::cerr << "Epoll wait failed: " << strerror(errno) << std::endl;
            break;
        }

        // Resize the events vector if it is full
        if (numEvents == maxEvents)
            events.resize(maxEvents + 1);

        for (int i = 0; i < numEvents; i++)
        {
            // If the event is for the server socket, accept the incoming connection
            if (events[i].data.fd == serverSocket)
            {
                int clientSocket = accept(serverSocket, NULL, NULL);
                if (clientSocket < 0)
                {
                    std::cerr << "Error accepting connection: " << strerror(errno) << std::endl;
                    continue;
                }

                std::cout << "Client connected" << std::endl;
                
                // Add the client socket to the epoll instance
                struct epoll_event evClient;
                evClient.events = EPOLLIN;
                evClient.data.fd = clientSocket;
                if (epoll_ctl(epollSocket, EPOLL_CTL_ADD, clientSocket, &evClient) < 0)
                {
                    std::cerr << "Epoll control client failed: " << strerror(errno) << std::endl;
                    close(clientSocket);
                }
            }
            // If the event is for a client socket, receive and process the message
            else
            {
                char buffer[1024];
                bzero(buffer, sizeof(buffer));
                int bytesReceived = recv(events[i].data.fd, buffer, sizeof(buffer), 0);
                if (bytesReceived <= 0)
                {
                    std::cout << "Client disconnected." << std::endl;
                    close(events[i].data.fd);
                }
                else
                    std::cout << "Message from client: " << buffer << std::endl;
            }
            
        }
    }

    close(serverSocket);
    return 0;
}