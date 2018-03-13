#include<rpc/rpc.h>
#include"date.h"
main(int argc,char *argv[])
{
	CLIENT* cl;
	char** date_time;
	void* null;
	cl = clnt_create(argv[1], DATE_PROG, DATE_VERS, "udp");
	date_time = date_time_1(null, cl);
	printf("\nDate & Time: %s\n", *date_time);
}