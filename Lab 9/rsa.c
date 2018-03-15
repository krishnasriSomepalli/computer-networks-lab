// d*e mod phi = 1, i.e. d*e = (phi*k)+1

#include <stdio.h>
int main()
{
    int p, q, n, phi, e, d;
    char msg[30];
    int cipher[30], plain[30];
    int i, j, k;

    printf("Enter two relatively prime numbers:\n\tp: ");
    scanf("%d", &p);
    printf("\tq: ");
    scanf("%d", &q);
    n = p*q;
    phi = (p-1)*(q-1);
    printf("Enter a coprime of %d (1 < e < %d): ", phi, phi);
    scanf("%d", &e);
    // so we choose the first value of d that satisfies the required condition
    d = 1;
    while((e*d)%phi != 1)
        d++;
    printf("\te:%d\td:%d\n", e, d);

    printf("Enter any message: ");
    getchar(); // to consume the previous newline that scanf didn't (?)
    scanf("%[^\n]s", msg);
    i = 0;
    while(msg[i] != '\0'){
        // encrypting
        cipher[i] = 1;
        for(k = 0; k < e; k++)
            cipher[i] = (cipher[i]*msg[i])%n;
        cipher[i] = cipher[i]%n;
        i++;
    }
    cipher[i] = msg[i];
    j = i;

    printf("\tEncrypted data: ");
    for(i = 0; i < j; i++)
        printf("%d ", cipher[i]);
    printf("\n");

    // decrypting
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

    printf("\tDecrypted data: ");
    for(i = 0; i < j; i++)
        printf("%d ", plain[i]);
    printf("\n");

    // decoding
    printf("Text (ASCII decoding): ");
    for(i=0; i<j; i++)
        printf("%c",plain[i]);
    printf("\n");
    return 0;
}