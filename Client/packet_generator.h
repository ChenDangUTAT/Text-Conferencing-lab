/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   packet_generator.h
 * Author: dangche1
 *
 * Created on March 14, 2017, 6:00 PM
 */
#pragma once

#include "packet.h"
#include <string.h>
#include <malloc.h>




char* msg_generator(struct packet input,unsigned* size);



