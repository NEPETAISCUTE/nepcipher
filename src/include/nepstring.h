#ifndef NEPSTRING_H
#define NEPSTRING_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pointerStruct {
    void* ptr;
    int len;
} pointerlen;

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef char* string;

int strcmp(string str1, const char* str2);

int strlen(const char* str);

int strtoint(const char* str);

int strcpy(char * target, const char* source, int length);

int strsplit(const char* str, char sep, pointerlen* returnptrptr);

#ifdef __cplusplus
}
#endif

#endif /*NEPSTRING_H*/