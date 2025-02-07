#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

constexpr unsigned DOW_MON = 0b0000'0001;
constexpr unsigned DOW_TUE = 0b0000'0010;
constexpr unsigned DOW_WED = 0b0000'0100;
constexpr unsigned DOW_THU = 0b0000'1000;
constexpr unsigned DOW_FRI = 0b0001'0000;
constexpr unsigned DOW_SAT = 0b0010'0000;
constexpr unsigned DOW_SUN = 0b0100'0000;
constexpr unsigned DOW_WORKDAYS = DOW_MON | DOW_TUE | DOW_WED | DOW_THU | DOW_FRI;
constexpr unsigned DOW_WEEKEND = DOW_SAT | DOW_SUN;
constexpr unsigned DOW_ALL = DOW_WORKDAYS | DOW_WEEKEND;

typedef struct TDate
{
	unsigned m_Year;
	unsigned m_Month;
	unsigned m_Day;
} TDATE;

TDATE makeDate(unsigned y,
			   unsigned m,
			   unsigned d)
{
	TDATE res = {y, m, d};
	return res;
}
#endif /* __PROGTEST__ */

unsigned dayOfWeek(TDATE current)
{
	unsigned year = current.m_Year;
	unsigned month = current.m_Month;
	unsigned day = current.m_Day;

	if (month < 3)
	{
		month += 12;
		year -= 1;
	}

	return (day + 2 * month + (3 * (month + 1)) / 5 + year + year / 4 - year / 100 + year / 400) % 7;
}

unsigned calculateConnectionsForDay(unsigned currentDay, unsigned perWorkDay, unsigned dowMask)
{
	switch (currentDay)
	{
	case 0:
		if (dowMask & DOW_MON)
		{
			return perWorkDay;
		}
		break;
	case 1:
		if (dowMask & DOW_TUE)
		{
			return perWorkDay;
		}
		break;
	case 2:
		if (dowMask & DOW_WED)
		{
			return perWorkDay;
		}
		break;
	case 3:
		if (dowMask & DOW_THU)
		{
			return perWorkDay;
		}
		break;
	case 4:
		if (dowMask & DOW_FRI)
		{
			return perWorkDay;
		}
		break;
	case 5:
		if (dowMask & DOW_SAT)
		{
			return (unsigned)ceil((double)perWorkDay / 2);
		}
		break;
	case 6:
		if (dowMask & DOW_SUN)
		{
			return (unsigned)ceil((double)perWorkDay / 3);
		}
		break;
	}
	return 0;
}

unsigned isLeapYear(TDATE currentDate)
{
	if (currentDate.m_Year % 4000 == 0)
		return 0;
	if (currentDate.m_Year % 400 == 0)
		return 1;
	if (currentDate.m_Year % 100 == 0)
		return 0;
	return currentDate.m_Year % 4 == 0;
}

unsigned daysInMonth(TDATE currentDate)
{
	switch (currentDate.m_Month)
	{
	case 2:
		return isLeapYear(currentDate) ? 29 : 28;
	case 4:
	case 6:
	case 9:
	case 11:
		return 30;
	default:
		return 31;
	}
}

unsigned isValidDate(TDATE from, TDATE to)
{
	if (from.m_Year > to.m_Year || (from.m_Year == to.m_Year && from.m_Month > to.m_Month) || (from.m_Year == to.m_Year && from.m_Month == to.m_Month && from.m_Day > to.m_Day))
	{
		return 1;
	}
	if (from.m_Month < 1 || from.m_Month > 12 || to.m_Month < 1 || to.m_Month > 12)
	{
		return 1;
	}
	if ((!isLeapYear(from) && from.m_Month == 2 && from.m_Day == 29) || (!isLeapYear(to) && to.m_Month == 2 && to.m_Day == 29))
	{
		return 1;
	}

	if (from.m_Day < 1 || from.m_Day > daysInMonth(from) || to.m_Day < 1 || to.m_Day > daysInMonth(to))
	{
		return 1;
	}
	return 0;
}

unsigned isValidDateCompact(TDATE from)
{
	if (from.m_Year < 2000)
	{
		return 1;
	}
	if (from.m_Month < 1 || from.m_Month > 12)
	{
		return 1;
	}
	if ((!isLeapYear(from) && from.m_Month == 2 && from.m_Day == 29))
	{
		return 1;
	}

	if (from.m_Day < 1 || from.m_Day > daysInMonth(from))
	{
		return 1;
	}
	return 0;
}

TDATE nextDay(TDATE currentDate)
{
	if (currentDate.m_Day < daysInMonth(currentDate))
	{
		currentDate.m_Day++;
	}
	else
	{
		currentDate.m_Day = 1;
		if (currentDate.m_Month < 12)
		{
			currentDate.m_Month++;
		}
		else
		{
			currentDate.m_Month = 1;
			currentDate.m_Year++;
		}
	}
	return currentDate;
}

long long countConnections(TDATE from,
						   TDATE to,
						   unsigned perWorkDay,
						   unsigned dowMask)
{
	if (isValidDate(from, to))
	{
		return -1;
	}
	long long totalConnections = 0;
	TDATE currentDate = from;
	while (currentDate.m_Year < to.m_Year || (currentDate.m_Year == to.m_Year && currentDate.m_Month < to.m_Month) || (currentDate.m_Year == to.m_Year && currentDate.m_Month == to.m_Month && currentDate.m_Day <= to.m_Day))
	{
		unsigned dow = dayOfWeek(currentDate);
		totalConnections += calculateConnectionsForDay(dow, perWorkDay, dowMask);
		currentDate = nextDay(currentDate);
	}

	return totalConnections;
}

TDATE endDate(TDATE from,
			  long long connections,
			  unsigned perWorkDay,
			  unsigned dowMask)
{
	if (connections < 0 || perWorkDay == 0 || dowMask == 0 || isValidDateCompact(from))
	{
		return makeDate(0, 0, 0);
	}
	long long int remainingConnections = connections;
	TDATE currentDate = from;
	if (remainingConnections - calculateConnectionsForDay(dayOfWeek(currentDate), perWorkDay, dowMask) < 0)
	{
		return makeDate(0, 0, 0);
	}
	while (remainingConnections >= 0)
	{
		unsigned dow = dayOfWeek(currentDate);
		remainingConnections -= calculateConnectionsForDay(dow, perWorkDay, dowMask);
		if (remainingConnections - calculateConnectionsForDay((dow + 1) % 7, perWorkDay, dowMask) < 0)
		{
			break;
		}
		currentDate = nextDay(currentDate);
	}
	return currentDate;
}

#ifndef __PROGTEST__
int main()
{
	TDATE d;
	assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1, DOW_ALL) == 31);
	assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 10, DOW_ALL) == 266);
	assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 31), 1, DOW_WED) == 5);
	assert(countConnections(makeDate(2024, 10, 2), makeDate(2024, 10, 30), 1, DOW_WED) == 5);
	assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10, DOW_TUE) == 10);
	assert(countConnections(makeDate(2024, 10, 1), makeDate(2024, 10, 1), 10, DOW_WED) == 0);
	assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 5, DOW_MON | DOW_FRI | DOW_SAT) == 7462);
	assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 0, DOW_MON | DOW_FRI | DOW_SAT) == 0);
	assert(countConnections(makeDate(2024, 1, 1), makeDate(2034, 12, 31), 100, 0) == 0);
	assert(countConnections(makeDate(2024, 10, 10), makeDate(2024, 10, 9), 1, DOW_MON) == -1);
	assert(countConnections(makeDate(2024, 2, 29), makeDate(2024, 2, 29), 1, DOW_ALL) == 1);
	assert(countConnections(makeDate(2023, 2, 29), makeDate(2023, 2, 29), 1, DOW_ALL) == -1);
	assert(countConnections(makeDate(2100, 2, 29), makeDate(2100, 2, 29), 1, DOW_ALL) == -1);
	assert(countConnections(makeDate(2400, 2, 29), makeDate(2400, 2, 29), 1, DOW_ALL) == 1);
	assert(countConnections(makeDate(4000, 2, 29), makeDate(4000, 2, 29), 1, DOW_ALL) == -1);
	d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_ALL);
	assert(d.m_Year == 2025 && d.m_Month == 1 && d.m_Day == 8);
	d = endDate(makeDate(2024, 10, 1), 100, 6, DOW_ALL);
	assert(d.m_Year == 2024 && d.m_Month == 10 && d.m_Day == 20);
	d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_WORKDAYS);
	assert(d.m_Year == 2025 && d.m_Month == 2 && d.m_Day == 17);
	d = endDate(makeDate(2024, 10, 1), 100, 4, DOW_WORKDAYS);
	assert(d.m_Year == 2024 && d.m_Month == 11 && d.m_Day == 4);
	d = endDate(makeDate(2024, 10, 1), 100, 1, DOW_THU);
	assert(d.m_Year == 2026 && d.m_Month == 9 && d.m_Day == 2);
	d = endDate(makeDate(2024, 10, 1), 100, 2, DOW_THU);
	assert(d.m_Year == 2025 && d.m_Month == 9 && d.m_Day == 17);
	d = endDate(makeDate(2024, 10, 1), 100, 0, DOW_THU);
	assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
	d = endDate(makeDate(2024, 10, 1), 100, 1, 0);
	assert(d.m_Year == 0 && d.m_Month == 0 && d.m_Day == 0);
	return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
