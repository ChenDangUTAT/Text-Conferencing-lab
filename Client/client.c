#include "client.h"

// what we want is that we want to build
// the handler in this client 

char *process_msg_phase_two(int type,int size, const char* clientID, const char* msg);
int establishConnection(const char* IP_address, char* portNum);

char* processMsgAndType(char **msg, int numberOfInputs, int* connectionEstablished, char** clientID){


	unsigned int type = 0;
	// case statements
	// for number of inputs
	switch (numberOfInputs){
	
		case 0:
			printf("You should not enter 0 inputs\n");
			return NULL;
			break;
		// for the one input case
		case 1:
			// leave the session case
			if(strcmp(msg[0],"/leavesession") == 0){
				type=LEAVE_SESS;
				

			}
			else if(strcmp(msg[0],"/list") == 0){
				type= QUERY;
			
			}
			else if(strcmp(msg[0],"/logout") ==0){
				type =EXIT;

			}
			else if(strcmp(msg[0],"/quit")==0){
				// close the connection without notifying

			}

			else{
				
			}
			break;
	
		case 2:
			if(strcmp(msg[0],"/joinsession")==0){



			}
			else if(strcmp(msg[0],"/createsession")==0){


			}
			else{
				
			}
			break;
		case 5:
			if(strcmp(msg[0],"/login")==0){
				type = LOGIN;
				// check if the connection is set
				if(!(*connectionEstablished)){
					printf("test 3%s", msg[3]);
					printf("test 4%s", msg[4]);
					int resultCheck = establishConnection(msg[3],msg[4]);
					if(resultCheck == -1){
						printf("failed connection\n");
						return NULL;
					}
					// we pack the message
					else{
						printf("test 1\n");
						char *tempMsg = malloc(sizeof(char) *400);
						strcat(tempMsg,msg[1]);
						strcat(tempMsg,":");
						strcat(tempMsg,msg[2]);
						// copy the second msg into client ID
						*clientID = msg[1];
						
						printf("temp msg%s\n",tempMsg);			
						printf("%d test h\n", strlen(tempMsg));
						return process_msg_phase_two(type,strlen(tempMsg),*clientID,tempMsg);
					}
				}
				else{
					//check if log in yet
					if(clientID!=NULL){
						printf("error, you already logged in\n");
						return NULL;
					}
					// we pack 
					else{
						//strcat the result in the form of
						// type:size:clientID,data
						printf("test 2\n");
						char *tempMsg = malloc(sizeof(char) *400);
						strcat(tempMsg,msg[1]);
						strcat(tempMsg,":");
						strcat(tempMsg,msg[2]);
						printf("temp msg%s\n",tempMsg);			
						*clientID = msg[1];
						
						return process_msg_phase_two(type,strlen(tempMsg),*clientID,tempMsg);

					}
				}
			}
			else{
				type = MESSAGE;
				}
			
			break;
		
		default: 
			type = MESSAGE;
			
			return NULL;

	}



}


/** to process the second message to input for different cases */
char *process_msg_phase_two(int type,int size, const char* clientID, const char* msg){

		char *msgGot = malloc(sizeof(char)*500);
		char *temp = malloc(sizeof (20));
		printf("test hhhhhhh2\n");
		sprintf(temp, "%d", type);
		strcat(msgGot,temp);
		printf("test hhhhhhh1\n");
		sprintf(temp,"%d",size);
		strcat(msgGot,":");
		printf("test hhhhhhh3\n");
		strcat(msgGot,temp);
			printf("test hhhhhhh4\n");
		strcat(msgGot,":");
		printf("test hhhhhhh5\n");
		strcat(msgGot,clientID);
		printf("test hhhhhhh6\n");
		strcat(msgGot,":");
		printf("test hhhhhhh\n");
		strcat(msgGot,msg);	
		return msgGot;
}


// establish a tcp connection
int establishConnection(const char* IP_address, char* portNum){
	
	// then use get address to set up
	struct addrinfo hints, *res;
	
	//a status to check if the get address succeed or not
	int status;	

	memset(&hints,0,sizeof hints);
	hints.ai_family = AF_UNSPEC;
	//for TCP
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // fill in IP for you 

	portNum[strlen(portNum)-1] ='\0';
	printf("test port %s\n",portNum);
	if((status = getaddrinfo(IP_address,portNum,&hints,&res)) !=0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
		printf("can not get address successfuly\n");
 		return -1;
	}
 	int socDesc; 
	// get the socket desciptor
	socDesc = socket(res->ai_family,res->ai_socktype, res->ai_protocol);

	if(socDesc == 0){
		printf("soc get failed\n");
		return -1;
	}

	// we establish a connection
	int connector = connect(socDesc, res->ai_addr,res->ai_addrlen);
	if(connector < 0){
		printf("connection failed\n");
		return -1;
	}
	else 
		return 1;
}


//char* checkQuitValidity(


//void establishConnection(




//void 

/**
* 1: we need to process the message 
* 2: we set the message into type:size:ID:data
* 3: we establish a TCP connection
* */
int main(int argc,char *argv[]){

	// check the number of arguments
	if(argc!=1){
		printf("Sorry, you have to only enter the client to run\n");
	}
	
	// now we want to check for the message they have
	
 	// there are serveral types of messages, if all of them 
   // are NULL, then we return false;
	// we do a self message check at this position


	// there isn't a connection at the beginning
	int connectionEstablished = 0;

	char* clientID =NULL;
	do{
		// we use fgets to get entire line and decode
		char msg[150];
		// get the standard input
		if(fgets(msg,150,stdin)!=NULL){
			// here we then want to process 
			// the message, allocate 5 pointers for the msg
			char ** msgArray = (char **) malloc(sizeof(char *) *500);

			int counter = 0;

			// first token is got 
			msgArray[0] = strtok(msg," ");
			
			// to factor out all of the spaces
			while(msgArray[counter]!=NULL){
				// check the result
				counter++;	
				// separate by the space
				msgArray[counter] = strtok(NULL," ");
			}
			// we check for the counter value 
			printf("first test %s\n", msgArray[2]);
			char *processedMsg = processMsgAndType(msgArray,counter,&connectionEstablished,&clientID);	
			printf("The processed msg is %s\n", processedMsg);

			
			
		}
		else{
			printf("you have to enter some thing\n");
			continue;
		}

	}while(1);
	// we use a do while loop to check
	// if user /quit or not

}



