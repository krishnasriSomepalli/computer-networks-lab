#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/if_ether.h> // for ETH_P_ALL
#include <net/ethernet.h> // for struct ethhdr

#define SIZE 65536

void print_ethernet_header(unsigned char*, int);
void printData(unsigned char*, int);

int main()
{
	int raw, socklen, size, n = 10;
	unsigned char msg[SIZE];
	struct sockaddr saddr;
	raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // ETH_P_ALL will include all protocol
	printf("Started capturing...\n");
	if(raw == -1)
	{
		perror("socket() error");
		exit(1);
	}
	while(n > 0)
	{
		socklen = sizeof(saddr);
		if((size = recvfrom(raw, &msg, SIZE, 0, &saddr, &socklen)) == -1)
		{
			perror("recvfrom() error");
			exit(2);
		}
		print_ethernet_header(msg, size);
		n--;
	}
	if(close(raw) == -1)
	{
		perror("close() error");
		exit(3);
	}
	printf("\nStopped.\n");
	return 0;
}

void print_ethernet_header(unsigned char* msg, int size)
{
	struct ethhdr* eth = (struct ethhdr*)msg;
	printf("\n\tEthernet Header\n");
	printf("\t\tDestination Address: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]); // why %.2X? Won't %X suffice?
	printf("\t\tSource Address: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
	printf("\t\tProtocol: %u\n", eth->h_proto);
	printf("\t\tData: ");
	printData(msg+sizeof(struct ethhdr), size);
}

void printData(unsigned char* msg, int size)
{
	int i;
	printf("\t");
	for(i = 0; i < size; i++)
	{
		if(msg[i] >= 32 && msg[i] <= 126)
			printf("%c", msg[i]);
		else
			printf(".");
		if(i%16 == 0 && i!=0)
			printf("\n\t\t\t");
	}
}
