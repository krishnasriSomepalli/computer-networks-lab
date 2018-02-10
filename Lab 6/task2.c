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
FILE* fp;

void processPacket(unsigned char*, int);
void print_icmp_packet(unsigned char*, int);
void print_tcp_packet(unsigned char*, int);
void print_udp_packet(unsigned char*, int);
void print_ip_header(unsigned char*);
void print_ethernet_header(unsigned char*);
void printData(unsigned char*, int);

int main()
{
	int raw, socklen, size;
	unsigned char msg[SIZE];
	struct sockaddr saddr;
	fp = fopen("log.txt", "w");
	if(fp == NULL)
	{
		perror("fopen() error");
		exit(0);
	}
	raw = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); // ETH_P_ALL will include all protocol
	printf("Started capturing...\n");
	if(raw == -1)
	{
		perror("socket() error");
		exit(1);
	}
	while(1)
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
	printf("Stopped.\n");
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
			print_icmp_packet(msg, size);
			break;
		case 2:			// IGMP
			igmp++;
			break;
		case 6: 		// TCP
			tcp++;
			print_tcp_packet(msg, size);
			break;
		case 17:		// UDP
			udp++;
			print_udp_packet(msg, size);
			break;
		default:
			others++;
			break;
	}
	printf("ICMP: %d | IGMP: %d | TCP: %d | UDP: %d | Others: %d | Total: %d\r", icmp, igmp, tcp, udp, others, total);
}


void print_icmp_packet(unsigned char* msg, int size)
{
	unsigned short iphdr_len;
	struct iphdr* iph;
	struct icmphdr* icmph;

	iph = (struct iphdr*)(msg + sizeof(struct ethhdr));
	iphdr_len = iph -> ihl*4; // ihl gives the number of rows in the IP header, each row being 4 bytes
	icmph = (struct icmphdr*)(msg + sizeof(struct ethhdr) + iphdr_len);
	fprintf(fp, "\n\nICMP Packet\n");
	print_ip_header(msg); // prints Ethernet header and the the IP header
	fprintf(fp, "\n\tICMP Header\n");
	fprintf(fp, "\t\tType: %u\n", icmph->type);
	fprintf(fp, "\t\tCode: %u\n", icmph->code);
	fprintf(fp, "\t\tChecksum: %u\n", icmph->checksum);
	fprintf(fp, "\n\tData Payload\n");
	printData(msg+sizeof(struct ethhdr)+iphdr_len+sizeof(icmph), size);
}

void print_tcp_packet(unsigned char* msg, int size)
{
	unsigned short iphdr_len;
	struct iphdr* iph;
	struct tcphdr* tcph;

	iph = (struct iphdr*)(msg + sizeof(struct ethhdr));
	iphdr_len = iph -> ihl*4; // ihl gives the number of rows in the IP header, each row being 4 bytes
	tcph = (struct tcphdr*)(msg + sizeof(struct ethhdr) + iphdr_len);
	fprintf(fp, "\n\nTCP Packet\n");
	print_ip_header(msg); // prints Ethernet header and the the IP header
	fprintf(fp, "\n\tTCP Header\n");
	fprintf(fp, "\t\tSource Port: %u\n", tcph->source);
	fprintf(fp, "\t\tDestination Port: %u\n", tcph->dest);
	fprintf(fp, "\t\tSequence Number: %u\n", tcph->seq);
	fprintf(fp, "\t\tAcknowledge Number: %u\n", tcph->ack_seq);
	fprintf(fp, "\t\tHeader Length: %u dwords (%u bytes)\n", tcph->doff, tcph->doff*4);
	// fprintf(fp, "\tCWR Flag: %u\n", tcph->cwr);
	// fprintf(fp, "\tECE Flag: %u\n", tcph->ece);
	fprintf(fp, "\t\tUrgent Flag: %u\n", tcph->urg);
	fprintf(fp, "\t\tAcknowledgement Flag: %u\n", tcph->ack);
	fprintf(fp, "\t\tPush Flag: %u\n", tcph->psh);
	fprintf(fp, "\t\tReset Flag: %u\n", tcph->rst);
	fprintf(fp, "\t\tSynchronise Flag: %u\n", tcph->syn);
	fprintf(fp, "\t\tFinish Flag: %u\n", tcph->fin);
	fprintf(fp, "\t\tWindow: %u\n", tcph->window);
	fprintf(fp, "\t\tChecksum: %u\n", tcph->check);
	fprintf(fp, "\t\tUrgent Pointer: %u\n", tcph->urg_ptr);
	fprintf(fp, "\n\tData Payload\n");
	printData(msg+sizeof(struct ethhdr)+iphdr_len+sizeof(tcph), size);
}

void print_udp_packet(unsigned char* msg, int size)
{
	unsigned short iphdr_len;
	struct iphdr* iph;
	struct udphdr* udph;

	iph = (struct iphdr*)(msg + sizeof(struct ethhdr));
	iphdr_len = iph -> ihl*4; // ihl gives the number of rows in the IP header, each row being 4 bytes
	udph = (struct udphdr*)(msg + sizeof(struct ethhdr) + iphdr_len);
	fprintf(fp, "\n\nUDP Packet\n");
	print_ip_header(msg); // prints Ethernet header and the the IP header
	fprintf(fp, "\n\tUDP Header\n");
	fprintf(fp, "\t\tSource Port: %u\n", udph->source);
	fprintf(fp, "\t\tDestination Port: %u\n", udph->dest);
	fprintf(fp, "\t\tUDP Length: %u\n", udph->len);
	fprintf(fp, "\t\tUDP Checksum: %u\n", udph->check);
	fprintf(fp, "\n\tData Payload\n");
	printData(msg+sizeof(struct ethhdr)+iphdr_len+sizeof(udph), size);
}

void print_ip_header(unsigned char* msg)
{
	struct iphdr* iph;
	struct sockaddr_in source, dest;

	print_ethernet_header(msg);
	iph = (struct iphdr*)(msg+sizeof(struct ethhdr));
	source.sin_addr.s_addr = iph->saddr;
	dest.sin_addr.s_addr = iph->daddr;
	fprintf(fp, "\n\tIP Header\n");
	fprintf(fp, "\t\tIP Version: %u\n", iph->version);
	fprintf(fp, "\t\tIP Header Length: %u dwords (%u bytes)\n", iph->ihl, iph->ihl*4);
	fprintf(fp, "\t\tType Of Service: %u\n", iph->tos);
	fprintf(fp, "\t\tIP Total Length: %u\n", iph->tot_len);
	fprintf(fp, "\t\tIdentification: %u\n", iph->id);
	// fprintf(fp, "\tReserved ZERO Field: %u\n", iphdr->ip_reserved_zero);
	// fprintf(fp, "\tDon't Fragment Field: %u\n", iphdr->ip_dont_fragment);
	// fprintf(fp, "\tMore Fragment Field: %u\n", iphdr->ip_more_fragment);
	fprintf(fp, "\t\tTTL: %u\n", iph->ttl);
	fprintf(fp, "\t\tProtocol: %u\n", iph->protocol);
	fprintf(fp, "\t\tChecksum: %u\n", iph->check);
	fprintf(fp, "\t\tSource IP: %s\n", inet_ntoa(source.sin_addr));
	fprintf(fp, "\t\tDestination IP: %s\n", inet_ntoa(dest.sin_addr));
}

void print_ethernet_header(unsigned char* msg)
{
	struct ethhdr* eth = (struct ethhdr*)msg;

	fprintf(fp, "\n\tEthernet Header\n");
	fprintf(fp, "\t\tDestination Address: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", eth->h_dest[0], eth->h_dest[1], eth->h_dest[2], eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]); // why %.2X? Won't %X suffice?
	fprintf(fp, "\t\tSource Address: %.2X:%.2X:%.2X:%.2X:%.2X:%.2X\n", eth->h_source[0], eth->h_source[1], eth->h_source[2], eth->h_source[3], eth->h_source[4], eth->h_source[5]);
	fprintf(fp, "\t\tProtocol: %u\n", eth->h_proto);
}

// changes left
void printData(unsigned char* msg, int size)
{
	int i, j;
	for(i = 0; i < size; i++)
	{
		if(i != 0 && i%16 == 0)
		{
			for(j = i-16; j < i; j++)
			{
				if(msg[j] >= 32 && msg[j] <=127)
					fprintf(fp, "%c", (unsigned char)msg[j]);
				else
					fprintf(fp, ".");
			}
			fprintf(fp, "\n");
		}
		if(i%16 == 0)
			fprintf(fp, "\t\t\t%02X||", (unsigned int)msg[i]); // what??
		if(i == size-1)
		{
			for(j = i-(i%16); j <= i; j++)
			{
				if(msg[j] >= 32 && msg[j] <=127)
					fprintf(fp, "%c", (unsigned char)msg[j]);
				else
					fprintf(fp, ".");
			}
			fprintf(fp, "\n");
		}
	}
}
