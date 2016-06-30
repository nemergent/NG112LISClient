/**
* @Author: Iñigo Ruiz Relloso
* @Date:   2016-02-21T19:18:21+01:00
* @Email:  iruiz077@ikasle.ehu.es
* @Last modified by:   Iñigo Ruiz Relloso
* @Last modified time: 2016-02-24T23:51:43+01:00
*/

#include "held.h"

void free_held_response(held_response* res)
{
    if (res != NULL)
    {
        if (res->entity != NULL) free(res->entity);
        if (res->civicAddress != NULL) free(res->civicAddress);
        if (res->geodetic != NULL) free(res->geodetic);
        if (res->refURL != NULL) free(res->refURL);
        if (res->error_msg != NULL) free(res->error_msg);
        if (res->error_code != NULL) free(res->error_code);
        free(res);
    }

}

held_response* create_held_response()
{
    held_response* response = (held_response*) calloc(1,sizeof(held_response));
    return response;
}
