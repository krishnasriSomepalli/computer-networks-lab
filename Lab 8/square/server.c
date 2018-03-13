#include <stdlib.h>
#include "square.h"

int result;

int* square_1_svc(int* num, struct svc_req* req)
{
    result = (*num)*(*num);
    return &result;
}