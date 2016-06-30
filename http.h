/**
* @Author: Iñigo Ruiz Relloso
* @Date:   2016-02-20T21:10:36+01:00
* @Email:  iruiz077@ikasle.ehu.es
* @Last modified by:   Iñigo Ruiz Relloso
* @Last modified time: 2016-02-25T01:46:49+01:00
*/
#pragma once

#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

struct MemoryStruct {
    char *memory;
    size_t size;
};

char* http_post(char* body, char* url);
char* http_get(char* url);
size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
