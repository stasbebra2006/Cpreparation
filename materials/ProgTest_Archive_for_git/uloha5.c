#include <stdio.h>
#include <stdlib.h>
typedef struct
{
	int price;
} PRICE;

void add_price(PRICE **price_list, size_t *prices_size, size_t counter_prices, int price_read_value)
{
	if (counter_prices >= *prices_size)
	{
		*prices_size *= 2;
		PRICE *temp = (PRICE *)realloc(*price_list, *prices_size * sizeof(PRICE));
		if (temp == NULL)
		{
			printf("Memory allocation failed\n");
			return;
		}
		*price_list = temp;
	}
	(*price_list)[counter_prices - 1].price = price_read_value;
	// printf("readed price: %d\n", (*price_list)[counter_prices - 1].price);
}

void Conclude(PRICE *price_list, size_t *prices_size, size_t from, size_t to)
{

	int max_profit = 0;
	size_t buy_time = 0;
	size_t sell_time = 0;
	int min_price = price_list[from].price;
	size_t min_time = from;

	for (size_t i = from + 1; i <= to; i++)
	{
		if (price_list[i].price - min_price > max_profit)
		{
			max_profit = price_list[i].price - min_price;
			buy_time = min_time;
			sell_time = i;
		}
		if (price_list[i].price < min_price)
		{
			min_price = price_list[i].price;
			min_time = i;
		}
	}

	int max_loss = 0;
	size_t buy_time_loss = 0;
	size_t sell_time_loss = 0;
	int max_price = price_list[from].price;
	size_t max_time = from;

	for (size_t i = from + 1; i <= to; i++)
	{
		if (max_price - price_list[i].price > max_loss)
		{
			max_loss = max_price - price_list[i].price;
			buy_time_loss = max_time;
			sell_time_loss = i;
		}
		if (price_list[i].price > max_price)
		{
			max_price = price_list[i].price;
			max_time = i;
		}
	}

	if (max_profit > 0)
	{
		printf("Nejvyssi zisk: %d (%zu - %zu)\n", max_profit, buy_time, sell_time);
	}
	else
	{
		printf("Nejvyssi zisk: N/A\n");
	}
	if (max_loss > 0)
	{
		printf("Nejvyssi ztrata: %d (%zu - %zu)\n", max_loss, buy_time_loss, sell_time_loss);
	}
	else
	{
		printf("Nejvyssi ztrata: N/A\n");
	}
}

void Mainline(PRICE **price_list, size_t *prices_size)
{
	size_t counter_prices = 0;
	while (1)
	{
		char op;
		int result;
		result = scanf(" %c", &op);
		if (result == EOF)
		{
			break;
		}
		if (result != 1 || (op != '+' && op != '?'))
		{
			printf("Nespravny vstup.\n");
			exit(1);
		}
		else
		{
			// printf("readed symbol: %c\n", op);
			if (op == '+')
			{
				int price_read_value;
				int result = scanf(" %d", &price_read_value);
				if (result == EOF)
				{
					break;
				}
				if (result != 1 || price_read_value < 0)
				{
					printf("Nespravny vstup.\n");
					exit(1);
				}
				counter_prices++;
				add_price(price_list, prices_size, counter_prices, price_read_value);
			}
			if (op == '?')
			{
				size_t order_read_from, order_read_to;
				int result = scanf("%zu %zu", &order_read_from, &order_read_to);
				if (result == EOF)
				{
					break;
				}
				if (result != 2 || order_read_from > order_read_to || order_read_to > counter_prices - 1)
				{
					printf("Nespravny vstup.\n");
					exit(1);
				}
				Conclude(*price_list, prices_size, order_read_from, order_read_to);
			}
		}
	}
}

int main()
{
	printf("Ceny, hledani:\n");
	PRICE *price_list = (PRICE *)malloc(10 * sizeof(PRICE));
	size_t prices_size = 10;

	if (price_list == NULL)
	{
		printf("Memory allocation failed\n");
		return 1;
	}

	Mainline(&price_list, &prices_size);

	free(price_list);
	return 0;
}
