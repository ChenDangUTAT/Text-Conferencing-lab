/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "socket_db.h"

struct socket_entry* socket_db_add(struct socket_entry newcomer, struct socket_entry* head) {

    struct socket_entry* tail = head;

    while (tail->next != NULL) {
        tail = tail->next;

    }
    struct socket_entry* new_entry = malloc(sizeof (struct socket_entry));

    *new_entry = newcomer;

    tail->next = new_entry;

    return head;


}

struct socket_entry* socket_db_rm_sid(int socket_id, struct socket_entry* head) {

    struct socket_entry* start = head -> next;

    struct socket_entry* before = head;

    while (start != NULL) {

        if (start->socket_ == socket_id) {

            break;

        }

        before = start;

        start = start-> next;


    }

    if (start == NULL) {
        // there is no such entry
        return head;
    }

    // there is such entry 

    before->next = start->next;

    free(start);

    return head;









}

struct socket_entry* socket_db_search_sid(int socket_id, struct socket_entry* head) {

    struct socket_entry* start = head -> next;

    while (start != NULL) {

        if (start->socket_ == socket_id) {

            return start;

        }

        start = start->next;


    }

    return start;











}

struct socket_entry* socket_db_search_cid(char* client_id, struct socket_entry* head) {

    struct socket_entry* start = head -> next;

    while (start != NULL) {

        if (start->is_client_id_set == true) {
            if (strcmp(start->client_ID, client_id) == 0) {

                // match
                return start;


            }
        }


        start = start->next;

    }

    return start;













}

char* socket_db_traverse(struct socket_entry* head) {

    struct socket_entry* start = head->next;
  
    char *msg = malloc(sizeof (char)*400);
    
    bzero(msg,sizeof(char)*400);
    
    strcat(msg, "The clients are:");

    while (start != NULL) {
        strcat(msg, start->client_ID);
        strcat(msg, "  ");
        start = start->next;


    }

    return msg;











}

bool socket_db_join_session(struct socket_entry* tar,unsigned int session_tag){


            unsigned int empty = 0;
            if (tar->is_session_id_set == true) {
               

                for (; tar->session_ID[empty] == 0 ||tar->session_ID[empty] == session_tag|| empty > 9; empty++) {

                }

                if (tar->session_ID[empty] == session_tag||empty > 9) {

                    //send rejection, max section joined

                    return false;

                }

                tar->session_ID[empty] = session_tag;
                
            } else {

                tar->session_ID[0] = session_tag;

                tar->is_session_id_set = true;
            }

            return true;









}

