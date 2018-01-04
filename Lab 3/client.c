#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 100
int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr, caddr;
	int sfd, len;
	char msg[SIZE];
	if(argc != 3)
	{
		printf("Usage: ./client <server IPv4> <port number>\n");
		exit(1);
	}
	sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(sfd == -1)
	{
		perror("socket() error");
		exit(2);
	}
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	memset(servaddr.sin_zero, '\0', sizeof(servaddr.sin_zero));
	// Why is bind not required?
	/*caddr.sin_family = AF_INET;
	caddr.sin_port = 0;
	caddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memset(caddr.sin_zero, '\0', sizeof(servaddr.sin_zero));
	if(bind(sfd, (struct sockaddr*)&caddr, sizeof(caddr)))
	{
		perror("bind() error");
		exit(2);
	}*/
	len = sizeof(caddr);
	while(1)
	{
		printf("Client: ");
		fgets(msg, SIZE, stdin);
		if(sendto(sfd, &msg, SIZE, 0, (struct sockaddr*)&servaddr, len) == -1)
		{
			perror("sendto() error");
			exit(3);
		}
		// try using toLower to make it case insensitive
		if(strcmp(msg, "bye\n") == 0 || strcmp(msg, "exit\n") == 0)
			break;
		if(recvfrom(sfd, &msg, SIZE, 0, (struct sockaddr*)&servaddr, &len) == -1)
		{
			perror("recvfrom() error");
			exit(4);
		}
		printf("Server: %s", msg);
		if(strcmp(msg, "bye\n") == 0 || strcmp(msg, "exit\n") == 0)
			break;
	}
	if(close(sfd) == -1)
	{
		perror("close() error");
		exit(7);
	}
	return 0;
}
