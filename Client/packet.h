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
#define MAX_NAME 100
#define MAX_DATA 1024


struct packet {
    
unsigned int type;

unsigned int size;

unsigned char source[MAX_NAME];

unsigned char data[MAX_DATA];

};

