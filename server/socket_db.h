/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   socket_db.h
 * Author: dangche1
 *
 * Created on March 12, 2017, 11:07 PM
 */

#pragma once

#include "socket_entry.h"

#include <stdio.h>

#include <stdbool.h>

#include <malloc.h>

struct socket_entry* socket_db_add(struct socket_entry newcomer,struct socket_entry* head);

struct socket_entry* socket_db_rm_sid(int socket_id,struct socket_entry* head);

struct socket_entry* socket_db_search_sid(int socket_id,struct socket_entry* head);

struct socket_entry* socket_db_search_cid(char* client_id,struct socket_entry* head);

char* socket_db_traverse(struct socket_entry* head);

bool socket_db_join_session(struct socket_entry* tar,unsigned int session_tag);

bool socket_db_leave_session(struct socket_entry* tar,unsigned int session_tag);





