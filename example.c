#include <stdio.h>

#include "str2arg.h"

const char TEST_ARG_STR_1[] = "dumphex  demo.txt  -l  16 -u";
const char TEST_ARG_STR_2[] = "  dumphex    -l  16 -u  ";
const char TEST_ARG_STR_3[] = "  dumphex  'demo data.txt'  -l  16 -u  ";
const char TEST_ARG_STR_4[] = "  dumphex  \"demo data.txt\"  -l  16 -u  ";

const char TEST_ARG_STR_5[] = "  dumphex  'demo data.txt' '  -l  16 -u  ";
const char TEST_ARG_STR_6[] = "  dum\rphex  'demo d\aata.txt' '  -l  16 -u  ";
const char TEST_ARG_STR_7[] = "  dumphex  'demo dat'a.txt'  -l  16 -u  ";

const char * TEST_ARG_STR_LST[] = {
    TEST_ARG_STR_1, TEST_ARG_STR_2, TEST_ARG_STR_3, TEST_ARG_STR_4,
    TEST_ARG_STR_5, TEST_ARG_STR_6, TEST_ARG_STR_7
};

int str2arg_cb(int err, int argc, char ** argv) {
    printf("ERR: %d\n", err);
    if(err == 0) {
        for(int i = 0; i < argc; i++) {
            printf("ARG %d: ^%s^\n", i, argv[i]);
        }
    }
    printf("\n");
}

int main(int argc, char **argv) {
    for(int i = 0; i < sizeof(TEST_ARG_STR_LST) / sizeof(const char *); i++) {
        str2arg(TEST_ARG_STR_LST[i], str2arg_cb);
    }
    return 0;
}