#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<sys/socket.h>

#define MAX 100
#define SIZE 1

int main(int argc, char* argv[])
{
    int sfd, clen, i;
    struct sockaddr_in servaddr, caddr;
    char msg[MAX], temp[MAX];
    if(argc != 3)
    {
        printf("Usage: ./a.out <server IPv4> <port number>\n");
        exit(1);
    }
    if((sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) == -1)
    {
        perror("socket() error");
        exit(2);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
    printf("IP address of device on subnet: ");
    scanf("%s", msg);
    if(sendto(sfd, (void*)msg, MAX, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("sendto() error");
        exit(3);
    }
    strcpy(temp, msg);
    clen = sizeof(servaddr);
    if(recvfrom(sfd, (void*)msg, MAX, 0, (struct sockaddr*)&caddr, &clen) == -1)
    {
        perror("recvfrom() error");
        exit(4);
    }
    printf("MAC address of the device with IP address %s is %s\n\n", temp, msg);
    printf("MAC address of device on subnet: ");
    scanf("%s", msg);
    if(sendto(sfd, (void*)msg, MAX, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("sendto() error");
        exit(5);
    }
    strcpy(temp, msg);
    if(recvfrom(sfd, (void*)msg, MAX, 0, (struct sockaddr*)&caddr, &clen) == -1)
    {
        perror("recvfrom() error");
        exit(6);
    }
    printf("IP address of the device with MAC address %s is %s\n", temp, msg);
    close(sfd);
    return 0;
}