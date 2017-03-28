/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "server_UNIX.h"
#include "session_entry.h"

#define MAX_TIMEOUT_SEC 180
#define MAX_TIMEOUT_USEC 0

int server_unix_action(int* listen_socket) {

    // Here we will set up things to handle.

    struct session_entry * head_s = malloc(sizeof (struct session_entry));

    int listener = *listen_socket;

    int newcomer;

    struct timeval tout;

    struct socket_entry new_entry;

    struct socket_entry *temp_entry;

    struct packet new_packet;

    struct packet reply_packet;

    struct socket_entry* head = malloc(sizeof (struct socket_entry)*1);

    bzero(head, sizeof (struct socket_entry));

    head ->next = NULL;

    bzero(head_s, sizeof (struct session_entry));

    head_s->next = NULL;

    int addr_size = sizeof (struct sockaddr_storage);

    //time out set
    tout.tv_sec = MAX_TIMEOUT_SEC;
    tout.tv_usec = MAX_TIMEOUT_USEC;


    fd_set all_user;

    FD_ZERO(&all_user);

    // insert the first entry in the ideal fd_set
    // this set is not governed as it is our "own"

    FD_SET(listener, &all_user);

    // copy for future uses
    fd_set read_fd = all_user;

    int nfds = listener + 1;


    // here we begin the painful loop

    int iResult;

    while (true) {

        read_fd = all_user;

        tout.tv_sec = MAX_TIMEOUT_SEC;
        tout.tv_usec = MAX_TIMEOUT_USEC;
#ifdef DEBUG
        printf("A loop is started\n");
#endif


        iResult = select(nfds, &read_fd, NULL, NULL, /*&tout*/NULL);
        // reset the timeout


#ifdef DEBUG
        printf("the function select has returned with result %d\n", iResult);
#endif      

        if (iResult == -1) {

            printf("The select function failed\n");
            break;
        }

        if (iResult == 0) {
            printf("Timeout reached\n");

            // close all the available ports           
            break;
        }

        // if we reach here, then something interesting happens
        // its either somebody else wants to a connection
        // or a message


        if (FD_ISSET(listener, &read_fd)) {

            // if a new connection is setup

            addr_size = sizeof (struct sockaddr_storage);
            newcomer = accept(listener, &(new_entry.ip_info), &addr_size);


            if (newcomer == -1) {
                printf("Failed to accept\n");
                return -1;
            }

            bzero(new_entry.session_ID, sizeof (unsigned int)*MAX_SESS);

            new_entry.socket_ = newcomer;

            new_entry.is_client_id_set = false;

            new_entry.is_session_id_set = false;

            new_entry.next = NULL;



            // here we will implement the data structure later
            socket_db_add(new_entry, head);
            // here we will implement the data structure later
#ifdef DEBUG
            printf("An entry with socket id %d is built\n", newcomer);
#endif



            FD_SET(newcomer, &all_user);

            if (newcomer > nfds - 1) {

                nfds = newcomer + 1;

            }
            continue;
        }



        // if we reach here, then a packet is sent;
        unsigned curr_socket;
        for (curr_socket = 0; curr_socket < nfds; curr_socket++) {

            if (FD_ISSET(curr_socket, &read_fd)) {
                break;
            }
        }

        if (curr_socket == nfds) {

            printf("unexpected error occurred\n");
            return -1;

        }

        char *buffer;

        buffer = malloc(sizeof (int)*2 + sizeof (char)*(MAX_NAME + MAX_DATA));

        iResult = recv(curr_socket, buffer, (sizeof (int)*2 + sizeof (char)*(MAX_NAME + MAX_DATA)), 0);

        if (iResult < 0) {

            printf("Receive function failed\n");
            free(buffer);
            return -1;
        }

        if (iResult == 0) {
            // close the socket and move on

            close(curr_socket);

            // required further implementation

            // socket_db_remove_sid(i);
            // required further implementation

            FD_CLR(curr_socket, &all_user);




        }

        if (!extraction(buffer, &new_packet)) {

            printf("received an invalid packet,continue\n");
            free(buffer);
            continue;

        }

        free(buffer);
        buffer = NULL;


        // from this point on we should be able to handle the packets.


        // login
        if (new_packet.type == LOGIN) {


            //need implementation
            //temp_entry = socket_db_access_by_sid(curr_socket);
            //need implementation

            temp_entry = socket_db_search_cid(new_packet.source, head);
            if (temp_entry != NULL) {

                unsigned msg_size;

                reply_packet.type = LO_NAK;
                strcpy(reply_packet.source, "server");
                strcpy(reply_packet.data, "User already login");
                reply_packet.size = strlen(reply_packet.data);

                char* msg = msg_generator(reply_packet, &msg_size);
#ifdef DEBUGLOGIN
                printf("The user %s is already login\n", temp_entry->client_ID);
                printf("The message sent should be %s\n", msg);
#endif
                send(curr_socket, msg, msg_size, 0);
                free(msg);
                continue;


            }

            temp_entry = NULL;
            unsigned password_size = new_packet.size - strlen(new_packet.source) - 1;

            char* password = malloc(sizeof (char)*(password_size + 1));

            memcpy(password, new_packet.data + strlen(new_packet.source) + 1, password_size);

            password[password_size] = '\0';

#ifdef DEBUGLOGIN

            printf("The password extracted is %s, it has a size of %d, the source client id is %s\n", password, password_size, new_packet.source);

#endif 

            if (/*password varification*/true) {

                unsigned msg_size;

                temp_entry = socket_db_search_sid(curr_socket, head);


                if (temp_entry == NULL) {
                    printf("Database error\n,the requested login is not found\n");
                    break;
                }

                temp_entry ->is_client_id_set = true;
                strcpy(temp_entry->client_ID, new_packet.source);

                reply_packet.type = LO_ACK;
                strcpy(reply_packet.source, "server");
                strcpy(reply_packet.data, "login_successful");
                reply_packet.size = strlen(reply_packet.data);

                char* msg = msg_generator(reply_packet, &msg_size);

#ifdef DEBUGLOGIN
                printf("The user %s successfully logged in \n", temp_entry->client_ID);
                printf("The message sent should be %s\n", msg);
#endif
                send(curr_socket, msg, msg_size, 0);
                free(msg);


            } else {

                unsigned msg_size;

                reply_packet.type = LO_NAK;
                strcpy(reply_packet.source, "server");
                strcpy(reply_packet.data, "Invalid Password");
                reply_packet.size = strlen(reply_packet.data);

                char* msg = msg_generator(reply_packet, &msg_size);
#ifdef DEBUGLOGIN
                printf("The message sent should be %s\n", msg);
#endif
                send(curr_socket, msg, msg_size, 0);
                free(msg);
                close(curr_socket);
                socket_db_rm_sid(curr_socket, head);
                FD_CLR(curr_socket, &all_user);






            }


            free(password);
            continue;





        }

        if (new_packet.type == EXIT) {

            temp_entry = NULL;

            temp_entry = socket_db_search_sid(curr_socket, head);

            if (temp_entry == NULL) {
                // should never happen
                // unless database error
                printf("The database has encountered an error, no user found \n");

                break;

            }

            if (temp_entry ->is_client_id_set == true) {

                unsigned msg_size;

                if (temp_entry->is_session_id_set) {

                    //leaving all the sessions 

                    struct session_entry* temp_session_entry = NULL;
                    temp_session_entry=session_db_search_sid(temp_entry->session_ID[0], head_s);


                    bool iResult_soc = socket_db_leave_session(temp_entry, temp_session_entry->session_tag);

                    bool iResult_ses = session_db_leave_socket(curr_socket, temp_session_entry, head_s);

                    if (!iResult_soc || !iResult_soc) {
                        printf("Database error when leaving sessions during log out request\n");
                        break;

                    }
                }
                if (socket_db_rm_sid(curr_socket, head)) {
                    reply_packet.type = EXIT;
                    strcpy(reply_packet.source, "server");
                    strcpy(reply_packet.data, "Logout successful");
                    reply_packet.size = strlen(reply_packet.data);

                    char* msg = msg_generator(reply_packet, &msg_size);

#ifdef DEBUGEXIT
                    temp_entry = socket_db_search_sid(curr_socket, head);
                    if (temp_entry == NULL) {

                        printf("The entry is no longer in the database\n");


                    } else {

                        printf("Deletion failed, the entry is still in the database\n");
                        printf("The entry remained is ");
                        if (temp_entry ->is_client_id_set) {
                            printf("logged in and has a id of %s\n", temp_entry ->client_ID);

                        } else {
                            printf("not logged in\n");
                        }


                    }


#endif


                    send(curr_socket, msg, msg_size, 0);
                    free(msg);
                    FD_CLR(curr_socket, &all_user);
                    close(curr_socket);

                    continue;
                } else {

                    printf("Database error, such user does not exist\n");
                    break;

                }
            } else {

                unsigned msg_size;

                socket_db_rm_sid(curr_socket, head);


#ifdef DEBUGEXIT
                
                temp_entry = socket_db_search_sid(curr_socket, head);
                if (temp_entry == NULL) {

                    printf("The entry is no longer in the database\n");


                } else {

                    printf("Deletion failed, the entry is still in the database\n");
                    printf("The entry remained is ");
                    if (temp_entry ->is_client_id_set) {
                        printf("logged in and has a id of %s\n", temp_entry ->client_ID);

                    } else {
                        printf("not logged in\n");
                    }


                }


#endif


                reply_packet.type = EXIT;
                strcpy(reply_packet.source, "server");
                strcpy(reply_packet.data, "The user is not logged in");
                reply_packet.size = strlen(reply_packet.data);
                char* msg = msg_generator(reply_packet, &msg_size);
                send(curr_socket, msg, msg_size, 0);
                free(msg);


                FD_CLR(curr_socket, &all_user);
                close(curr_socket);

                continue;





            }



        }

        if (new_packet.type == NEW_SESS) {

            temp_entry = NULL;

            temp_entry = socket_db_search_sid(curr_socket, head);
            if (temp_entry->is_client_id_set) {
                if (strlen(new_packet.data) > MAX_NAME) {
                    // should send rejection

                    continue;

                }

                if (session_db_search_sname(new_packet.data, head_s) != NULL) {

                    //send rejection, name too long

                    continue;

                }

                if (temp_entry->is_session_id_set && temp_entry->session_ID[MAX_SESS - 1] != 0) {


                    //send rejection, joined too many sessions
                    continue;

                }

                // this will always be true since the we have already checked whether the user is full on session or not
                unsigned int new_session_tag = session_db_add(new_packet.data, curr_socket, head_s);

                if (new_session_tag == 0) {

                    //send rejection,session with same name exists

                    continue;

                }

                bool iResult = socket_db_join_session(temp_entry, new_session_tag);

                unsigned int msg_size;
                reply_packet.type = NS_ACK;
                strcpy(reply_packet.source, "server");
                strcpy(reply_packet.data, "The session is successfully created");
                reply_packet.size = strlen(reply_packet.data);

                char* msg = msg_generator(reply_packet, &msg_size);

                send(curr_socket, msg, msg_size, 0);
                free(msg);








            } else {

                // reject with user not logged in 
                continue;

            }
        }


        if (new_packet.type == QUERY) {



            char* client_str = socket_db_traverse(head);

#ifdef DEBUGQU
            printf("\nThe client string is %s\n\n", client_str);
#endif
            char* session_str = (char*)session_db_traverse(head_s);

#ifdef DEBUGQU
            printf("\nThe session string is %s\n\n", session_str);
#endif           
            strcat(client_str, session_str);

            unsigned int msg_size;

            reply_packet.type = QU_ACK;
            strcpy(reply_packet.source, "server");
            strcpy(reply_packet.data, client_str);
            reply_packet.size = strlen(reply_packet.data);
            char* msg = msg_generator(reply_packet,&msg_size);
            send(curr_socket, msg,msg_size, 0);
            free(msg);
            free(client_str);
            free(session_str);



        }


        if (new_packet.type == JOIN) {

            temp_entry = NULL;

            temp_entry = socket_db_search_sid(curr_socket, head);

            if (/*temp_entry->is_session_id_set == false* 
                 *this limits the user to join one session only
                 */true) {

                struct session_entry* curr_s = (struct session_entry*)session_db_search_sname(new_packet.data, head_s);

                if (curr_s == NULL) {

                    unsigned int msg_size;

                    reply_packet.type = JN_NAK;
                    strcpy(reply_packet.source, "server");
                    strcpy(reply_packet.data, "Session does not exist");
                    reply_packet.size = strlen(reply_packet.data);

                    char* msg = msg_generator(reply_packet, &msg_size);

                    send(curr_socket, msg, msg_size, 0);
                    free(msg);

                    continue;

                } else {

                    bool iResult_full = session_db_join_socket(curr_socket, curr_s);

                    temp_entry = NULL;

                    temp_entry = socket_db_search_sid(curr_socket, head);
                    
                    bool iResult_sfull = false;
                    
                    if(iResult_full){
                    
                        iResult_sfull = socket_db_join_session(temp_entry, curr_s->session_tag);
                        if(!iResult_sfull){
                       
                        session_db_leave_socket(curr_socket,curr_s,head_s);
                        
                        }
                    
                    }
                    
                    
                    if (iResult_full && iResult_sfull) {

                        unsigned int msg_size;

                        reply_packet.type = JN_ACK;
                        strcpy(reply_packet.source, "server");
                        strcpy(reply_packet.data, "Join successful");
                        reply_packet.size = strlen(reply_packet.data);

                        char* msg = msg_generator(reply_packet, &msg_size);

                        send(curr_socket, msg, msg_size, 0);
                        free(msg);
                        continue;



                    } else {

                        unsigned int msg_size;
                        reply_packet.type = JN_NAK;
                        strcpy(reply_packet.source, "server");
                        strcpy(reply_packet.data, (iResult_sfull) ? "Session is full" : "you have joined too many sessions");
                        reply_packet.size = strlen(reply_packet.data);

                        char* msg = msg_generator(reply_packet, &msg_size);

                        send(curr_socket, msg, msg_size, 0);
                        free(msg);

                        continue;






                    }





                }
            } else {

                // old one session feature left over 
                unsigned int msg_size;

                reply_packet.type = JN_NAK;
                strcpy(reply_packet.source, "server");
                strcpy(reply_packet.data, "you have joined too many sessions");
                reply_packet.size = strlen(reply_packet.data);

                char* msg = msg_generator(reply_packet, &msg_size);

                send(curr_socket, msg, msg_size, 0);
                free(msg);

                continue;
            }
        }

        if (new_packet.type == MESSAGE) {

            temp_entry = NULL;

            temp_entry = socket_db_search_sid(curr_socket, head);

            if (temp_entry->is_session_id_set == false) {

                continue;


            } else {
                // potential feature for all session
                // find the temporary session entry, find all of the clients and get their sockets
                struct session_entry* temp_session_entry = (struct session_entry* )session_db_search_sid(temp_entry->session_ID[0], head_s);

                int counter = 0;
                for (; counter < MAX_MEM; counter++) {

                    // means end of session
                    if (temp_session_entry->socket_id[counter] == 0)
                        break;

                    // send the msg
                    unsigned int msg_size;

                    reply_packet.type = MESSAGE;
                    strcpy(reply_packet.source, "server");
                    strcpy(reply_packet.data, new_packet.source);
                    strcat(reply_packet.data, " : ");
                    strcat(reply_packet.data, new_packet.data);
                    reply_packet.size = strlen(reply_packet.data);

                    char* msg = msg_generator(reply_packet, &msg_size);

                    send(temp_session_entry->socket_id[counter], msg, msg_size, 0);
                    free(msg);

                    // we assume it is zeros thus have to be 0



                }



            }









        }

        if (new_packet.type == LEAVE_SESS) {


            // retrieve the user data from db
            // this is almost a standard for every functionality
            temp_entry = NULL;

            temp_entry = socket_db_search_sid(curr_socket, head);



            if (temp_entry->is_session_id_set == false) {
                // this indicates the user is not in any session

                unsigned int msg_size;
                reply_packet.type = LEAVE_SESS;
                strcpy(reply_packet.source, "server");
                strcpy(reply_packet.data, "You are not in any session");
                reply_packet.size = strlen(reply_packet.data);
                char* msg = msg_generator(reply_packet, &msg_size);
                send(curr_socket, msg, msg_size, 0);
                free(msg);


                continue;


            } else {

                // find the temporary session entry, find all of the clients and get their sockets
                // for now we only handle the case where user has one session
                // in the future we may add functionality like multiple session
                
                struct session_entry* temp_session_entry = NULL;
                
                unsigned int i = 0;
                
                bool iResult_soc = true; 
                bool iResult_ses = true;
                while(temp_entry->session_ID[i]!=0){
                
                     
                temp_session_entry = session_db_search_sid(temp_entry->session_ID[0], head_s);

                iResult_soc = socket_db_leave_session(temp_entry, temp_session_entry->session_tag);

                iResult_ses = session_db_leave_socket(curr_socket, temp_session_entry, head_s);
                
                
                }
                if (iResult_soc && iResult_ses) {

                    unsigned int msg_size;

                    reply_packet.type = LEAVE_SESS;
                    strcpy(reply_packet.source, "server");
                    strcpy(reply_packet.data, "Leave session successful");
                    reply_packet.size = strlen(reply_packet.data);

                    char* msg = msg_generator(reply_packet, &msg_size);

                    send(curr_socket, msg, msg_size, 0);
                    free(msg);


                } else {
                    unsigned int msg_size;

                    reply_packet.type = LEAVE_SESS;
                    strcpy(reply_packet.source, "server");
                    strcpy(reply_packet.data, "Leave session failed");
                    reply_packet.size = strlen(reply_packet.data);

                    char* msg = msg_generator(reply_packet, &msg_size);

                    send(curr_socket, msg, msg_size, 0);
                    free(msg);


                }




            }














        }





















    }

    // check all entry are properly freed
    session_db_deletion(head_s);
    socket_db_deletion(head);






    return 0;




















}
