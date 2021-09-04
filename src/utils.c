/*
 * Copyright (C) 2019-2020 Niko Rosvall <niko@byteptr.com>
 */

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include "entry.h"
#include "utils.h"
#include "crypto.h"

#define COLOR_DEFAULT "\x1B[0m"

/* Function returns NULL if the environment variable
   YLVA_DEFAULT_USER is not set.
 */
char* get_default_username()
{
    char* username = getenv("YLVA_DEFAULT_USERNAME");
    return username;
}

static char *get_output_color()
{
    char *color = getenv("YLVA_COLOR");

    if(color == NULL)
        return COLOR_DEFAULT;

    if(strcmp(color, "BLUE") == 0)
        return "\x1B[34m";
    else if(strcmp(color, "RED") == 0)
        return "\x1B[31m";
    else if(strcmp(color, "GREEN") == 0)
        return "\x1B[32m";
    else if(strcmp(color, "YELLOW") == 0)
        return "\x1B[33m";
    else if(strcmp(color, "MAGENTA") == 0)
        return "\x1B[35m";
    else if(strcmp(color, "CYAN") == 0)
        return "\x1B[36m";
    else if(strcmp(color, "WHITE") == 0)
        return "\x1B[37m";
    else
        return COLOR_DEFAULT; /* Handle empty variable too */
}

bool print_entry(Entry_t *entry, int show_password)
{
    char *color = get_output_color();

    fprintf(stdout, "=====================================================================\n");

    /* Set the color */
    fprintf(stdout, "%s", color);

    fprintf(stdout, "ID: %d\n", entry->id);
    fprintf(stdout, "Title: %s\n", entry->title);
    fprintf(stdout, "User: %s\n", entry->user);
    fprintf(stdout, "Url: %s\n", entry->url);

    if(show_password == 1)
        fprintf(stdout, "Password: %s\n", entry->password);
    else
        fprintf(stdout, "Password: **********\n");

    fprintf(stdout, "Notes: %s\n", entry->notes);
    fprintf(stdout, "Modified: %s\n", entry->stamp);

    /* Reset the color */
    fprintf(stdout, "%s", COLOR_DEFAULT);

    fprintf(stdout, "=====================================================================\n");

    return 0;
}

bool file_exists(const char *path)
{
    struct stat buf;

    if(stat(path, &buf) != 0)
        return false;

    return true;
}

/* Function checks that we have a valid path
 * in our ylva.open_db file and if the database is not
 * encrypted.
 */
bool has_active_database()
{
    char *path = NULL;

    path = get_open_db_path_holder_filepath();

    if(!path)
        return false;

    struct stat buf;

    if(stat(path, &buf) != 0)
    {
        free(path);
        return false;
    }

    //If the database is encrypted, it's not active so return false
    if(is_file_encrypted(path))
    {
        free(path);
        return false;
    }

    free(path);

    return true;
}

/* Returns the path of ~/.ylva.open_db file.
 * Caller must free the return value */
char *get_open_db_path_holder_filepath()
{
    char *home = NULL;
    char *path = NULL;

    home = getenv("HOME");

    if(!home)
        return NULL;

    /* /home/user/.ylva.open_db */
    path = tmalloc(sizeof(char) * (strlen(home) + 15));

    strcpy(path, home);
    strcat(path, "/.ylva.open_db");

    return path;
}

/* Reads and returns the path of currently decrypted
 * database. Caller must free the return value */
char *read_active_database_path()
{
    FILE *fp = NULL;
    char *path = NULL;
    char *opendbholderpath = NULL;
    size_t len;

    path = get_open_db_path_holder_filepath();

    if(!path)
        return NULL;

    fp = fopen(path, "r");

    if(!fp)
    {
        free(path);
        return NULL;
    }

    /* We only need the first line from the file */

    if(getline(&opendbholderpath, &len, fp) < 0)
    {
        if(opendbholderpath)
            free(opendbholderpath);

        fclose(fp);
        free(path);

        return NULL;
    }

    fclose(fp);
    free(path);

    return opendbholderpath;
}

void write_active_database_path(const char *db_path)
{
    FILE *fp = NULL;
    char *path = NULL;

    path = get_open_db_path_holder_filepath();

    if(!path)
        return;

    fp = fopen(path, "w");

    if(!fp)
    {
        fprintf(stderr, "Error creating ~/.ylva.open_db file\n");
        free(path);
        return;
    }

    fprintf(fp, "%s", db_path);
    fclose(fp);

    free(path);
}

//Simple malloc wrapper to prevent enormous error
//checking every where in the code
void *tmalloc(size_t size)
{
    void *data = NULL;

    data = malloc(size);

    if(data == NULL)
    {
        fprintf(stderr, "Malloc failed. Abort.\n");
        abort();
    }

    return data;
}

void set_file_owner_rw(const char *path)
{
    if(chmod(path, S_IRUSR | S_IWUSR) != 0)
        fprintf(stderr, "WARNING: Error changing permissions of file %s\n", path);
}
