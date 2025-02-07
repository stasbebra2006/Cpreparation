#include <stdio.h>


void read_WH(int * W, int * H, int * error)
{
	printf("Napiste mapu\n");
	int test = scanf(" %d %d", W, H);
	if(test != 2)
	{
		*error = 1;
		return;
	}
	printf("W = %d, H = %d\n", *W, *H);
}



int main()
{
	int error = 0;
	int W,H;
	read_WH(&W,&H, &error);
	if(error == 1)
	{
		printf("Nespravny vstup\n");
		return 1;
	}
	return 0;
}