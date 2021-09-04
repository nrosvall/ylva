/*
 * Copyright (C) 2019-2021 Niko Rosvall <niko@byteptr.com>
 */


#ifndef __UTILS_H
#define __UTILS_H

#include <stdbool.h>
#include "entry.h"

#define COLOR_DEFAULT "\x1B[0m"

bool print_entry(Entry_t *entry, int show_password, int as_qrcode);
char *get_open_db_path_holder_filepath();
void write_active_database_path(const char *db_path);
char *read_active_database_path();
bool has_active_database();
void *tmalloc(size_t size);
void set_file_owner_rw(const char *path);
bool file_exists(const char *path);
char* get_default_username();

#endif
