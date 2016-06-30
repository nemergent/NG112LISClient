/**
* @Author: Iñigo Ruiz Relloso
* @Date:   2016-02-20T19:26:10+01:00
* @Email:  iruiz077@ikasle.ehu.es
* @Last modified by:   Iñigo Ruiz Relloso
* @Last modified time: 2016-06-08T21:27:58+02:00
*/

#include "client.h"

int main(int argc, char *argv[])
{
    DocoptArgs args = docopt(argc, argv, /* help */ 1, /* version */ "1.0");

    verbose = args.verbose;

    char* req = NULL,* answ = NULL;

    if (args.locDeRef)
    {
        puts("[+] Location DeReference running...");

        if (args.url == NULL)
            die("URL not supplied", NULL, NULL);

        if (args.method == NULL || (args.method != NULL && strcmp(args.method, "GET") == 0))
        {
            if (args.types != NULL)
                puts("[i] Location types supplied, but method is GET (default). Use POST.");
            answ = http_get(args.url);
        }
        else if (strcmp(args.method, "POST") == 0)
        {
            req = xml_LocReq(
                NULL,
                NULL,
                NULL,
                args.types == NULL ? NULL : args.types,
                args.exact
                );

            if (req == NULL)
                die("Error generating the Location DeReference", NULL, NULL);

            answ = http_post(req, args.url);

            if (answ == NULL)
                die("Error posting the Location DeReference", NULL, req);
        }
        else
        {
            die("Method supplied not equal to GET or POST", NULL, NULL);
        }
    }
    else if (args.locReq)
    {
        puts("[+] Location Request running...");

        if (args.url == NULL)
            die("URL not supplied", NULL, NULL);

        req = xml_LocReq(
            args.ip == NULL ? NULL : args.ip,
            args.uri == NULL ? NULL : args.uri,
            args.flow == NULL ? NULL : args.flow,
            args.types == NULL ? NULL : args.types,
            args.exact
            );
        if (req == NULL)
            die("Error generating the Location Request", NULL, NULL);

        answ = http_post(req, args.url);

        if (answ == NULL)
            die("Error posting the Location Request", NULL, req);

    }else
    {
        printf("%s\n", args.usage_pattern);
        exit(-1);
    }

    held_response* held = xml_parse(answ);

    if (held == NULL)
    {
        free(held);
        die("Error parsing HELD response", answ, req);
    }


    if (held->error_code != NULL)
    {
        printf(
            "[!] Server returned error status\n"
            "Error code: \n\t%s\n"
            "Error message: \n\t%s\n",
            held->error_code, held->error_msg
        );
    }else
    {
        printf(
            "[+] Server returned the following location information:\n"
            "Presence Entity:\n"
            "----------------\n"
            "%s\n\n"
            "Civic Location:\n"
            "---------------\n"
            "%s\n\n"
            "Geodetic Location:\n"
            "------------------\n"
            "%s\n\n"
            "Reference URL's:\n"
            "----------------\n"
            "%s\n\n",
            held->entity ? held->entity : "(none)",
            held->civicAddress ? held->civicAddress : "(none)",
            held->geodetic ? held->geodetic : "(none)",
            held->refURL ? held->refURL : "(none)"
        );
    }

    if (verbose)
    {
        print_req(req);
        print_answ(answ);
    }


    free(answ);
    free_held_response(held);
    if (req) free(req);



    return 0;
}

void die(const char * msg, char* answ, char* req)
{
    printf("[!] %s\n", msg);
    if (req != NULL)
    {
        if (verbose)
            print_req(req);

        free(req);
    }
    if (answ != NULL)
    {
        if (verbose)
            print_answ(answ);

        free(answ);
    }
    exit(-1);
}

void print_answ (char* answ)
{
    printf(
        "\n\n"
        "=================================\n"
        "         SERVER RESPONSE\n"
        "=================================\n%s",
        answ
    );
}
void print_req (char* req)
{
    printf(
        "\n\n"
        "=================================\n"
        "         CLIENT REQUEST\n"
        "=================================\n%s",
        req
    );
}
