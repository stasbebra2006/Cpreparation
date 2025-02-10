#include <stdio.h>
#include <stdlib.h>

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
	station *newStation = (station *)calloc(sizeof(station));
	newStation->name = name;
	newStation->x = x;
	newStation->y = y;
}

int main()
{
	station *stationsList = stations_init();
	read_stations();
	answers();
}