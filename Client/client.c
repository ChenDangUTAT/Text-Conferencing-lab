#include "client.h"

/******************Data Section**********************************/

// get the socket descriptor 
int socketglobal = 0;
int quitIndicator = 0;
int connectionEstablished = 0;
char actualClient[100];


/******************Data End *************************************/


// what we want is that we want to build
// the handler in this client 

char *process_msg_phase_two(int type,int size, const char* msg);
int establishConnection(const char* IP_address, char* portNum);


char* processMsgAndType(char **msg, int numberOfInputs, int* connectionEstablishedTest){

	//printf("wtf %s\n",actualClient);


	char *testMsg = malloc(sizeof(char)*400);
	
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

			// potential fk up!
			strcpy(testMsg,msg[0]);
			// get the test msg last to be 0
			testMsg[strlen(testMsg)-1]='\0';
			//printf("wtf2 %s\n",actualClient);
			//printf("testMsg output %s\n",testMsg);
			if(*connectionEstablishedTest!=0 && strcmp(testMsg,"/quit")!=0){
				if(strcmp(testMsg,"/leavesession") == 0){
					type= LEAVE_SESS;
					char *leaveSessMsg ="leaveSess";
					return process_msg_phase_two(type,strlen(leaveSessMsg),leaveSessMsg);
				}
				else if(strcmp(testMsg,"/list") == 0){
					type= QUERY;
					char *queryMsg ="query";
					return process_msg_phase_two(type,strlen(queryMsg),queryMsg);
				}
				else if(strcmp(testMsg,"/logout") ==0){
					type =EXIT;
					// we want to log out
					char *logoutMsg ="logout";
					//printf("the client id is %s\n",actualClient);
					char * test = process_msg_phase_two(type,strlen(logoutMsg),logoutMsg);
					//printf("the client id is %s\n",actualClient);
					return test;

				}
				// set the message
				else{
					type = MESSAGE;
					return process_msg_phase_two(type,strlen(testMsg),testMsg);
				}
		}
			else{
				//printf("test \n");
				if(strcmp(testMsg,"/quit")==0 ){
					// close the connection without notifying
					//printf("test 2\n");
					if(*connectionEstablishedTest!=0){
						type =EXIT;
						char *quitMsg ="quit";
						quitIndicator = 1;
						return process_msg_phase_two(type,strlen(quitMsg),quitMsg);
					}
					else{
							//printf("test 6\n");
						quitIndicator = 1;
						return NULL;

	
					}
				}
				else if(*connectionEstablishedTest ==0){
					// showing an error message

					printf("Please establish a connection first\n");
					return NULL;
				}
				//printf("test 4\n");
			}
			break;
	
		case 2:
			if(*connectionEstablishedTest==1){
				if(strcmp(msg[0],"/joinsession")==0){
					// get the type as join
					strcpy(testMsg,msg[1]);
					// get the test msg last to be 0
					testMsg[strlen(testMsg)-1]='\0';
					type =JOIN;
					return process_msg_phase_two(type,strlen(testMsg),testMsg);

				}
				else if(strcmp(msg[0],"/createsession")==0){

	
					strcpy(testMsg,msg[1]);
					// get the test msg last to be 0
					testMsg[strlen(testMsg)-1]='\0';
					// get the new session
					type=NEW_SESS;
					// we send the message for creating new session
					return process_msg_phase_two(type,strlen(testMsg),testMsg);
				}
				else{
					// send the msg as string
				
					type=MESSAGE;
					char *tempMsg = malloc(sizeof(char) *400);
					strcat(tempMsg,msg[0]);
					strcat(tempMsg,":");
					strcat(tempMsg,msg[1]);

					return process_msg_phase_two(type,strlen(tempMsg),tempMsg);
				}
		}	
			else{
				printf("Please establish a connection first\n");
				return NULL;
			}
			break;
		case 5:
			if(strcmp(msg[0],"/login")==0){
				type = LOGIN;
				// check if the connection is set
				if(*connectionEstablishedTest==0){
					//printf("test 3%s", msg[3]);
					//printf("test 4%s", msg[4]);
					int resultCheck = establishConnection(msg[3],msg[4]);
					if(resultCheck == -1){
						printf("failed connection\n");
						return NULL;
					}
					// we pack the message
					else{
						// set the connection established
						*connectionEstablishedTest =1;
						char *tempMsg = malloc(sizeof(char) *400);
						strcat(tempMsg,msg[1]);
						strcat(tempMsg,":");
						strcat(tempMsg,msg[2]);
						// copy the second msg into client ID
						strcpy(actualClient,msg[1]);
						
						//printf("test client ID intializer%s\n",actualClient);
						//printf("temp msg%s\n",tempMsg);			
						//printf("%d test h\n", strlen(tempMsg));
						return process_msg_phase_two(type,strlen(tempMsg),tempMsg);
					}
				}
				else{
					//check if log in yet
					
						printf("error, you already logged in\n");
						return NULL;
					//}
					// we pack 
					
						
						//strcat the result in the form of
						// type:size:clientID,data
						//printf("test 2\n");
						//char *tempMsg = malloc(sizeof(char) *400);
						//strcat(tempMsg,msg[1]);
						//strcat(tempMsg,":");
						//strcat(tempMsg,msg[2]);
						//printf("temp msg%s\n",tempMsg);			
						//strcpy(actualClient,msg[1]);
						//printf("test client ID intializer%s\n",actualClient);
						//return process_msg_phase_two(type,strlen(tempMsg),tempMsg);

					
				}
			}
			else{
				if(*connectionEstablishedTest==1){
					type = MESSAGE;
					// to create a counter to send all the message
					int msgCounter = 0;
					// use a for loop to output the mssage out
					char *tempMsg = malloc(sizeof(char) *400);
					for(; msgCounter< numberOfInputs; msgCounter++){
						//concat the message into
						strcat(tempMsg,msg[msgCounter]);
						// concat the colon
						if(msgCounter != numberOfInputs -1)
							strcat(tempMsg,":");
					}
					return process_msg_phase_two(type,strlen(tempMsg),tempMsg);
			}
				else{
					printf("Please establish a connection first\n");
					return NULL;
				}
				}
			
			break;
		
		default: 
			if(*connectionEstablishedTest ==1){
				type = MESSAGE;
				// use a for loop to output the mssage out
				char *tempMsg = malloc(sizeof(char) *400);
				int msgCounter = 0;
				for(; msgCounter< numberOfInputs; msgCounter++){
						//concat the message into
					strcat(tempMsg,msg[msgCounter]);
						// concat the colon
					if(msgCounter != numberOfInputs -1)
						strcat(tempMsg,":");
				}
				return process_msg_phase_two(type,strlen(tempMsg),tempMsg);
			}
			else{

				printf("Please establish a connection first\n");
				return NULL;
			}
	}



}


/** to process the second message to input for different cases */
char *process_msg_phase_two(int type,int size, const char* msg){

		char *msgGot = malloc(sizeof(char)*500);
		char *temp = malloc(sizeof (20));
		sprintf(temp, "%d", type);
		strcat(msgGot,temp);
		sprintf(temp,"%d",size);
		strcat(msgGot,":");
		strcat(msgGot,temp);
		strcat(msgGot,":");
		strcpy(temp,actualClient);
		strcat(msgGot,temp);
		//printf("test change for client ID%s\n",actualClient);
		strcat(msgGot,":");
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
	else {
		socketglobal = socDesc;
		return 1;
}
}

// now what we need to do is to 
// create a pthread_stub 

//char* checkQuitValidity(

// the thread to output messages, to receive msgs etc
// in the client side
void* pthread_stub(void *arg){
	// want this pthread to handle

	// all the receive msg from the user
	printf("pthread _ test \n");
	// if not yet to be quitted
	// then we keep the thread opening
	while(1 &&!quitIndicator){
		while(connectionEstablished){
			//printf("pthread _ test  multiple\n");
			// we want to keep waiting for the response
			char *recvMsg = malloc(sizeof(char) *400);
			// we keep receiving the numOfBytes from buffer
			int numOfBytes = recv(socketglobal,recvMsg,400,0);
			
			struct lab3message recvMsgPack;
			// indicate the decode is success or not
			int success_indicator = decodeMsg(recvMsg,&recvMsgPack);
			
			// loop back if decode fails
			if(success_indicator == -1){
				continue;
			}
			// check up to here for the type
			switch(recvMsgPack.type){
				case LO_ACK:
					printf("login successful %s\n");
					break;
				case LO_NAK:
					printf("the reason for  login failure is%s\n",recvMsgPack.data);
					connectionEstablished = 0;
					break;
				case JN_ACK:
					printf("join the session successfully%s\n",recvMsgPack.data);
					break;
				case JN_NAK:
					printf("not able to join the session%s\n",recvMsgPack.data);
					break;
				case NS_ACK:
					printf("session acknowledged, sessionID is %s\n",recvMsgPack.data);
					break;
				case QU_ACK:
					printf("\n The queries are\n%s\n",recvMsgPack.data);
					break;
				case MESSAGE:
					printf("%s\n",recvMsgPack.data);
					break;
				case EXIT:
					// since we are receiving, it has to be logged in
					connectionEstablished = 0;
					close(socketglobal);
					socketglobal = 0;
					printf("successfully logged out,%s\n",recvMsgPack.data);
					break;
				case LEAVE_SESS:
					printf("\n successfully leave the session %s\n",recvMsgPack.data);
					break;				
				default:
					printf("server has servere problem \n");

			}



		}
	
	}


}

// decode the received msg, and return a struct
// if successfully decoded report 1,
// else report -1;
int decodeMsg(char *recvMsg, struct lab3message* recvMsgStruct){
	

	//see the type
	int msgType = 1000000;
	
	// use a processor for a middle state 
	char** processor = (char **) malloc(sizeof(char *) *500);
	
	int counter = 0;

	// first token is got 
	processor[0] = strtok(recvMsg,":");
			
		// to factor out all of the spaces
	while(counter <2){
		counter++;
		// separate by the space
		processor[counter] = strtok(NULL,":");
	}
	// get the last part out
	processor[3] = strtok(NULL,"");
	//printf("test to see what are left%s\n",recvMsg);
	//printf("test to see what left #2 %s\n",processor[3]);
	// decode the first type to enter switch statement
	msgType = atoi(processor[0]);

	if(msgType ==0){
		printf("the string can not be decoded\n");
		return -1;
	}
	
	int size = atoi(processor[1]);
	if(size ==0){
		printf("the string can not be decoded\n");
		return -1;
	}
	//printf("final section test\n");
	recvMsgStruct->size = (unsigned int)size;
	recvMsgStruct->type =(unsigned int) msgType;
	// copy into the result
	strcpy(recvMsgStruct->source,processor[2]);
	strcpy(recvMsgStruct->data,processor[3]);
		
	
	return 1;
}

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

	// get a receiver thread
	pthread_t receiver_thread;
	
	// get the attribute
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	// create the pthread to receive the second info
	pthread_create(&receiver_thread,&attr,pthread_stub,NULL);

	char **msgArray = (char **) malloc(sizeof(char *) *500);
	do{
		// we use fgets to get entire line and decode
		char msg[150];
		// get the standard input
		if(fgets(msg,150,stdin)!=NULL){
			// here we then want to process 
			// the message, allocate 5 pointers for the msg
				
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
			//printf("first test %s\n", msgArray[2]);
			char *processedMsg = processMsgAndType(msgArray,counter,&connectionEstablished);	
			//printf("The processed msg is %s\n", processedMsg);
			//printf("frustrating test %s\n",actualClient);
			// we want to  send this msg to the msg
			if(connectionEstablished && processedMsg !=NULL){
				send(socketglobal,processedMsg,strlen(processedMsg)+1,0);
				//printf("first message sent test\n");
			}
			//printf("frustrating test 2 %s\n",actualClient);
			int testCount = 0;
			for(;testCount <500; testCount ++){
				msgArray[testCount] = NULL;
			}
		}
		else{
			printf("you have to enter some thing\n");
			continue;
		}

	}while(!quitIndicator);
	// we use a do while loop to check
	// if user /quit or not
	connectionEstablished = 0;
	// close the socket
	if(socketglobal)	
		close(socketglobal);
	//printf("test 5\n");
	exit(0);
}



