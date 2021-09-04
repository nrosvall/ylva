/*
 * Copyright (C) 2019-2020 Niko Rosvall <niko@byteptr.com>
 */

#ifndef __ENTRY_H
#define __ENTRY_H

typedef struct _entry
{
    int id;
    char *title;
    char *user;
    char *url;
    char *password;
    char *notes;
    char *stamp;

    struct _entry *next;

} Entry_t;


Entry_t *entry_new(const char *title, const char *user, const char *url,
                   const char *password, const char *notes);

Entry_t* entry_new_empty();

Entry_t *entry_add(Entry_t *head, const char *title, const char *user,
                   const char *url, const char *password, const char *notes);
Entry_t *entry_dup(Entry_t *entry);
void entry_free(Entry_t *entry);

#endif
