//Server
#include<stdio.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>
#define SIZE 100
int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr, caddr;
	int sfd1, sfd2, len, i, n;
	char msg[SIZE], no[10];
	if(argc != 2)
	{
		printf("Usage: ./server <port number>\n");
		exit(1);
	}
	sfd1 = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(sfd1 == -1)
	{

		perror("socket() error");
		exit(2);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(sfd1, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind() error");
		exit(3);
	}
	if(listen(sfd1, 5) == -1)
	{
		perror("listen() error");
		exit(4);
	}
	printf("Listening...\n");
	sfd2 = accept(sfd1, (struct sockaddr*)&caddr, &len);
	if(sfd2 == -1)
	{
		perror("accept() error");
		exit(5);
	}
	if(recv(sfd2, &no, 10, 0) == -1)
	{
		perror("read() error");
		exit(6);
	}
	n = atoi(no);
	for(i = 0; i < n; i++)
	{
		if(recv(sfd2,&msg,100,0) == -1)
		{
			perror("read() error");
			exit(7);
		}
		printf("--Received: %s\n", msg);
		if(send(sfd2,&msg,100,0) == -1)
		{
			perror("write() error");
			exit(8);
		}
	}
	if(close(sfd2) == -1)
	{
		perror("close() error");
		exit(9);
	}
	if(close(sfd1) == -1)
	{
		perror("close() error");
		exit(10);
	}
	printf("Stopped.\n");
	return 0;
}
