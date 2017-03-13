/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "server_UNIX.h"
#define MAX_TIMEOUT_SEC 180
#define MAX_TIMEOUT_USEC 0

#define DEBUG


int server_unix_action(int* listen_socket){
    
    // Here we will set up things to handle.
    
    int listener = *listen_socket;
    
    int newcomer;
    
    struct timeval tout;
    
    struct socket_entry new_entry;
    
    struct socket_entry *temp_entry;
    
    struct packet new_packet;
    
    int addr_size = sizeof(struct sockaddr_storage);
    
    //time out set
    tout.tv_sec = MAX_TIMEOUT_SEC;
    tout.tv_usec = MAX_TIMEOUT_USEC;
    
    
    fd_set all_user;
 
    FD_ZERO(&all_user);
    
    // insert the first entry in the ideal fd_set
    // this set is not governed as it is our "own"
    
    FD_SET(listener,&all_user);
    
    // copy for future uses
    fd_set read_fd = all_user;
    
    int nfds = listener + 1;
    
    
    // here we begin the painful loop
    
    int iResult;
    
    while(true){

        read_fd = all_user;
        
        tout.tv_sec = MAX_TIMEOUT_SEC;
        tout.tv_usec = MAX_TIMEOUT_USEC;
        
        iResult = select(nfds,&read_fd,NULL,NULL,&tout);
        // reset the timeout
        
        
#ifdef DEBUG
      printf("A loop is started\n,the function select has returned with result %d",iResult);      
#endif      
        
        if(iResult == -1){
        
            printf("The select function failed\n");
            return -1;
        }
        
        if(iResult == 0){
            printf("Timeout reached\n");
            
            // close all the available ports           
            return 0;
        }
    
        // if we reach here, then something interesting happens
        // its either somebody else wants to a connection
        // or a message
        
        
        if(FD_ISSET(listener,&read_fd)){
            
            // if a new connection is setup
            
            addr_size = sizeof(struct sockaddr_storage);
            newcomer = accept(listener,&(new_entry.ip_info),&addr_size);
            
            
            if(newcomer == -1 ){
                printf("Failed to accept\n");
                return -1;
            }
            
            new_entry.socket_ = newcomer;
          
            new_entry.is_client_id_set = false;
            
            new_entry.is_session_id_set = false;
            
            
            // here we will implement the data structure later
            // socket_db_add(new_entry);
            // here we will implement the data structure later
#ifdef DEBUG
      printf("An entry with socket id %d is built\n",newcomer);      
#endif
            
            
            
            FD_SET(newcomer,&all_user);            
            
            if(newcomer > nfds -1 ){
            
                nfds = newcomer + 1;
            
            }
            continue;
        }
        
        
        
        // if we reach here, then a packet is sent;
        unsigned i;
        for(i = 0;i < nfds;i++){
            
            if(FD_ISSET(i,&read_fd)){
                break;
            }
        }
        
        if(i == nfds){
        
            printf("unexpected error occurred\n");
            return -1;
        
        }
        
        char* buffer;
        
        buffer = malloc(sizeof(int)*2+sizeof(char)*(MAX_NAME+MAX_DATA));
        
        iResult = recv(i,buffer,sizeof(int)*2+sizeof(char)*(MAX_NAME+MAX_DATA),0);
    
        if(iResult < 0){
        
            printf("Receive function failed\n");
            free(buffer);
            return -1;
        }
        
        if(iResult == 0){
        // close the socket and move on
            
            close(i);
            
            // required further implementation
            
            //socket_db_remove_sid(i);
            // required further implementation
            
            FD_CLR(i,&all_user);
            
 
        
        
        }
        
        if(!extraction(buffer,&new_packet)){
            
            printf("received an invalid packet,continue\n");
            free(buffer);
            continue;
            
        }
    
    
    // from this point on we should be able to handle the packets.
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    }
    
    























}
