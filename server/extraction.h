/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   extraction.h
 * Author: dangche1
 *
 * Created on February 2, 2017, 6:35 PM
 */
#pragma once 



#include "packet.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// this function extracts the information needed for semi-ftp packets in our lab
// it returns nothing while fill in the data in packet*
bool extraction( char* ,struct packet* );
