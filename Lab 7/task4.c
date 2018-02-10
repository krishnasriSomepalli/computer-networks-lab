#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <netinet/if_ether.h> // for ETH_P_ALL
#include <net/ethernet.h> // for struct ethhdr
#include <netinet/ip.h> // for struct iphdr
#include <netinet/udp.h> // udp header
#include <netinet/ip.h> // ip header
/*#include<arpa/inet.h> // ??
#include<sys/ioctl.h> // ??
#include<sys/time.h> // ??
#include<unistd.h> // ??*/

#define SIZE 65536

int n, total;
void processPacket(unsigned char*, int);
void print_udp_packet(unsigned char*, int);
void printData(unsigned char*, int);

int main()
{
	int raw, socklen, size;
	unsigned char msg[SIZE];
	struct sockaddr saddr;
	raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // ETH_P_ALL will include all protocol
	printf("Number of UDP packets you want to capture: ");
	scanf("%d", &n);
	total = n;
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
		processPacket(msg, size);
	}
	if(close(raw) == -1)
	{
		perror("close() error");
		exit(3);
	}
	printf("\nStopped.\n");
	return 0;
}

void processPacket(unsigned char* msg, int size)
{
	struct iphdr* iph = (struct iphdr*)(msg + sizeof(struct ethhdr)); // skipping over the Ethernet header to reach the IP header
	if(iph -> protocol == 17)
	{
		print_udp_packet(msg, size);
		n--;
	}
}


void print_udp_packet(unsigned char* msg, int size)
{
	unsigned short iphdr_len;
	struct iphdr* iph;
	struct udphdr* udph;

	iph = (struct iphdr*)(msg + sizeof(struct ethhdr));
	iphdr_len = iph -> ihl*4; // ihl gives the number of rows in the IP header, each row being 4 bytes
	udph = (struct udphdr*)(msg + sizeof(struct ethhdr) + iphdr_len);
	printf("\n\nUDP Packet %d\n", total - n + 1);
	printf("\n\tUDP Header\n");
	printf("\t\tSource Port: %u\n", udph->source);
	printf("\t\tDestination Port: %u\n", udph->dest);
	printf("\t\tUDP Length: %u\n", udph->len);
	printf("\t\tUDP Checksum: %u\n", udph->check);
	printf("\n\tData Payload\n");
	printData(msg+sizeof(struct ethhdr)+iphdr_len+sizeof(udph), size);
}


void printData(unsigned char* msg, int size)
{
	int i;
	printf("\t\t");
	for(i = 0; i < size; i++)
	{
		if(msg[i] >= 32 && msg[i] <= 126)
			printf("%c", msg[i]);
		else
			printf(".");
		if(i%16 == 0 && i!=0)
			printf("\n\t\t");
	}
}
