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

bool socket_db_rm_sid(int socket_id, struct socket_entry* head) {

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
        return false;
    }

    // there is such entry 

    before->next = start->next;

    free(start);

    return true;









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

    bzero(msg, sizeof (char)*400);

    strcat(msg, "The clients are:");

    while (start != NULL) {
        strcat(msg, start->client_ID);
        strcat(msg, "  ");
        start = start->next;


    }

    return msg;











}

bool socket_db_join_session(struct socket_entry* tar, unsigned int session_tag) {


    unsigned int empty = 0;
    if (tar->is_session_id_set == true) {


        for (; tar->session_ID[empty] != 0 && tar->session_ID[empty] != session_tag && empty < MAX_SESS-1; empty++) {

        }

        if (tar->session_ID[empty] == session_tag || empty > MAX_SESS-1) {

            //send rejection, max section joined or session already joined 

            return false;

        }

        tar->session_ID[empty] = session_tag;

    } else {

        tar->session_ID[0] = session_tag;

        tar->is_session_id_set = true;
    }

    return true;









}

bool socket_db_leave_session(struct socket_entry* tar, unsigned int session_tag) {

    unsigned spot = 0;

    for (; spot < MAX_SESS && tar->session_ID[spot] != session_tag; spot++) {
    }

    if (spot == MAX_SESS) {

        return false;

    }


    for (; spot < (MAX_SESS - 1); spot++) {


        tar->session_ID[spot] = tar->session_ID[spot + 1];



    }
    
    tar->session_ID[MAX_SESS] = 0;
    spot = 0;


    for (; spot < MAX_SESS && tar->session_ID[spot] != 0; spot++) {
    }

    if (spot == 0) {

        tar->is_session_id_set = false;

    } else {

        tar->is_session_id_set = true;


    }

    return true;

















}

bool socket_db_deletion(struct socket_entry* head) {
    close(head->socket_);
    if (head->next == NULL) {
        free(head);
        return true;
    } else {
        struct socket_entry* next = head->next;
        free(head);
        return socket_db_deletion(next);
    }
















}