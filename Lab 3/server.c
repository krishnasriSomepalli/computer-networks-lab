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
	int sfd1, len;
	char msg[SIZE];
	if(argc != 3)
	{
		printf("Usage: ./server <server IPv4> <port number>\n");
		exit(1);
	}
	sfd1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(sfd1 == -1)
	{

		perror("socket() error");
		exit(2);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]); // does this work or should one server IP be given?
	if(bind(sfd1, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		perror("bind() error");
		exit(3);
	}
	printf("Server has been started...\n");
	len = sizeof(servaddr);
	while(1)
	{
		if(recvfrom(sfd1, &msg, SIZE, 0, (struct sockaddr*)&caddr, &len) == -1)
		{
			perror("recvfrom() error");
			exit(4);
		}
		printf("Client: %s", msg);
		if(strcmp(msg, "bye\n") == 0 || strcmp(msg, "exit\n") == 0)
			break;
		printf("Server: ");
		fgets(msg, SIZE, stdin);
		// try using toLower to make it case insensitive
		if(sendto(sfd1, &msg, SIZE, 0, (struct sockaddr*)&caddr, len) == -1)
		{
			perror("recvfrom() error");
			exit(5);
		}
		if(strcmp(msg, "bye\n") == 0 || strcmp(msg, "exit\n") == 0)
			break;
	}
	if(close(sfd1) == -1)
	{
		perror("close() error");
		exit(10);
	}
	printf("Stopped.\n");
	return 0;
}

