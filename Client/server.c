#include "server.h"


int main(int argc,char* argv[]) {

	// check if there is one argument to the function
	if (argc != 2) {
		printf("usage: %s <port number>\n",argv[0]);
		return 1;
	}

	//check if the port number is a number

	char* udp = argv[1];

	for (; *udp!= '\0'; udp = udp + 1) {
	
		if (!isdigit(*udp)) {
#ifdef DEBUG

                    printf("The non-digit is %c\n",*udp);

#endif
                    
			printf("The port must be an integer between %d to %d\n",TCP_PORT_LOW_RANGE,TCP_PORT_HIGH_RANGE);
			return 1;
		
		}
	}

	//checking whether the udp port is valid or not given that the input is an integer

	int udp_number = atoi(argv[1]);

	if (udp_number > TCP_PORT_HIGH_RANGE || udp_number < TCP_PORT_LOW_RANGE) {

		printf("The port must be an integer between %d to %d\n", TCP_PORT_LOW_RANGE, TCP_PORT_HIGH_RANGE);
#ifdef DEBUG

                    printf("The invalid string is %s, and the corresponding number is %d\n",argv[1],udp_number);

#endif   
             
		return 1;

	}

	// here we should have a valid input awaiting to to processed 

 #ifdef WIN

		SOCKET socket_;

		int iResult;

		iResult =  server_win_setup(argv[1],&socket_);

		if (iResult != 0) {
		
		
			wprintf("setup failed\n");
		    
		
		}

		if (iResult == 0) {
			
			
			iResult = server_win_action(&socket_);

			if (iResult != 0) {
			
				wprintf("functionality failed\n");
			
			}
		
		
		}
		WSACleanup();

	
	}
#endif

#ifdef UNIX



                
                int listen_socket;

		int iResult;

		iResult = server_unix_setup(argv[1],&listen_socket);

		if (iResult != 0) {


			printf("setup failed\n");


		}

		if (iResult == 0) {


			iResult = server_unix_action(&listen_socket);

			if (iResult != 0) {

				printf("functionality failed\n");

			}


		}
                
                
                close(listen_socket);

	
	
	


#endif


	return 0;

}