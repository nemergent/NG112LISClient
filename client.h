/**
* @Author: Iñigo Ruiz Relloso
* @Date:   2016-02-20T17:44:56+01:00
* @Email:  iruiz077@ikasle.ehu.es
* @Last modified by:   Iñigo Ruiz Relloso
* @Last modified time: 2016-02-22T12:01:42+01:00
*/
#pragma once

#include "docopt.c"

#include "http.h"
#include "xml.h"
#include "held.h"

int verbose = 0;

void die(const char* msg, char* answ, char* req);
void print_answ (char* msg);
void print_req (char* req);
