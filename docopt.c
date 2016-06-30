#ifdef __cplusplus
#include <cstdio>
#include <cstdlib>
#include <cstring>
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#endif


typedef struct {
    /* commands */
    int LISCLient;
    /* options without arguments */
    int exact;
    int help;
    int locDeRef;
    int locReq;
    int verbose;
    int version;
    /* options with arguments */
    char *flow;
    char *ip;
    char *method;
    char *types;
    char *uri;
    char *url;
    /* special */
    const char *usage_pattern;
    const char *help_message;
} DocoptArgs;

const char help_message[] =
"Nemergent LIS Client.\n"
"\n"
"Interface with any LIS client implemented according to the following RFC's:\n"
"\n"
"RFC 5985 - HTTP-Enabled Location Delivery (HELD)\n"
"RFC 6155 - Use of Device Identity in HTTP-Enabled Location Delivery (HELD)\n"
"RFC 6915 - Flow Identity Extension for HTTP-Enabled Location Delivery (HELD)\n"
"\n"
"Usage:\n"
"    LISClient --locReq --url=<HELDURL> [--types=<locTypes> --exact --verbose]\n"
"    LISClient --locReq --url=<HELDURL> [--types=<locTypes> --exact --verbose] [--ip=<DEVICEIP> | --uri=<SIPURI> | --flow=<FLOW>]\n"
"    LISClient --locDeRef --url=<REFURL> [--types=<locTypes> --method=<POST|GET> --verbose]\n"
"    LISCLient --help\n"
"    LISCLient --version\n"
"\n"
"Options:\n"
"    --locReq                        Perform a Location Request\n"
"    --url HELDURL                   Specifies a HELD URL of the LIS Server\n"
"    --types LOCTYPES                Specifies a series of Location Types to request, separated by commas.\n"
"    --exact                         Request exactly the specified location types.\n"
"    --ip IP_V4                      Perform a 3rd Party request, using the IP as the ID\n"
"    --uri sip:user@ip               Perform a 3rd Party request, using an URI as the ID (SIP, Tel...)\n"
"\n"
"    --flow OIP:OPRT-DIP:DPRT/L3/L4  Perform a 3rd Party request, using a flow (RFC 6915) as the ID\n"
"\n"
"    --locDeRef                      Perform a Location Dereference using a reference URL\n"
"    --method POST|GET               Specifies the HTTP method to perform the dereference request.\n"
"\n"
"    --verbose                       Makes the program output more debug info.\n"
"\n"
"\n"
"    --help                          Show this screen.\n"
"    --version                       Show version.\n"
"";

const char usage_pattern[] =
"Usage:\n"
"    LISClient --locReq --url=<HELDURL> [--types=<locTypes> --exact --verbose]\n"
"    LISClient --locReq --url=<HELDURL> [--types=<locTypes> --exact --verbose] [--ip=<DEVICEIP> | --uri=<SIPURI> | --flow=<FLOW>]\n"
"    LISClient --locDeRef --url=<REFURL> [--types=<locTypes> --method=<POST|GET> --verbose]\n"
"    LISCLient --help\n"
"    LISCLient --version";

typedef struct {
    const char *name;
    bool value;
} Command;

typedef struct {
    const char *name;
    char *value;
    char **array;
} Argument;

typedef struct {
    const char *oshort;
    const char *olong;
    bool argcount;
    bool value;
    char *argument;
} Option;

typedef struct {
    int n_commands;
    int n_arguments;
    int n_options;
    Command *commands;
    Argument *arguments;
    Option *options;
} Elements;


/*
 * Tokens object
 */

typedef struct Tokens {
    int argc;
    char **argv;
    int i;
    char *current;
} Tokens;

Tokens tokens_new(int argc, char **argv) {
    Tokens ts = {argc, argv, 0, argv[0]};
    return ts;
}

Tokens* tokens_move(Tokens *ts) {
    if (ts->i < ts->argc) {
        ts->current = ts->argv[++ts->i];
    }
    if (ts->i == ts->argc) {
        ts->current = NULL;
    }
    return ts;
}


/*
 * ARGV parsing functions
 */

int parse_doubledash(Tokens *ts, Elements *elements) {
    //int n_commands = elements->n_commands;
    //int n_arguments = elements->n_arguments;
    //Command *commands = elements->commands;
    //Argument *arguments = elements->arguments;

    // not implemented yet
    // return parsed + [Argument(None, v) for v in tokens]
    return 0;
}

int parse_long(Tokens *ts, Elements *elements) {
    int i;
    int len_prefix;
    int n_options = elements->n_options;
    char *eq = strchr(ts->current, '=');
    Option *option;
    Option *options = elements->options;

    len_prefix = (eq-(ts->current))/sizeof(char);
    for (i=0; i < n_options; i++) {
        option = &options[i];
        if (!strncmp(ts->current, option->olong, len_prefix))
            break;
    }
    if (i == n_options) {
        // TODO '%s is not a unique prefix
        fprintf(stderr, "%s is not recognized\n", ts->current);
        return 1;
    }
    tokens_move(ts);
    if (option->argcount) {
        if (eq == NULL) {
            if (ts->current == NULL) {
                fprintf(stderr, "%s requires argument\n", option->olong);
                return 1;
            }
            option->argument = ts->current;
            tokens_move(ts);
        } else {
            option->argument = eq + 1;
        }
    } else {
        if (eq != NULL) {
            fprintf(stderr, "%s must not have an argument\n", option->olong);
            return 1;
        }
        option->value = true;
    }
    return 0;
}

int parse_shorts(Tokens *ts, Elements *elements) {
    char *raw;
    int i;
    int n_options = elements->n_options;
    Option *option;
    Option *options = elements->options;

    raw = &ts->current[1];
    tokens_move(ts);
    while (raw[0] != '\0') {
        for (i=0; i < n_options; i++) {
            option = &options[i];
            if (option->oshort != NULL && option->oshort[1] == raw[0])
                break;
        }
        if (i == n_options) {
            // TODO -%s is specified ambiguously %d times
            fprintf(stderr, "-%c is not recognized\n", raw[0]);
            return 1;
        }
        raw++;
        if (!option->argcount) {
            option->value = true;
        } else {
            if (raw[0] == '\0') {
                if (ts->current == NULL) {
                    fprintf(stderr, "%s requires argument\n", option->oshort);
                    return 1;
                }
                raw = ts->current;
                tokens_move(ts);
            }
            option->argument = raw;
            break;
        }
    }
    return 0;
}

int parse_argcmd(Tokens *ts, Elements *elements) {
    int i;
    int n_commands = elements->n_commands;
    //int n_arguments = elements->n_arguments;
    Command *command;
    Command *commands = elements->commands;
    //Argument *arguments = elements->arguments;

    for (i=0; i < n_commands; i++) {
        command = &commands[i];
        if (!strcmp(command->name, ts->current)){
            command->value = true;
            tokens_move(ts);
            return 0;
        }
    }
    // not implemented yet, just skip for now
    // parsed.append(Argument(None, tokens.move()))
    /*fprintf(stderr, "! argument '%s' has been ignored\n", ts->current);
    fprintf(stderr, "  '");
    for (i=0; i<ts->argc ; i++)
        fprintf(stderr, "%s ", ts->argv[i]);
    fprintf(stderr, "'\n");*/
    tokens_move(ts);
    return 0;
}

int parse_args(Tokens *ts, Elements *elements) {
    int ret;

    while (ts->current != NULL) {
        if (strcmp(ts->current, "--") == 0) {
            ret = parse_doubledash(ts, elements);
            if (!ret) break;
        } else if (ts->current[0] == '-' && ts->current[1] == '-') {
            ret = parse_long(ts, elements);
        } else if (ts->current[0] == '-' && ts->current[1] != '\0') {
            ret = parse_shorts(ts, elements);
        } else
            ret = parse_argcmd(ts, elements);
        if (ret) return ret;
    }
    return 0;
}

int elems_to_args(Elements *elements, DocoptArgs *args, bool help,
                  const char *version){
    Command *command;
    Argument *argument;
    Option *option;
    int i;

    /* options */
    for (i=0; i < elements->n_options; i++) {
        option = &elements->options[i];
        if (help && option->value && !strcmp(option->olong, "--help")) {
            printf("%s", args->help_message);
            return 1;
        } else if (version && option->value &&
                   !strcmp(option->olong, "--version")) {
            printf("%s\n", version);
            return 1;
        } else if (!strcmp(option->olong, "--exact")) {
            args->exact = option->value;
        } else if (!strcmp(option->olong, "--help")) {
            args->help = option->value;
        } else if (!strcmp(option->olong, "--locDeRef")) {
            args->locDeRef = option->value;
        } else if (!strcmp(option->olong, "--locReq")) {
            args->locReq = option->value;
        } else if (!strcmp(option->olong, "--verbose")) {
            args->verbose = option->value;
        } else if (!strcmp(option->olong, "--version")) {
            args->version = option->value;
        } else if (!strcmp(option->olong, "--flow")) {
            if (option->argument)
                args->flow = option->argument;
        } else if (!strcmp(option->olong, "--ip")) {
            if (option->argument)
                args->ip = option->argument;
        } else if (!strcmp(option->olong, "--method")) {
            if (option->argument)
                args->method = option->argument;
        } else if (!strcmp(option->olong, "--types")) {
            if (option->argument)
                args->types = option->argument;
        } else if (!strcmp(option->olong, "--uri")) {
            if (option->argument)
                args->uri = option->argument;
        } else if (!strcmp(option->olong, "--url")) {
            if (option->argument)
                args->url = option->argument;
        }
    }
    /* commands */
    for (i=0; i < elements->n_commands; i++) {
        command = &elements->commands[i];
        if (!strcmp(command->name, "LISCLient")) {
            args->LISCLient = command->value;
        }
    }
    /* arguments */
    for (i=0; i < elements->n_arguments; i++) {
        argument = &elements->arguments[i];
    }
    return 0;
}


/*
 * Main docopt function
 */

DocoptArgs docopt(int argc, char *argv[], bool help, const char *version) {
    DocoptArgs args = {
        0, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL,
        usage_pattern, help_message
    };
    Tokens ts;
    Command commands[] = {
        {"LISCLient", 0}
    };
    Argument arguments[] = {
    };
    Option options[] = {
        {NULL, "--exact", 0, 0, NULL},
        {NULL, "--help", 0, 0, NULL},
        {NULL, "--locDeRef", 0, 0, NULL},
        {NULL, "--locReq", 0, 0, NULL},
        {NULL, "--verbose", 0, 0, NULL},
        {NULL, "--version", 0, 0, NULL},
        {NULL, "--flow", 1, 0, NULL},
        {NULL, "--ip", 1, 0, NULL},
        {NULL, "--method", 1, 0, NULL},
        {NULL, "--types", 1, 0, NULL},
        {NULL, "--uri", 1, 0, NULL},
        {NULL, "--url", 1, 0, NULL}
    };
    Elements elements = {1, 0, 12, commands, arguments, options};

    ts = tokens_new(argc, argv);
    if (parse_args(&ts, &elements))
        exit(EXIT_FAILURE);
    if (elems_to_args(&elements, &args, help, version))
        exit(EXIT_SUCCESS);
    return args;
}
