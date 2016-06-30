/**
* @Author: Iñigo Ruiz Relloso
* @Date:   2016-02-21T12:46:43+01:00
* @Email:  iruiz077@ikasle.ehu.es
* @Last modified by:   Iñigo Ruiz Relloso
* @Last modified time: 2016-02-22T16:12:40+01:00
*/
#pragma once

#include <stdlib.h>

typedef struct {
    char* entity;
    char* civicAddress;
    char* geodetic;
    char* refURL;
    char* error_msg;
    char* error_code;
} held_response;

void free_held_response(held_response* res);
held_response* create_held_response();
