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

station *station_add(char *name, int x, int y, station *next)
{
	station *newStation = (station *)calloc(1, sizeof(station));
	newStation->name = name;
	newStation->x = x;
	newStation->y = y;
	newStation->next = next;
}

void free_stations_list(station *stationsList)
{
	if (!stationsList)
		return;
	while (stationsList)
	{
		station *temp = stationsList;
		stationsList = stationsList->next;
		free(temp);
	}
}

char *read_name(char firstInName, int *error)
{
	char symbol;
	char *name = (char *)calloc(17, sizeof(char));
	name[0] = firstInName;
	int position = 1;
	while (1)
	{
		int checkScanf = scanf("%c", symbol);
		if (checkScanf != 1 || (symbol != '/' && !isdigit(symbol) && !isupper(symbol)) || position > 16)
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

station *read_station(station *stationsList, char firstInName, int *error)
{
	char *name = read_name(firstInName, error);
	if (*error == 1)
	{
		return;
	}
	int currentX = 0, currentY = 0;
	int checkScanf = scanf(" %d %d", currentX, currentY);
	if (checkScanf != 2)
}

void crossroad(station *stationsList, int *error)
{
	int isListComplited = 0;
	while (1)
	{
		char decisionMaker;
		int checkScanf = scanf("%c", decisionMaker);
		if (checkScanf != 1)
		{
			*error = 1;
			return;
		}
		if (isupper(decisionMaker) || decisionMaker == '/' || isdigit(decisionMaker))
		{
			if (isListComplited == 0)
			{
				stationsList = read_station(stationsList, decisionMaker, error);
				if (*error == 1)
				{
					return;
				}
			}
			else
			{
				*error = 1;
				return;
			}
		}
		else
		{
			isListComplited++;
			switch (decisionMaker)
			{
			case '%':
				find_distance_between();
				break;
			case '?':
				find_the_nearest();
			default:
				*error = 1;
				return;
			}
		}
	}
}

int main()
{
	int error = 0;
	station *stationsList = stations_init();
	crossroad(stationsList, &error);
	if (error == 1)
	{
		printf("Nespravny vstup.\n");
		free_stations_list(stationsList);
		return 1;
	}
	free_stations_list(stationsList);
}