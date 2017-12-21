#include<stdio.h>
int BigEndian();
int main()
{
	unsigned short hex, hex1;
	hex = 0X56AB;
	int big_endian = BigEndian();
	if(big_endian == 1)
	{
		printf("Big Endian:\n");
		printf("Decimal: %d\n", hex);
		printf("Hexadecimal: 0X%X\n",hex);
		hex1 = htons(hex);
		printf("\nLittle Endian:\n");
                printf("Decimal: %d\n", hex1);
                printf("Hexadecimal: 0X%X\n",hex1);
	}
	else
	{
                printf("Little Endian:\n");
                printf("Decimal: %d\n", hex);
                printf("Hexadecimal: 0X%X\n",hex);
                hex1 = ntohs(hex);
                printf("\nBig Endian:\n");
                printf("Decimal: %d\n", hex1);
                printf("Hexadecimal: 0X%X\n",hex1);
	}
}

int BigEndian()
{
	int i = 1;
	char* c = (char*)&i;
	if(*c == 1) 
		return 0;
	else
		return 1;
}
