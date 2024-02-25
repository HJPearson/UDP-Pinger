#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    // If used incorrectly, print proper usage
    if (argc != 3) {
        std::cout << "Usage: ./server <IPv4 address> <Port number>" << std::endl;
        return 1;
    }
    // Grab the desired IPv4 address from input
    const char *ip_string = argv[1];
    // Set up the struct to hold the server and client information
    struct sockaddr_in server_address, client_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(std::stoi(argv[2]));
    // Convert the IPv4 input from a string to proper format
    if (inet_pton(AF_INET, ip_string, &server_address.sin_addr) == -1) {
        perror("inet_pton failed");
        return 1;
    }
    // Create socket file descriptor for UDP
    int socket_fd;
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket() failed");
        return 1;
    }
    // Bind the socket
    if (bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)) == -1) {
        perror("bind() failed");
        return 1;
    }
    while (true) {
        // Receive a ping from a client
        char buffer[1024];
        socklen_t client_address_length = sizeof(client_address);
        ssize_t bytes_received;
        if ((bytes_received = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &client_address, &client_address_length)) == -1) {
            perror("recvfrom() failed");
        }
        // Echo the ping back to the client
        char ping[] = "Ping";
        ssize_t bytes_sent;
        if ((bytes_sent = sendto(socket_fd, ping, strlen(ping), 0, (struct sockaddr *) &client_address, sizeof(client_address))) == -1) {
            perror("sendto() failed");
        }
    }
    // Close the socket
    if (close(socket_fd) == -1) {
        perror("close() failed");
        return 1;
    }
    return 0;
}