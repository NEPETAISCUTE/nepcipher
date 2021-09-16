
#ifndef NEPCIPHER_H
#define NEPCIPHER_H

#include <stdio.h>
#include <stdlib.h>
#include <nepstring.h>

enum operationType {
    OTYPE_UNDEFINED,
    OTYPE_CIPHER,
    OTYPE_DECIPHER
};

typedef struct allocated_fs {
    FILE* ifile;
    FILE* ofile;
    char* buffer;
    char* keybuffer;
}fileAllocPointers;

typedef struct keyfile {
    string key;
    int keyfile;
}keyfiledat;

#define DEC_FALSE 1
#define DEC_TRUE -1

int operation(string ifname, string ofname, keyfiledat* key, int* offset, int offsetsize, operationType* otypeptr, int stdinput, int stdoutput, fileAllocPointers* returnvalueptr);
void cipher(string buf, string key, int fsize, int* offset, int offsetsize, int decipher);

#endif /*NEPCIPHER_H*/