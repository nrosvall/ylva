/*
 * Copyright (C) 2019-2021 Niko Rosvall <niko@byteptr.com>
 */

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <qrcodegen/qrcodegen.h>
#include "qr.h"
#include "utils.h"

// Prints the given QR Code to the console.
static void printQr(const uint8_t qrcode[])
{
    printf(QR_FG_COLOR QR_BG_COLOR);

    int size = qrcodegen_getSize(qrcode);
    int border = 4;

    printf("\n");

    for (int y = -border; y < size + border; y++) {
        for (int x = -border; x < size + border; x++) {
            fputs((qrcodegen_getModule(qrcode, x, y) ? "\u2585\u2585" : "  "), stdout);
        }

        printf("\n");
    }

    printf(COLOR_DEFAULT);
    printf("\n");
}

void print_entry_as_qr(Entry_t *entry)
{
    enum qrcodegen_Ecc error_level = qrcodegen_Ecc_LOW;
    uint8_t qrcode[qrcodegen_BUFFER_LEN_MAX];
    uint8_t tmp_buffer[qrcodegen_BUFFER_LEN_MAX];

    char *data = NULL;
    size_t data_len = strlen(entry->title) + strlen(entry->user) +
                    strlen(entry->url) + strlen(entry->password) +
                    strlen(entry->notes);

    //Allocate space for the entry data plus five newline characters and for the \0
    data_len += 6;
    data = tmalloc((data_len) * sizeof(char));

    if( (snprintf(data, data_len, "%s\n%s\n%s\n%s\n%s\n", entry->title,
            entry->user, entry->url, entry->password, entry->notes) ) < 0 ) {

            fprintf(stderr, "Unable to combine entry data.\n");
            free(data);
            return;
    }

    bool ok = qrcodegen_encodeText(data, tmp_buffer, qrcode, error_level,
                                   qrcodegen_VERSION_MIN, qrcodegen_VERSION_MAX,
                                   qrcodegen_Mask_AUTO, true);

    if (ok)
        printQr(qrcode);
    else
        fprintf(stderr, "Unable to generate QR code.\n");

    free(data);
}

