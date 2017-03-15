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

struct socket_entry* socket_db_add(struct socket_entry newcomer,struct socket_entry* entry_array);

struct socket_entry* socket_db_rm_sid(int socket_id);


