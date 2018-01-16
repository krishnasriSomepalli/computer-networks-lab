#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAX 100
#define SIZE 1

struct IP_MAC{
    char IP[16], MAC[24];
};

int main(int argc, char* argv[])
{
    int sfd, clen, i;
    struct sockaddr_in servaddr, caddr;
    char msg[MAX];
    struct IP_MAC table[SIZE];
    strcpy(table[0].IP, "172.17.0.68");
    strcpy(table[0].MAC, "02:42:ac:11:00:27");
    if(argc != 3)
    {
        printf("Usage: ./a.out <server IPv4> <port number>\n");
        exit(1);
    }
    printf("Server has been started...\n");
    if((sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) == -1)
    {
        perror("socket() error");
        exit(2);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    if(bind(sfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind() error");
        exit(3);
    }
    clen = sizeof(servaddr);
    // receiving IP from the client - ARP simulation
    if(recvfrom(sfd, (void*)&msg, MAX, 0,(struct sockaddr*)&caddr, &clen) == -1)
    {
        perror("recvfrom() error");
        exit(4);
    }
    printf("Received IP address.\n");
    for(i = 0; i < SIZE; i++)
    {
        if(strcmp(msg, table[i].IP) == 0)
        {
            strcpy(msg, table[i].MAC);
            if(sendto(sfd, (void*)&msg, MAX, 0, (struct sockaddr*)&caddr, clen) == -1)
            {
                perror("sendto() error");
                exit(5);
            }
            printf("Sent corresponding MAC address.\n");
            break;
        }
    }

    // receiving MAC from the client - RARP simulation
    if(recvfrom(sfd, (void*)msg, MAX, 0,(struct sockaddr*)&caddr, &clen) == -1)
    {
        perror("recvfrom() error");
        exit(6);
    }
    printf("Received MAC address.\n");
    for(i = 0; i < SIZE; i++)
    {
        if(strcmp(msg, table[i].MAC) == 0)
        {
            strcpy(msg, table[i].IP);
            if(sendto(sfd, (void*)msg, MAX, 0, (struct sockaddr*)&caddr, clen) == -1)
            {
                perror("sendto() error");
                exit(7);
            }
            printf("Sent corresponding IP address.\n");
            break;
        }
    }
    if(close(sfd) == -1)
    {
        perror("close() error");
        exit(8);
    }
    printf("Stopped.\n");
    return 0;
}