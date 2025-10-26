#ifndef DAISO_RESPONSE_H
#define DAISO_RESPONSE_H

#include "daiso_http.h" 

DaisoHttpResponse* daiso_response_parse(char *raw_response);

#endif
