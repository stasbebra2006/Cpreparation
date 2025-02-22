#include <iostream>
#include <iomanip>
#include <cassert>

using namespace std;

constexpr size_t INIT_ARRAY_CAPACITY = 10;

struct Array
{
	int *data;
	size_t size;
	size_t capacity;
};

void a_init(Array &array)
{
	array.capacity = INIT_ARRAY_CAPACITY;
	array.size = 0;
	array.data = new int[array.capacity];
}

bool a_push(Array &array, int elem)
{
	assert(array.size <= array.capacity);
	if (array.size == array.capacity)
	{
		array.capacity = 2 * array.capacity + 2;
		int *tmp = new int[array.capacity];
		for (size_t i = 0; i < array.size; i++)
		{
			tmp[i] = array.data[i];
		}
		if (!tmp)
			return false;
		delete[] array.data;
		array.data = tmp;
	}

	array.data[array.size++] = elem;
	return true;
}

bool a_read(Array &array)
{
	int number;

	while (cin >> number)
		if (!a_push(array, number))
		{
			return false;
		}

	return cin.eof();
}

void a_print(const Array &array)
{
	for (size_t i = 0; i < array.size; i++)
		cout << array.data[i];

	cout << endl;
}

int main()
{
	Array array;
	a_init(array);

	cout << "Write numbers:" << endl;
	if (!a_read(array))
	{
		cout << "Program failed." << endl;
		delete[] array.data;
		return EXIT_FAILURE;
	}

	cout << "Scanned numbers:" << endl;
	a_print(array);
	delete[] array.data;
	return EXIT_SUCCESS;
}