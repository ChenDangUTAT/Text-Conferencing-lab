/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   packet.h
 * Author: dangche1
 *
 * Created on February 2, 2017, 5:47 PM
 */

#pragma once
#include <netinet/in.h>
struct packet {
    
unsigned int type;

unsigned int size;

unsigned char source[MAX_NAME];

unsigned char data[MAX_DATA];

};

