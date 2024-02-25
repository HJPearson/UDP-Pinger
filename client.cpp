#include <iostream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>


int main(int argc, char *argv[]) {
    // If used incorrectly, print proper usage
    if (argc != 3) {
        std::cout << "Usage: ./client <IPv4 address> <Port number>" << std::endl;
        return 1;
    }
    // Grab the desired IPv4 address from input
    const char *ip_string = argv[1];
    // Set up the struct to hold the server information
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(std::stoi(argv[2]));
    // Convert the IPv4 input from a string to proper format
    if (inet_pton(AF_INET, ip_string, &server_address.sin_addr) == -1) {
        perror("inet_pton failed");
        return 1;
    }
    // Create a socket file descriptor for UDP
    int socket_fd;
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket() failed");
        return 1;
    }
    // Create variables for timing the round trip time of the ping
    std::chrono::steady_clock::time_point start, end;
    // Send a ping
    char ping[] = "Ping";
    size_t bytes_sent;
    if ((bytes_sent = sendto(socket_fd, ping, strlen(ping), 0, (struct sockaddr *) &server_address, sizeof(server_address))) == -1) {
        perror("sendto() failed");
        return 1;
    }
    // Grab the time when the ping was sent
    start = std::chrono::steady_clock::now();
    // Receive Ping echo
    char buffer[1024];
    socklen_t server_address_length = sizeof(server_address);
    ssize_t bytes_received;
    if ((bytes_received = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *) &server_address, &server_address_length)) == -1) {
        perror("recvfrom() failed");
        return 1;
    }
    // Grab the time when the ping was received
    end = std::chrono::steady_clock::now();
    // Calculate round trip time
    std::chrono::duration<double> rtt = std::chrono::duration_cast<std::chrono::duration<double> >(end - start);
    // Null terminate the buffer and print it
    buffer[bytes_received] = '\0';
    std::cout << buffer << std::endl;
    std::cout << "Round trip time: " << rtt.count() << " seconds" << std::endl;

    return 0;
}