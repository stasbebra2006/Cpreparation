#include <iostream>
#include <sstream>
#include <cassert>
#include <memory>
#include <compare>
//size_
//top const ne
//a tak dal
#define LVL 5

struct LinkedList
{
	using T = int;

	LinkedList() : head(nullptr) {} // = default;
	// TODO destruktor
	~LinkedList()
	{
		while (!empty())
		{
			pop();
		}
	}

	// zakazeme kopirovani
	LinkedList(const LinkedList &) = delete;
	LinkedList &operator=(const LinkedList &) = delete;

	// TODO metody
	bool empty() const
	{
		return head == nullptr;
	}

	void push(const T &value)
	{
		head = new Node(value, head);
	}

	T pop()
	{
		if (empty())
		{
			throw std::out_of_range("Nemuzu popnout");
		}

		Node *tmp = head;
		head = head->next;
		T hodnota = tmp->value;
		delete tmp;
		return hodnota;
	}
	size_t size()
	{
		size_t counter = 0;
		Node * tmp = head;
		while(!(tmp == nullptr))
		{
			counter++;
			tmp = tmp->next;
		}
		return counter;
	}

	T top()
	{
		return head->value;
	}

	

private:
	// TODO atributy a dalsi privatni definice
	struct Node
	{
		T value;
		Node *next;

		Node(T val, Node *nxt) : value(val), next(nxt) {}
	};

	Node *head; //= nullptr
};

#if LVL >= 5
void check_list(const LinkedList &L, const std::string &ref)
{
	std::ostringstream out;
	out << L;
	assert(out.str() == ref);
}
#endif

#define DOES_NOT_COMPILE(...) assert(!requires { __VA_ARGS__; });

int main()
{
	LinkedList L;
	(void)L; // unused warning...

	// LVL 1: konstruktor, empty, push a pop
#if LVL >= 1
	assert(L.empty());

	for (int i = 0; i < 10; i++)
		L.push(i);
	assert(!L.empty());

	for (int i = 0; i < 5; i++)
	{
		int x = L.pop();
		assert(x == 9 - i);
	}
	assert(!L.empty());
#endif

	// LVL 2: destruktor
#if LVL == 1
	while (!L.empty())
		L.pop();
#endif

	// LVL 3: výjimky
#if LVL >= 3
	try
	{
		LinkedList L2;
		L2.pop();
		assert(0);
	}
	catch (const std::out_of_range &)
	{
		// OK
	}
#endif

	// LVL 4: top a size
#if LVL >= 4
	assert(L.size() == 5);
	assert(L.top() == 4);
	L.push(-10);
	assert(L.size() == 6);
	assert(L.top() == -10);
#endif

	// LVL 5: výpis pomocí <<, top() je první
#if LVL >= 5
	std::cout << L << std::endl;
	check_list(L, "-10 4 3 2 1 0");
	L.pop();
	check_list(L, "4 3 2 1 0");
#endif

	// LVL 6: vkládání pomocí << a vybírání >>
#if LVL >= 6
	assert(L.size() == 5);
	assert(L.top() == 4);
	L << -10 << 2;
	assert(L.size() == 7);
	assert(L.top() == 2);
	int x, y;
	L >> x << 4 >> y;
	assert(x == 2);
	assert(y == 4);
	assert(L.top() == -10);
#endif

	// LVL 7: consty
#if LVL >= 7
	[](auto &L)
	{
		const auto &constL = L;
		assert(constL.top() == -10);
		assert(!constL.empty());
		assert(constL.size() == 6);

		int x;
		DOES_NOT_COMPILE(constL.top() = 42); // NESMÍ fungovat
		DOES_NOT_COMPILE(constL.push());	 // NESMÍ fungovat
		DOES_NOT_COMPILE(constL.pop());		 // NESMÍ fungovat
		DOES_NOT_COMPILE(constL << 4);		 // NESMÍ fungovat
		DOES_NOT_COMPILE(constL >> x);		 // NESMÍ fungovat

		L.top() = 43; // musí fungovat
		assert(constL.top() == 43);
	}(L);
#endif

	// LVL 8: in-place (tj. upravuje současný objekt) otočení spojáku pomocí ~
#if LVL >= 8
	~L << 8;
	assert(L.size() == 7);
	assert(L.top() == 8);
	check_list(L, "8 0 1 2 3 4 43");
	L >> x;
	assert(x == 8);
	assert(L.top() == 0);
	check_list(L, "0 1 2 3 4 43");
#endif

	// LVL 9: porovnávání, kratší list je vždy menší, jinak lexikograficky
#if LVL >= 9
	LinkedList L2;
	assert(L2 < L);
	assert(!(L2 > L));
	assert(L2 != L);
	assert(!(L == L2));
	assert(!(L <= L2));
	assert(L >= L2);
	assert(!(L2 >= L));

	for (int x : {43, 4, 3, 2, 1})
		L2 << x;

	assert(L2 < L);
	assert(!(L2 > L));
	assert(L2 != L);
	assert(!(L == L2));
	assert(!(L <= L2));
	assert(L >= L2);
	assert(!(L2 >= L));

	L2 << 0;

	assert(!(L2 < L));
	assert(!(L2 > L));
	assert(L2 == L);
	assert(!(L != L2));
	assert(L <= L2);
	assert(L >= L2);

	L2 >> x << 1;

	assert(!(L2 < L));
	assert(L2 > L);
	assert(L2 != L);
	assert(!(L == L2));
	assert(L <= L2);
	assert(!(L >= L2));
#endif

	// LVL 10: top pomocí unárního + a pop vracející prvek pomocí unárního -
#if LVL >= 10
	x = -~L;
	assert(x == 43);
	assert(L.size() == 5);
	assert(+L == 4);
	check_list(L, "4 3 2 1 0");
#endif

	// LVL 11: testování neprázdnosti přetypováním na bool
#if LVL >= 11
	if (L)
	{
	}
	else
		assert(false);

	{
		const LinkedList empty;
		if (empty)
			assert(false);
	}

	{
		LinkedList L2;
		if (L2)
			assert(false);
		assert(!!(L2 << 0));
		assert(!(L2 >> x));
	}
#endif

	// LVL 99: inicializace pomocí std::initializer_list
#if LVL >= 99
	LinkedList L3 = {1, 2, 3, 4};
	check_list(L3, "4 3 2 1");
#endif

	std::cout << "Successfully completed level " << LVL << ".\n";
}
