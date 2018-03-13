#include<rpc/rpc.h>
#include"square.h"

int main(int argc, char *argv[])
{
	CLIENT *cl;
    int num; int* result;
    printf("Enter any integer: ");
    scanf("%d", &num);
	cl = clnt_create(argv[1], SQUARE_PROG, SQUARE_VERS, "udp");
	if(cl == NULL)
	{
	    printf("error: could not connect to server.\n");
        return 1;
	}
	result = square_1(&num, cl);
	printf("%d^2 = %d\n", num, *result);
    return 0;
}