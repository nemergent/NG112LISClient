/**
* @Author: Iñigo Ruiz Relloso
* @Date:   2016-02-20T21:11:50+01:00
* @Email:  iruiz077@ikasle.ehu.es
* @Last modified by:   Iñigo Ruiz Relloso
* @Last modified time: 2016-02-25T01:46:19+01:00
*/

#include "http.h"

char* http_post(char* body, char* url)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *list = NULL;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        list = curl_slist_append(list, "Expect:");
        list = curl_slist_append(list, "Content-Type: application/held+xml");
        list = curl_slist_append(list, "Accept:");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }else
        {
            return chunk.memory;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return NULL;
}

char* http_get(char* url)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *list = NULL;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);  /* will be grown as needed by the realloc */
    chunk.size = 0;    /* no data at this point */

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    if(curl) {
        list = curl_slist_append(list, "Expect:");

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);


        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        }else
        {
            return chunk.memory;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();

    return NULL;
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if(mem->memory == NULL) {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}
