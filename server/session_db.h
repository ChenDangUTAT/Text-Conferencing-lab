/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   session_db.H
 * Author: dangche1
 *
 * Created on March 14, 2017, 10:20 PM
 */

#pragma once

#include "session_entry.h"

#include <stdio.h>

#include <stdbool.h>

#include <malloc.h>

#include <string.h>

unsigned int session_db_add(char* session_name,int socket_, struct session_entry *head);


void session_db_remove(unsigned int session_tag, struct session_entry *head);


struct session_entry* session_db_search_sid(unsigned int session_tag,struct session_entry* head);

struct session_entry* session_db_search_sname(char* name,struct session_entry* head);

char* session_db_traverse(struct session_entry* head);

bool session_db_join_socket(int socket_, struct session_entry * session);