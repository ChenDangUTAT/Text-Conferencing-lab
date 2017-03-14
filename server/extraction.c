/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "extraction.h"
#define DEBUG


bool extraction(char* msg_string, struct packet* packet) {
    // extract total_frag
#ifdef DEBUG
    printf("The raw string is %s\n\n\n", msg_string);
#endif
    
    unsigned ext_counter = 0;

    for (; msg_string[ext_counter] != ':'; ext_counter++) {

    }

    packet->type = atoi(msg_string);

#ifdef DEBUG

    printf("The type number is %d\n", packet->type);


#endif

    // extract complete


    ext_counter++;

    // extract size

    packet->size = atoi(msg_string + ext_counter);

    for (; msg_string[ext_counter] != ':'; ext_counter++) {

    }
    
    if(packet->size>MAX_DATA){
        printf("The size received is not valid\n");
        
        return false;
    }

#ifdef DEBUG

    printf("The size is %d\n", packet->size);


#endif

    // extract name

    ext_counter++;

    unsigned name_counter = 0;

    for (; msg_string[ext_counter + name_counter] != ':'; name_counter++) {
        
        
        if(name_counter > MAX_NAME-1){
        
            printf("The name is too long\n");
            return false;
            
            
            
        }
        packet->source[name_counter] = msg_string[ext_counter + name_counter];

    }
    packet->source[name_counter] = '\0';
    ext_counter += name_counter;
    // extraction data

#ifdef DEBUG

    printf("The file source is %s\n", packet->source);

#endif

    unsigned i=1;
    for (; i < packet->size + 1; i++) {

        packet->data[i - 1] = msg_string[ext_counter + i];


   }
    
    packet->data[packet->size+1] = '\0';
    
#ifdef DEBUG
    
    printf("The file data is %s\n",packet->data);
    
#endif
    
    
    
    //memcpy(packet->filedata,msg_string+ext_counter+1,packet->size);
    
#ifdef DEBUG
    //printf("The content of the file is ");
    //for(i = 0 ; i < packet->size;i++)
    //printf("%c", packet->filedata[i]);
    //printf("\n");
#endif



    return true;
    //extraction complete



}
