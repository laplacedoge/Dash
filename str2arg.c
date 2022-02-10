#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "str2arg.h"

#define isPrintableChar(c)  (33 <= c && c <= 126)

#define isQuoteChar(c)      ((c)=='\''||(c)=='"')

#define isSpaceChar(c)      ((c)==' ')

#define isNormalChar(c)     (isPrintableChar(c) && ((c) != '\'' && (c) != '"' && (c) != ' '))

typedef enum {
    STAT_SPACE,
    STAT_ARG,
} stat_t;

typedef enum {
    ERR_OK = 0,
    ERR_COM = -16,
    ERR_ARG_OVERFLOW,
    ERR_INVALID_CHAR,
    ERR_BAD_SYNTAX,
} err_t;

int str2arg(const char * str, str2arg_cb_t cb) {
    #define ARG_MAX 16
    int stat = STAT_SPACE;
    bool isQuoting = false;
    char * head = (char *)str;
    char * tail = (char *)str + strlen(str);
    char * p = (char *)str;
    char * _argv[ARG_MAX] = {0};
    int _args[ARG_MAX] = {0};
    int _argi = 0;
    size_t offset = 0;
    int argc = 0;
    char ** argv = NULL;
    char * argb = NULL;
    int ret = ERR_OK;
    while(p != tail) {
        switch(stat) {
            case STAT_SPACE: {
                if(isNormalChar(*p)) {
                    stat = STAT_ARG;
                    _argv[_argi] = p;
                    _args[_argi] = 1;
                } else if(isQuoteChar(*p)) {
                    stat = STAT_ARG;
                    _argv[_argi] = p + 1;
                    _args[_argi] = 0;
                    isQuoting = true;
                } else if(isSpaceChar(*p)) {

                } else {
                    ret = ERR_INVALID_CHAR;
                    goto exit;
                }
            } break;
            case STAT_ARG: {
                if(isNormalChar(*p)) {
                    _args[_argi]++;
                } else if(isQuoteChar(*p)) {
                    stat = STAT_SPACE;
                    isQuoting = false;
                    if(++_argi == ARG_MAX + 1) {
                        ret = ERR_ARG_OVERFLOW;
                        goto exit;
                    }
                } else if(isSpaceChar(*p)) {
                    if(isQuoting) {
                        _args[_argi]++;
                    } else {
                        stat = STAT_SPACE;
                        if(++_argi == ARG_MAX + 1) {
                            ret = ERR_ARG_OVERFLOW;
                            goto exit;
                        }
                    }
                } else {
                    ret = ERR_INVALID_CHAR;
                    goto exit;
                }
            } break;
        }
        p++;
    }
    if(stat == STAT_ARG) {
        if(++_argi == ARG_MAX + 1) {
            ret = ERR_ARG_OVERFLOW;
            goto exit;
        }
    }
    if(isQuoting) {
        ret = ERR_BAD_SYNTAX;
        goto exit;
    }
    argc = _argi;
    argv = (char **)malloc(sizeof(char *) * argc);
    for(int i = 0; i < argc; i++) {
        offset += _args[i] + 1;
    }
    argb = (char *)malloc(offset);
    memset(argb, 0, offset);
    offset = 0;
    for(int i = 0; i < argc; i++) {
        argv[i] = argb + offset;
        memcpy(argb + offset, _argv[i], _args[i]);
        offset += _args[i] + 1;
    }
    exit:
    if(ret == ERR_OK) {
        cb(ret, argc, argv);
    } else {
        cb(ret, 0, NULL);
    }
    
    if(argv) {
        free(argv);
    }
    if(argb) {
        free(argb);
    }
    return ret;
}
