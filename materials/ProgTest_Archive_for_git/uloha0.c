#include <stdio.h>

int main()
{
	const char *quotes[] = {
		"Qapla'\nnoH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n",
		"Qapla'\nbortaS bIr jablu'DI' reH QaQqu' nay'.\n",
		"Qapla'\nQu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n",
		"Qapla'\nbISeH'eghlaH'be'chugh latlh Dara'laH'be'.\n",
		"Qapla'\nqaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n",
		"Qapla'\nSuvlu'taHvIS yapbe' HoS neH.\n",
		"Qapla'\nHa'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n",
		"Qapla'\nHeghlu'meH QaQ jajvam.\n",
		"Qapla'\nleghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n"};
	int a = 0;
	printf("ml' nob:\n");
	if (scanf("%d", &a) != 1)
	{
		printf("Neh mi'\n");
	}
	else if (a < 0 || a >= (int)(sizeof(quotes) / sizeof(quotes[0])))
	{
		printf("Qih mi' %d\n", a);
	}
	else
	{
		printf(quotes[a]);
	}
	return 0;
}