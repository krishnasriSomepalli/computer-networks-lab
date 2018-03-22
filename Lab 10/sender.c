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
	int sfd1, sfd2, len, M, N, i, seqNo, errSeqNo, size, ACK;
	char msg[SIZE], buffer[10];

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

	// Prompt the user for the data to send and sender window size, N
	printf("Text to transmit: ");
	scanf("%[^\n]s", msg);

	size = 0;
	while(msg[size] != '\0')
		size++;

	// Until all data is successfully sent:
		// send 1 indicating to the receiver that there's still data to send
		// receive M (receiver's window size)
		// create M frames
		// send the frames to the receiver
		// receive ACK and if ACK is -1 (error),
			// receive seqNo
			// receive M
			// send M number of frames from the frame numbered seqNo
		// else modify seqNo
	// send 0 indicating to the receiver that there's no more data to send

	seqNo = -1;
	while(seqNo < size)
	{
		sprintf(buffer, "1");
		send(sfd2, buffer, 10, 0);
		recv(sfd2, &buffer, 10, 0);
		M = atoi(buffer);
		for(i = seqNo+1; i < M+seqNo+1; i++)
			buffer[i-(seqNo+1)] = msg[i];
		buffer[i] = '\0';
		send(sfd2, buffer, 10, 0);
		printf("Sending |%s|\n", buffer);
		recv(sfd2, &buffer, 10, 0);
		ACK = atoi(buffer);
		if(ACK == 0)
		{
			printf("Successfully sent\n");
			seqNo = seqNo+M;
		}
		else
		{
			recv(sfd2, &buffer, 10, 0);
			errSeqNo = atoi(buffer);
			printf("Error in transmission at frame %d\n", errSeqNo);
			seqNo = seqNo+errSeqNo-1;
		}
	}
	sprintf(buffer, "0");
	send(sfd2, buffer, 10, 0);

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