#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> // ??
#include <errno.h> // ??
#include <netdb.h> // ??
#include <netinet/if_ether.h> // for ETH_P_ALL
#include <net/ethernet.h> // for struct ethhdr
#include <netinet/ip.h> // for struct iphdr
#include <netinet/ip_icmp.h> // icmp header
#include <netinet/tcp.h> // tcp header
#include <netinet/udp.h> // udp header
#include <netinet/ip.h> // ip header
#include<arpa/inet.h> // ??
#include<sys/ioctl.h> // ??
#include<sys/time.h> // ??
#include<unistd.h> // ??

#define SIZE 65536

int total = 0, icmp = 0, igmp = 0, tcp = 0, udp = 0, others = 0;
void processPacket(unsigned char*, int);

int main()
{
	int raw, socklen, size, n = 100;
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
		processPacket(msg, size);
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

void processPacket(unsigned char* msg, int size)
{
	struct iphdr* iph = (struct iphdr*)(msg + sizeof(struct ethhdr)); // skipping over the Ethernet header to reach the IP header
	total++;
	switch(iph -> protocol)
	{
		case 1:			// ICMP
			icmp++;
			break;
		case 2:			// IGMP
			igmp++;
			break;
		case 6: 		// TCP
			tcp++;
			break;
		case 17:		// UDP
			udp++;
			break;
		default:
			others++;
			break;
	}
	printf("ICMP: %d | IGMP: %d | TCP: %d | UDP: %d | Others: %d | Total: %d\r", icmp, igmp, tcp, udp, others, total);
}
