#include "common.h"

int main() {

    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        die("socket");
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT); // Port number
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    if (connect(client_fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        close(client_fd);
        die("connect");
    }

    // Send a message to the server
    const char *message = "Hi, My name is client!";
    send_one_packet(client_fd, message);

    // Receive a response from the server
    char buf[4+MAX_PACKET_SIZE+1];
    recv_one_packet(client_fd, buf);
    
    // Print the response
    std::cout << "Received from server: " << std::string(buf+4) << std::endl;
    // Close the socket
    if (close(client_fd)) {
        die("close");
    }
}