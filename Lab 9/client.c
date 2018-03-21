#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include<stdlib.h>
#define SIZE 100
int main(int argc, char* argv[])
{
	struct sockaddr_in servaddr, caddr;
	int sfd, len, i, n, k, e;
	char msg[SIZE], temp[10];
	int cipher[SIZE];
	size_t msgsize = 100;
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
	// accept message
	printf("Enter any message: ");
    scanf("%[^\n]s", msg);

	// receive n value
	if(recv(sfd, &temp, 10, 0) == -1)
	{
		perror("recv() error");
		exit(4);
	}
	n = atoi(temp);

	// receive e value
	if(recv(sfd, &temp, 10, 0) == -1)
	{
		perror("recv() error");
		exit(4);
	}
	e = atoi(temp);

	i = 0;

	// encrypt the message
	while(msg[i] != '\0'){
        cipher[i] = 1;
        for(k = 0; k < e; k++)
            cipher[i] = (cipher[i]*msg[i])%n;
        cipher[i] = cipher[i]%n;
        i++;
    }
    cipher[i] = msg[i];

	printf("Sending encrypted data: ");
	i = 0;
	while(cipher[i] != '\0')
		printf("%d ", cipher[i++]);
	printf("\n");

	// send the encrypted message
	if(send(sfd, cipher, SIZE, 0) == -1)
	{
		perror("send() error");
		exit(7);
	}

	if(close(sfd) == -1)
	{
		perror("close() error");
		exit(7);
	}
	return 0;
}