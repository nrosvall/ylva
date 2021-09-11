/*
 * Copyright (C) 2019-2021 Niko Rosvall <niko@byteptr.com>
 */


#ifndef __QR_H
#define __QR_H

#include "entry.h"

//fonts color
#define QR_FG_COLOR "\33[30m"
//background color
#define QR_BG_COLOR "\33[107m"

void print_entry_as_qr(Entry_t *entry);

#endif
