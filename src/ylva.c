/*
 * Copyright (C) 2019-2020 Niko Rosvall <niko@byteptr.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <getopt.h>
#include "cmd_ui.h"
#include "entry.h"
#include "db.h"
#include "utils.h"
#include "pwd-gen.h"
#include "crypto.h"

static int show_password = 0;
static int force = 0;
static int auto_encrypt = 0;
static int show_as_qrcode = 0;

static double v = 1.7;

static void version()
{
    printf("Ylva version %.1f\n", v);
}

static void usage()
{
#define HELP "\
SYNOPSIS\n\
\n\
    ylva [flags] [options]\n\
\n\
OPTIONS\n\
\n\
    -i --init                <path>   Initialize new database\n\
    -E --encrypt                      Encrypt the current password database\n\
    -D --decrypt             <path>   Decrypt password database\n\
    -a --add                          Add new entry\n\
    -c --copy                <id>     Copy an entry\n\
    -r --remove              <id>     Remove entry pointed by id\n\
    -p --show-db-path                 Show current database path\n\
    -u --use-db              <path>   Switch using another database\n\
    -f --find                <search> Search entries\n\
    -F --regex               <search> Search entries with regular expressions\n\
    -e --edit                <id>     Edit entry pointed by id\n\
    -l --list-entry          <id>     List entry pointed by id\n\
    -t --show-latest         [count]  Show latest entries, count is optional\n\
    -A --list-all                     List all entries\n\
    -h --help                         Show short help and exit. This page\n\
    -g --gen-password        <length> Generate password\n\
    -q --quick               <search> This is the same as running\n\
                                      --show-passwords -f\n\
\n\
    -v --version                      Show version number of program\n\
\n\
FLAGS\n\
\n\
    --auto-encrypt                    Automatically encrypt after exit\n\
    --show-passwords                  Show passwords in listings\n\
    --show-qrcode                     Show password as QR code in --list-entry\n\
    --force                           Ignore everything and force operation\n\
                                      --force only works with --init option\n\
\n\
For more information and examples see man ylva(1).\n\
\n\
AUTHORS\n\
    Copyright (C) 2019-2020 Niko Rosvall <niko@byteptr.com>\n\
"
    printf(HELP);
}

int main(int argc, char *argv[])
{
    int c;

    if(argc == 1)
    {
        usage();
        return 0;
    }

    while(true)
    {
        static struct option long_options[] =
        {
            {"init",                  required_argument, 0,             'i'},
            {"encrypt",               no_argument,       0,             'E'},
            {"decrypt",               required_argument, 0,             'D'},
            {"use-db",                required_argument, 0,             'u'},
            {"add",                   no_argument,       0,             'a'},
            {"copy",                  required_argument, 0,             'c'},
            {"remove",                required_argument, 0,             'r'},
            {"find",                  required_argument, 0,             'f'},
            {"regex",                 required_argument, 0,             'F'},
            {"edit",                  required_argument, 0,             'e'},
            {"list-all",              no_argument,       0,             'A'},
            {"list-entry",            required_argument, 0,             'l'},
            {"gen-password",          required_argument, 0,             'g'},
            {"help",                  no_argument,       0,             'h'},
            {"version",               no_argument,       0,             'v'},
            {"show-db-path",          no_argument,       0,             'p'},
            {"show-latest",           no_argument,       0,             't'},
            {"quick",                 required_argument, 0,             'q'},
            {"auto-encrypt",          no_argument,       &auto_encrypt,  1 },
            {"show-passwords",        no_argument,       &show_password, 1 },
            {"show-qrcode",           no_argument,       &show_as_qrcode,   1 },
            {"force",                 no_argument,       &force,         1 },
            {0, 0, 0, 0}
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "i:ED:u:ac:r:f:F:e:Al:g:hvptq:",
                        long_options, &option_index);

        if(c == -1)
            break;

        switch(c)
        {
        case 0:
            /* Handle flags here automatically */
            break;
        case 'i':
            init_database(optarg, force, auto_encrypt);
            break;
        case 'E': //encrypt
            encrypt_database();
            break;
        case 'D': //decrypt
            decrypt_database(optarg);
            break;
        case 'u':
            set_use_db(optarg);
        case 'a':
            add_new_entry(auto_encrypt);
            break;
        case 'c':
            copy_entry(atoi(optarg));
            break;
        case 'r':
            remove_entry(atoi(optarg), auto_encrypt);
            break;
        case 'f':
            find(optarg, show_password, auto_encrypt);
            break;
        case 'F':
            find_regex(optarg, show_password);
            break;
        case 'e':
            edit_entry(atoi(optarg), auto_encrypt);
            break;
        case 'A':
            list_all(show_password, auto_encrypt, -1);
            break;
        case 'l':
            list_by_id(atoi(optarg), show_password, auto_encrypt, show_as_qrcode);
            break;
        case 'g':
        {
            char *pass = generate_password(atoi(optarg));
            if(pass != NULL)
                free(pass);
            break;
        }
        case 'h':
            usage();
            break;
        case 'v':
            version();
            break;
        case 'p':
            show_current_db_path();
            break;
        case 't':
        {
            int count = -2;
            if(argv[optind]) {
                count = atoi(argv[optind]);
            }
            show_latest_entries(show_password, auto_encrypt, count);
            break;
        }
        case 'q':
            show_password = 1;
            find(optarg, show_password, auto_encrypt);
            break;
        case '?':
            usage();
            break;
        }
    }
    return 0;
}
