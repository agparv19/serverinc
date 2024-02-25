#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <cassert>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2000

#define MAX_PACKET_SIZE 4096

void die(const char* msg);

void msg(const char *msg);

void print(const char *msg);

int recv_exactly(int fd, char* buf, size_t n_bytes);

int write_exactly(int fd, const char* buf, size_t n_bytes);

int recv_one_packet(int fd, char* buf);

int send_one_packet(int fd, const char* buf);