#ifndef __STR2ARG_H__
#define __STR2ARG_H__


typedef int (* str2arg_cb_t)(int err, int argc, char ** argv);

int str2arg(const char * str, str2arg_cb_t cb);

#endif
