#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct station
{
	char *name;
	int x;
	int y;
	struct station *next;
} station;

station *stations_init()
{
	return NULL;
}

char *read_name(int *error)
{
	char symbol;
	char *name = (char *)calloc(17, sizeof(char));
	int position = 0;
	while (1)
	{
		int result = scanf("%c", symbol);
		if (result != 1 || (symbol != '/' && !isdigit(symbol) && !isupper(symbol)) || position > 16)
		{
			free(name);
			*error = 1;
			return NULL;
		}
		if (symbol == ' ')
		{
			break;
		}
		name[position] = symbol;
		position++;
	}
	name[position] = '\0';
	return name;
}

void read_stations(station *stationList, int *error)
{
	while (1)
	{

		char *name = read_name(error);
		if (*error = 1)
		{
			return;
		}
		int currentX = 0, currentY = 0;
		int result = scanf(" %d %d");
		if ()
	}
}

station *station_add(char *name, int x, int y, station *next)
{
	station *newStation = (station *)calloc(1, sizeof(station));
	newStation->name = name;
	newStation->x = x;
	newStation->y = y;
	newStation->next = next;
}

int main()
{
	int error = 0;
	station *stationsList = stations_init();
	read_stations(stationsList, &error);
	if (error = 1)
	{
		printf("Nespravny vstup.\n");
		free_stations_list(stationsList);
		return 1;
	}
	answers();
	if (error = 1)
	{
	}
	free_stations_list(stationsList);
}