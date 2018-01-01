#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<stdlib.h>
#define SIZE 100
int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr, caddr;
	int sfd, len, i, n;
	size_t msgsize = 100;
	char rec[SIZE], msg[SIZE];
	if(argc != 4)
	{
		// 'n' indicates the number of messages
		printf("Usage: ./client <server IP> <port number> <n>\n");
		exit(1);
	}
	sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if(sfd == -1)
	{
		perror("socket() error");
		exit(2);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	if(connect(sfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("connect() error");
		exit(3);
	}
	n = atoi(argv[3]);
	if(send(sfd, argv[3], sizeof(argv[3]), 0) == -1)
	{
		perror("send() error");
		exit(4);
	}
	for(i = 0; i < n; i++)
	{
		printf(">> ");
		fgets(msg, SIZE, stdin);
		if(send(sfd, msg, SIZE, 0) == -1)
		{
			perror("send() error");
			exit(5);
		}
		if(read(sfd, rec, SIZE, 0) == -1)
		{
			perror("read() error");
			exit(6);
		}
		printf("--Received: %s", rec);
	}
	if(close(sfd) == -1)
	{
		perror("close() error");
		exit(7);
	}
	return 0;
}
