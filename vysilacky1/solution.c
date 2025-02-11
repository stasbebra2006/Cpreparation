#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <float.h>

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

station *station_add(station *next, char *name, int x, int y)
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
		printf("NO stations\n");
	}
	int i = 1;
	while (stationsList)
	{
		printf("station %d\n", i);
		printf("station name = %s\n", stationsList->name);
		printf("station x = %d\n", stationsList->x);
		printf("station y = %d\n", stationsList->y);
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
		//
		printf("position: %d\n", position);
		//
		int checkScanf = scanf("%c", &symbol);
		if (symbol == ' ' || symbol == '\n' || checkScanf == EOF)
		{
			break;
		}
		if (checkScanf != 1 || (symbol != '/' && !isdigit(symbol) && !isupper(symbol)) || position >= 16)
		{
			//
			printf("this error 2\n");
			printf("checked char is - \"%c\"\n", symbol);
			printf("position fale is: %d\n", position);
			printf("checkScanf = \"%d\"\n", checkScanf);
			//
			free(name);
			*error = 1;
			return NULL;
		}
		name[position] = symbol;
		position++;
	}
	if (position == 0)
	{
		free(name);
		*error = 1;
		return NULL;
	}
	name[position] = '\0';
	return name;
}

station *read_station(station *stationsList, int *error)
{
	char *name = read_name(error);
	if (*error == 1)
	{
		//
		printf("this error 1\n");
		//
		return NULL;
	}
	int currentX = 0, currentY = 0;
	int checkScanf = scanf(" %d %d", &currentX, &currentY);
	if (checkScanf != 2)
	{
		free(name);
		*error = 1;
	}
	stationsList = station_add(stationsList, name, currentX, currentY);
	return stationsList;
}

double find_distance(station *station1, station *station2)
{
	return sqrt(abs(pow(station2->x - station1->x, 2)) + abs(pow(station2->y - station1->y, 2)));
}

void find_distance_between(station *stationsList, int *error)
{
	if (getchar() != ' ')
	{
		*error = 1;
		return;
	}
	printf("%% is called\n");
	char *name1 = read_name(error);
	if (*error == 1)
	{
		free(name1);
		return;
	}
	char *name2 = read_name(error);
	if (*error == 1)
	{
		free(name1);
		free(name2);
		return;
	}
	//
	printf("first name is %s\n", name1);
	printf("second name is %s\n", name2);
	//

	station *station1 = NULL;
	station *station2 = NULL;
	while (stationsList)
	{
		if (strcmp(stationsList->name, name1) == 0)
			station1 = stationsList;
		if (strcmp(stationsList->name, name2) == 0)
			station2 = stationsList;
		stationsList = stationsList->next;
	}
	if (!station1 || !station2)
	{
		printf("Nenalezeno.\n");
	}
	else
	{
		double distance = find_distance(station1, station2);
		printf("distance between is %.2f\n", distance);
	}
	free(name1);
	free(name2);
}

void find_the_nearest(station *stationsList, int *error)
{
	printf("? is called\n");
	int currentX = 0;
	int currentY = 0;
	station *holder = calloc(1, sizeof(station));
	holder->x = currentX;
	holder->y = currentY;
	station *nearest = NULL;
	int checkScanf = scanf(" %d %d", &currentX, &currentY);
	if (checkScanf != 2)
	{
		*error = 1;
		return;
	}
	//
	printf("currentX = %d\n", currentX);
	printf("currentY = %d\n", currentY);
	//
	double distanceMin = DBL_MAX;
	while (stationsList)
	{
		double distanceCurrent = find_distance(holder, stationsList);
		if (distanceCurrent < distanceMin || (distanceCurrent == distanceMin && strcmp(stationsList->name,nearest->name) < 0))
		{
			distanceMin = distanceCurrent;
			nearest = stationsList;
		}
		stationsList = stationsList->next;
	}
	printf("Nejblizsi: %s %d %d\n", nearest->name, nearest->x, nearest->y);
	printf("Vzdalenost: %.2f\n", distanceMin);
	free(holder);
}

station *crossroad(station *stationsList, int *error)
{
	int isListComplited = 0;
	while (1)
	{
		char decisionMaker = getchar();
		//
		printf("decisionMaker = %c\n", decisionMaker);
		//
		if (decisionMaker == EOF)
		{
			if (!stationsList)
			{
				*error = 1;
				return stationsList;
			}
			return stationsList;
		}
		if (decisionMaker == '\n')
		{
			continue;
		}
		//
		printf("decisionMaker after ungetc = %c\n", decisionMaker);
		//
		if (isupper(decisionMaker) || decisionMaker == '/' || isdigit(decisionMaker))
		{
			//
			printf("im here\n");
			//
			if (isListComplited == 0)
			{
				//
				printf("im here 2\n");
				//
				ungetc(decisionMaker, stdin);
				stationsList = read_station(stationsList, error);
				if (*error == 1)
				{
					return stationsList;
				}
			}
			else
			{
				*error = 1;
				return stationsList;
			}
		}
		else
		{
			isListComplited++;
			//
			printf("isListComplited has changed, it's \"%d\" now\n", isListComplited);
			//
			switch (decisionMaker)
			{
			case '%':
				find_distance_between(stationsList, error);
				if (*error == 1)
					return stationsList;
				break;
			case '?':
				find_the_nearest(stationsList, error);
				if (*error == 1)
					return stationsList;
				break;
			default:
				*error = 1;
				return stationsList;
			}
		}
	}
	return stationsList;
}

int main()
{
	int error = 0;
	station *stationsList = stations_init();
	stationsList = crossroad(stationsList, &error);
	if (error == 1)
	{
		printf("Nespravny vstup.\n");
		free_stations_list(stationsList);
		return 1;
	}
	print_stations_list(stationsList);
	free_stations_list(stationsList);
}