#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<stdlib.h>
#define SIZE 100
int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr, caddr;
	int sfd, len, i, ACK, M = 4, err, errno;
	char msg[SIZE], buffer[10];
	if(argc != 3)
	{
		printf("Usage: ./client <server IP> <port number>\n");
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

	// send M (window size)
	// receive frames
	// ask user if they found any errors and ask for the frame no. of the first error
	// if yes
		// send ACK -1
		// send the frame no.
		// send M
		// receive frames
	while(1)
	{
		recv(sfd, buffer, 10, 0);
		if(atoi(buffer) == 0)
			break;
		sprintf(buffer, "%d", M);
		send(sfd, buffer, 10, 0);
		recv(sfd, &buffer, 10, 0);
		printf("Received |%s|\n", buffer);
		printf("Any errors in the transmission: ");
		scanf("%d", &err);
		if(err == 1)
		{
			ACK = -1;
			sprintf(buffer, "%d", ACK);
			send(sfd, buffer, 10, 0);
			printf("Enter the frame number where the first error has been found: ");
			scanf("%d", &errno);
			sprintf(buffer, "%d", errno);
			send(sfd, buffer, 10, 0);
		}
		else
		{
			ACK = 0;
			sprintf(buffer, "%d", ACK);
			send(sfd, buffer, 10, 0);
		}
	}

	if(close(sfd) == -1)
	{
		perror("close() error");
		exit(7);
	}
	return 0;
}