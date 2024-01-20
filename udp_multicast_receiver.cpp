#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

void receiveMulticastMessage(const char* multicastGroup, int port) {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up the multicast group address
    struct sockaddr_in multicastAddr;
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr(multicastGroup);
    multicastAddr.sin_port = htons(port);

    // Bind the socket to any available local address and the specified port
    struct sockaddr_in localAddr;
    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = INADDR_ANY;
    localAddr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr*)&localAddr, sizeof(localAddr)) == -1) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Join the multicast group
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr(multicastGroup);
    multicastRequest.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&multicastRequest, sizeof(multicastRequest)) == -1) {
        perror("Error joining multicast group");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Receive and print data
    //char buffer[1024];
    //struct sockaddr_in senderAddr;

    char buffer[1024];
    struct sockaddr_in senderAddr;
    socklen_t senderAddrLen = sizeof(senderAddr);

    //ssize_t bytesRead = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    ssize_t bytesRead = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&senderAddr, &senderAddrLen);
    if (bytesRead == -1) {
        perror("Error receiving data");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Null-terminate the received data
    buffer[bytesRead] = '\0';

    printf("Received multicast message: %s\n", buffer);

    // Get and print the sender's IP address
    char senderIP[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &senderAddr.sin_addr, senderIP, INET_ADDRSTRLEN) == NULL) {
        perror("Error converting sender's IP address");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Received multicast message from %s: %s\n", senderIP, buffer);



    // Leave the multicast group
    if (setsockopt(sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*)&multicastRequest, sizeof(multicastRequest)) == -1) {
        perror("Error leaving multicast group");
    }

    // Close the socket
    close(sockfd);
}

int main() {
    const char* multicastGroup = "239.0.0.1"; // Replace with your desired multicast group
    int port = 12345; // Replace with your desired port number

    receiveMulticastMessage(multicastGroup, port);

    return 0;
}

