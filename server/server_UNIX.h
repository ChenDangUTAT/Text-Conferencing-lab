#pragma once



//determining the receiver buff size

// the maximum link allowed, for our uses, it should not be very large
#define MAX_PENDING_LINK 8




// other necessary libraries for UNIX like system
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "type_header.h"
#include "extraction.h"
#include "socket_entry.h"
#include "packet_generator.h"


// this function is responsible for setting up the UDP receiving socket
int server_unix_setup(char* udp_port,int* listen_socket);


// this function should be handling UDP socket behavior and should receive update over time
int server_unix_action(int* listen_socket);




