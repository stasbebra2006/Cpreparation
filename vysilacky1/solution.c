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
	return newStation;
}

void free_stations_list(station *stationsList)
{
	if (!stationsList)
		return;
	while (stationsList)
	{
		station *temp = stationsList;
		stationsList = stationsList->next;
		free(temp->name);
		free(temp);
	}
}

void print_stations_list(station *stationsList)
{
	if (!stationsList)
	{
	}
	int i = 1;
	while (stationsList)
	{
		printf("station %d\n", i);
		printf("station name = \n", stationsList->name);
		printf("station x = \n", stationsList->x);
		printf("station y = \n", stationsList->y);
		stationsList = stationsList->next;
		i++;
	}
}

char *read_name(int *error)
{
	char symbol;
	char *name = (char *)calloc(17, sizeof(char));
	int position = 0;
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

station *read_station(station *stationsList, int *error)
{
	char *name = read_name(error);
	if (*error == 1)
	{
		return;
	}
	int currentX = 0, currentY = 0;
	int checkScanf = scanf(" %d %d", currentX, currentY);
	if (checkScanf != 2)
	{
		free(name);
		*error = 1;
	}
	stationsList = station_add(stationsList, name, currentX, currentY);
	return stationsList;
}

void crossroad(station *stationsList, int *error)
{
	int isListComplited = 0;
	while (1)
	{
		char decisionMaker = getchar();
		if (decisionMaker == EOF)
		{
			if (!stationsList)
			{
				*error = 1;
				return;
			}
		}
		if (decisionMaker == '\n')
		{
			continue;
		}
		ungetc(decisionMaker, stdin);
		if (isupper(decisionMaker) || decisionMaker == '/' || isdigit(decisionMaker))
		{
			if (isListComplited == 0)
			{
				stationsList = read_station(stationsList, error);
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