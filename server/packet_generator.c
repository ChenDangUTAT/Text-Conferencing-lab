/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "packet_generator.h"


char* msg_generator(struct packet input,unsigned* size){


    char* reply = malloc((sizeof(int)*2+sizeof(char)*(MAX_NAME+MAX_DATA)));
    
    *size = sprintf(reply,"%u:%u:%s:%s",input.type,input.size,input.source,input.data);
    
    *size = *size + 1 ;
    
    return reply;
            













}
