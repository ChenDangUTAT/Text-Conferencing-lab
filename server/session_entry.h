
#pragma once
#include <stdbool.h>
#include <netinet/in.h>
#include <stdlib.h>

#ifndef MAX_NAME
#define MAX_NAME 100
#endif
#ifndef MAX_MEM
#define MAX_MEM 100
#endif

struct session_entry{

    unsigned int session_tag;
    
    char session_id[MAX_NAME];
    
    int socket_id[MAX_MEM];
   
    struct session_entry* next;

};



