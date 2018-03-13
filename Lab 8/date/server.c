#include<stdlib.h>
#include<time.h>
#include "date.h"

char* date_time;    // why should this be global?

char** date_time_1_svc(void* temp, struct svc_req* req)
{
    time_t seconds = time(NULL);
    date_time = ctime(&seconds);
    return &date_time;
}