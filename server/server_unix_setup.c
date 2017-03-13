#include "server_UNIX.h"


int server_unix_setup(char* tcp_port,int* listen_socket) {


    struct addrinfo hints;

    struct addrinfo* result = NULL;

    int iResult = 0;


    // set up the hint for the getaddrinfo()

    // IPv4
    hints.ai_family = AF_INET;

    //TCP
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // fill in my IP as server
    hints.ai_flags = AI_PASSIVE;


    // this will get the information we need to create a socket
    iResult = getaddrinfo(NULL,tcp_port, &hints, &result);


    if (iResult != 0) {

        printf("Get Address Info failed\n");
        return -1;

    }


#ifdef DEBUG

    printf("The port number is %s \n", udp_port);

#endif

    // now we have the info, we will open up a socket

    int socket_;

    socket_ = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (socket_ == -1) {

        printf("Failed to create socket descryptor\n");

        return -1;


    }


    // now we bind

    iResult = bind(socket_, result->ai_addr, (int) result->ai_addrlen);

    if (iResult != 0) {
#ifdef DEBUG

        printf("The error code is %d\n", iResult);
        printf("The address protocol is %s   \n", result->ai_addr->sa_data);
        printf("The address lenght is %d\n", result->ai_addrlen);


#endif



        printf("Failed to bind the socket\n");
        return -1;


    }
    
    
    // now we listen
    iResult = listen(socket_,MAX_PENDING_LINK);
    if (iResult != 0) {
        
	printf("Failed to execute listen\n");
        if(errno == EADDRINUSE ){
            printf("The error is \nAnother socket is already listening on the same port.\n");
        
        }
        if(errno == EBADF){
        
            printf("The error is \nThe argument sockfd is not a valid file descriptor.\n");
        
        }
        
        if(errno == ENOTSOCK){
        
            printf("The error is \n The file descriptor sockfd does not refer to a socket.\n");
        
        }
        
        if(errno == EOPNOTSUPP){
        
            printf("The error is \n   The socket is not of a type that supports the listen()operation.\n");
        }
        
        return -1;
    
    
    
    
    }
    
    
    
    
    // the act of accept is handled in the next session.    
    freeaddrinfo(result);

    // and we pass the "binded" socket descryptor to the next function
    *listen_socket = socket_;

    return 0;











}

