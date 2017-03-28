/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "session_db.h"

unsigned int session_db_add(char* session_name,int socket_, struct session_entry *head){
    
    // we add the element into the session
       struct session_entry* tail = head;
       if( session_db_search_sname(session_name,head)!=NULL){
       
           //session already exists
           return 0;
           
           
           
       }
    
       
       
       
       while(tail->next != NULL){
        tail = tail->next;

    }    
       
       
    struct session_entry* new_entry = malloc(sizeof(struct session_entry));
    
    new_entry->session_tag = tail->session_tag+1;
    
    tail->next = new_entry;
    
    new_entry->next = NULL;
    
    strcpy(new_entry->session_id,session_name);
    
    bzero(new_entry->socket_id,sizeof(int)*MAX_MEM);
    
    new_entry->socket_id[0]=socket_;
    
    
    return new_entry->session_tag;

    
}


bool session_db_remove(unsigned int session_tag, struct session_entry *head){
    
     struct session_entry* start = head -> next;
    
    struct session_entry* before = head;
    
    while(start != NULL){
    
        if(start->session_tag == session_tag){
        
            break;
        
        }
        
        before = start;
        
        start = start-> next;
    
    
    }

    if(start == NULL){
    // there is no such entry
        return false;
    }
    
    // there is such entry 
    
    before->next = start->next;
    
    free(start);
    
    return true;

    
    
    
}


struct session_entry* session_db_search_sid(unsigned int session_tag,struct session_entry* head){
    
    
    struct session_entry* start = head -> next;
    
    while(start != NULL){
        
        if(start->session_tag == session_tag){
        
            return start;
        
        }
    
        start = start -> next;
    }

    return start;



    
}

struct session_entry* session_db_search_sname(char* name,struct session_entry* head){

    struct session_entry* start = head->next;
    
    while(start != NULL){
        if(strcmp(start->session_id,name)==0){
        
            return start;
        
        }
        start = start -> next;
 
    }
    return start;
    

}

char* session_db_traverse(struct session_entry* head){

    struct session_entry* start = head->next;
    
    char* msg = malloc(sizeof(char)*400);
    
    bzero(msg,sizeof(char)*400);
    
    strcat(msg,"The session ids are :");
    
    while(start != NULL){
    
        strcat(msg,start->session_id);
        strcat(msg,"  ");
    
    
    
        start = start->next;
               
    
    }
    



    return msg;

}


bool session_db_join_socket(int socket_, struct session_entry * session){
    unsigned int spot = 0;
    
    for(;session->socket_id[spot]!=0 && spot < MAX_MEM;spot++){
        if(session->socket_id[spot] == socket_){
        
            return false;
        
        }
    
    
    
    
    
    }
    
    if(spot == MAX_MEM){return false;}
    
    session->socket_id[spot] = socket_;
    
    return true;










}


bool session_db_leave_socket(int socket_, struct session_entry* session,struct session_entry *head){
    unsigned int spot = 0;
    
    for(;session->socket_id[spot] != socket_&& spot < MAX_MEM;spot++){}
    
    if(spot == MAX_MEM){
        return false;
    }

    for(;spot != MAX_MEM-1;spot++){
    
        session->socket_id[spot] =session->socket_id[spot+1];
    
    
    }
    

    session->socket_id[spot] = 0;
    
    if(session->socket_id[0]==0){
    
    session_db_remove(session->session_tag, head);
    
    }
    
    
    
    return true;






}


bool session_db_deletion(struct session_entry *head){


    if (head->next == NULL) {
        free(head);
        return true;
    }
    else {
        struct session_entry* next = head->next;
        free(head);
        return socket_db_deletion(next);
    }



}