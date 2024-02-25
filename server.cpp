#include "common.h"

void do_something(int client_fd){

    char buf[4+MAX_PACKET_SIZE+1];
    recv_one_packet(client_fd, buf);

    //std::cout << "buf length is:" << strlen(buf) << std::endl;

    // Print the response
    std::cout << "Received from client: " << std::string(buf+4) << std::endl;

    const char *response = "Hello from server!";
    send_one_packet(client_fd, response);
}

/**
 * Returns fd on which server is setup.
*/
int setup_server() {
    int server_fd;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        // perror appends errno to your msg
        // errno: a thread-local error value written to by POSIX syscalls 
        die("socket");
    }
    int reuse = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
        die("setsockopt");
    }
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    // htons: host to network short (sin_port is only 16 bit)
    // host (x86) byte order is little-endian (LSB at lowest mem addr)
    // network (TCP) byte order is big-endian (MSB at lowest mem addr)
    // XXX Should htons be mandatory? Maybe not if both server and client
    // stay consistent. (Since your don't know endian-ness of the client
    // better to always use htons for portability)
    serverAddr.sin_port = htons(PORT);
    // Bind to any available interface
    // XXX Did not understand this fully
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (const sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        die("bind");
    }
    if (listen(server_fd, SOMAXCONN) == -1) {
        die("listen");
    }

    // TODO: Add timeout in socket for recv
    // TODO: Add timeout in socket for accept (using select or poll)

    std::cout << "Server listening on port: " << PORT << std::endl;
    return server_fd;
}

void handle_clients(int server_fd) {
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int client_fd;
        if ((client_fd = accept(server_fd, (struct sockaddr *)&clientAddr, &clientAddrLen)) < 0) {
            die("accept");
        }
        do_something(client_fd);
        if (close(client_fd)) {
            die("close");
        }
    }
}

int main() {

    int server_fd = setup_server();
    handle_clients(server_fd);
    if (close(server_fd)) {
        die("close");
    }
}