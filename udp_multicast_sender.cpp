#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

void sendMulticastMessage(const char* multicastGroup, int port, const char* message) {
    // Create a UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        return;
    }

    // Set up the multicast group address
    struct sockaddr_in multicastAddr;
    memset(&multicastAddr, 0, sizeof(multicastAddr));
    multicastAddr.sin_family = AF_INET;
    multicastAddr.sin_addr.s_addr = inet_addr(multicastGroup);
    multicastAddr.sin_port = htons(port);

    // Enable loopback for testing on the same machine
    int loopback = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback, sizeof(loopback)) < 0) {
        perror("Error setting multicast loopback");
        close(sockfd);
        return;
    }

    // Send the multicast message
    ssize_t bytesSent = sendto(sockfd, message, strlen(message), 0, (struct sockaddr*)&multicastAddr, sizeof(multicastAddr));
    if (bytesSent == -1) {
        perror("Error sending multicast message");
    } else {
        std::cout << "Sent multicast message: " << message << std::endl;
    }

    // Close the socket
    close(sockfd);
}

int main() {
    const char* multicastGroup = "239.0.0.1"; // Replace with your desired multicast group
    int port = 12345; // Replace with your desired port number
    const char* message = "Hello, multicast world!";

    sendMulticastMessage(multicastGroup, port, message);

    return 0;
}

