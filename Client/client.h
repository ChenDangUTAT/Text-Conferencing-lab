
// specify the system manually

#define win 1
#define UNIX 0
#define UDP_PORT_LOW_RANGE 0
#define UDP_PORT_HIGH_RANGE 65535
#define MAX_NAME 500
#define MAX_DATA 1000


#define LOGIN 1001
#define LO_ACK 1002
#define LO_NAK 1003
#define EXIT 1004
#define JOIN 1005
#define JN_ACK 1006
#define JN_NAK 1007
#define LEAVE_SESS 1008
#define NEW_SESS 1009
#define NS_ACK 1010
#define MESSAGE 1011
#define QUERY 1012
#define QU_ACK 1013




// other common headers

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <math.h>


struct lab3message {
	unsigned int type;
	unsigned int size;
	unsigned char source[MAX_NAME];
	unsigned char data[MAX_DATA];
};
