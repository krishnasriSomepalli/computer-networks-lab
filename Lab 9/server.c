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
	int sfd1, sfd2, len, i, k, p, q, n, e, d, phi;
	int cipher[SIZE], plain[SIZE];
	char buffer[10];
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

	// accept p & q values
	printf("Enter two prime numbers:\n\tp: ");
	scanf("%d", &p);
	printf("\tq: ");
	scanf("%d", &q);
	n = p*q;
    phi = (p-1)*(q-1);

    // accept e
    printf("Enter a coprime of %d (1 < e < %d): ", phi, phi);
    scanf("%d", &e);

	// send n
    sprintf(buffer, "%d", n);
	if(send(sfd2, buffer, sizeof(buffer), 0) == -1)
	{
		perror("send() error");
		exit(6);
	}

    // send e
    sprintf(buffer, "%d", e);
	if(send(sfd2, buffer, sizeof(buffer), 0) == -1)
	{
		perror("send() error");
		exit(7);
	}

	// receive encrypted message
	if(recv(sfd2, &cipher, SIZE, 0) == -1)
	{
		perror("recv() error");
		exit(8);
	}

	printf("\nReceived encrypted data: ");
	i = 0;
	while(cipher[i] != '\0')
		printf("%d ", cipher[i++]);
	printf("\n");

    // calculate for d
    d = 1;
    while((e*d)%phi != 1)
        d++;

	// decrypt the message
	i = 0;
    while(cipher[i] != '\0'){
        // (a*b*c)%n == (((a%n)*(b%n))*c)%n
        plain[i] = 1;
        for(k = 0; k < d; k++)
            plain[i] = (plain[i]*cipher[i])%n;
        plain[i] = plain[i]%n;
        i++;
    }
    plain[i] = cipher[i];

	printf("Decrypted data: ");
	i = 0;
	while(plain[i] != '\0')
		printf("%d ", plain[i++]);
	printf("\n");

    // print the decrypted message
     printf("Decoded message (ASCII): ");
    i = 0;
    while(cipher[i] != '\0')
    {
        printf("%c",plain[i]);
        i++;
    }
    printf("\n");

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