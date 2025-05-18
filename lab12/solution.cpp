#include <cassert>
#include <cstdarg>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <array>
#include <random>
#include <type_traits>

// We use std::set as a reference to check our implementation.
#include <set>

template <typename T>
struct Ref
{
	size_t size() const { return _data.size(); }
	const T *find(const T &value) const
	{
		auto it = _data.find(value);
		if (it == _data.end())
			return nullptr;
		return &*it;
	}
	bool insert(const T &value) { return _data.insert(value).second; }
	bool erase(const T &value) { return _data.erase(value); }

private:
	std::set<T> _data;
};

// TODO implement
template <typename T>
struct Tree
{
	struct Node
	{
		Node(T v) : val(v), left(nullptr), right(nullptr) {} // Added constructor

		size_t size() const
		{
			size_t result = 1;
			if (left)
			{
				result += left->size();
			}
			if (right)
			{
				result += right->size();
			}
			return result;
		}

		T val;
		std::shared_ptr<Node> left;
		std::shared_ptr<Node> right;
	};
	size_t size() const
	{
		if (!root)
			return 0;
		return root->size();
	}
	const T *find(const T &value) const
	{
		std::shared_ptr<Node> current = root;
		while (current)
		{
			if (current->val == value)
			{
				return &(current->val);
			}

			if (value < current->val)
			{
				current = current->left;
			}
			else
			{
				current = current->right;
			}
		}
		return nullptr;
	}
	bool insert(T value)
	{
		std::shared_ptr<Node> parrent = nullptr, current = root;
		while (current)
		{
			if (current->val == value)
			{
				return false; // Value already exists
			}
			parrent = current;
			if (value < current->val)
			{
				current = current->left;
			}
			else
			{
				current = current->right;
			}
		}
		// Create the new node
		std::shared_ptr<Node> newNode = std::make_shared<Node>(value);

		if (!parrent) // Tree was empty
		{
			root = newNode;
		}
		else
		{ // Tree was not empty, attach to parent
			if (value < parrent->val)
			{
				parrent->left = newNode;
			}
			else
			{ // value >= parrent->val (but not equal due to check above)
				parrent->right = newNode;
			}
		}
		return true;
	}
	bool erase(const T &value) // Declaration only, implementation needed
	{
		std::shared_ptr<Node> parrent = nullptr, current = root;
		while (current)
		{
			if (current->val == value)
			{
				if (!parrent)
				{
					root = nullptr;
				}
				else
				{
					if (!current->left && !current->right)
					{
						if (value < parent->val)
						{
							parrent->left = nullptr;
						}
						else
						{
							parrent->right = nullptr;
						}
					}
				}
			}
			else if (!current->left || !current->right)
			{
				auto &child = value < parrent->val ? parrent->left : parrent->right;
				child = current->left ? current->left : current->right;
			}
		}
		return true;
		// TODO both and fix the current state
	}

private:
	// std::shared_ptr<Node> & find_node(const T& value) const {
	// 	std::shared_ptr<Node> & current = root;
	// 	while (current) {
	// 		if(current->val == value)
	// 		{
	// 			return &(current->val); // Incorrect return type
	// 		}

	// 		if(value < current->val) {
	// 			current = current->left;
	// 		}
	// 		else{
	// 			current = current->right;
	// 		}
	// 	}
	// 	return nullptr; // Incorrect return type
	// } --ono boli -- const reference -- mutable

	std::shared_ptr<Node> root;
};

struct TestFailed : std::runtime_error
{
	using std::runtime_error::runtime_error;
};

std::string fmt(const char *f, ...)
{
	va_list args1;
	va_list args2;
	va_start(args1, f);
	va_copy(args2, args1);

	std::string buf(vsnprintf(nullptr, 0, f, args1), '\0');
	va_end(args1);

	vsnprintf(buf.data(), buf.size() + 1, f, args2);
	va_end(args2);

	return buf;
}

template <typename T>
struct Tester
{
	Tester() = default;

	void size() const
	{
		size_t r = ref.size();
		size_t t = tested.size();
		if (r != t)
			throw TestFailed(fmt("Size: got %zu but expected %zu.", t, r));
	}

	void find(const T &x) const
	{
		auto r = ref.find(x);
		auto t = tested.find(x);
		bool found_r = r != nullptr;
		bool found_t = t != nullptr;

		if (found_r != found_t)
			_throw("Find mismatch", found_r);
		if (found_r && *t != x)
			throw TestFailed("Find: found different value");
	}

	void insert(const T &x)
	{
		auto succ_r = ref.insert(x);
		auto succ_t = tested.insert(x);
		if (succ_r != succ_t)
			_throw("Insert mismatch", succ_r);
		size();
	}

	void erase(const T &x)
	{
		bool succ_r = ref.erase(x);
		auto succ_t = tested.erase(x);
		if (succ_r != succ_t)
			_throw("Erase mismatch", succ_r);
		size();
	}

	static void _throw(const char *msg, bool s)
	{
		throw TestFailed(fmt("%s: ref %s.", msg, s ? "succeeded" : "failed"));
	}

	Tree<T> tested;
	Ref<T> ref;
};

void test_insert()
{
	Tester<int> t;

	for (int i = 0; i < 10; i++)
		t.insert(i);
	for (int i = -10; i < 20; i++)
		t.find(i);

	for (int i = 0; i < 10; i++)
		t.insert((1 + i * 7) % 17);
	for (int i = -10; i < 20; i++)
		t.find(i);
}

void test_erase()
{
	Tester<int> t;

	for (int i = 0; i < 10; i++)
		t.insert((1 + i * 7) % 17);
	for (int i = -10; i < 20; i++)
		t.find(i);

	for (int i = 3; i < 22; i += 2)
		t.erase(i);
	for (int i = -10; i < 20; i++)
		t.find(i);

	for (int i = 0; i < 10; i++)
		t.insert((1 + i * 13) % 17 - 8);
	for (int i = -10; i < 20; i++)
		t.find(i);

	for (int i = -4; i < 10; i++)
		t.erase(i);
	for (int i = -10; i < 20; i++)
		t.find(i);
}

enum RandomTestFlags : unsigned
{
	SEQ = 1,
	NO_ERASE = 2
};

void test_random(size_t size, unsigned flags = 0)
{
	Tester<size_t> t;
	std::mt19937 my_rand(24707 + size);

	bool seq = flags & SEQ;
	bool erase = !(flags & NO_ERASE);

	for (size_t i = 0; i < size; i++)
		t.insert(seq ? 2 * i : my_rand() % (3 * size));

	for (size_t i = 0; i < 3 * size + 1; i++)
		t.find(i);

	for (size_t i = 0; i < 30 * size; i++)
		switch (my_rand() % 5)
		{
		case 1:
			t.insert(my_rand() % (3 * size));
			break;
		case 2:
			if (erase)
				t.erase(my_rand() % (3 * size));
			break;
		default:
			t.find(my_rand() % (3 * size));
		}
}

int main()
{
	try
	{
		std::cout << "Insert test..." << std::endl;
		test_insert();

		std::cout << "Erase test..." << std::endl;
		test_erase();

		std::cout << "Tiny random test..." << std::endl;
		test_random(20);

		std::cout << "Small random test..." << std::endl;
		test_random(200);

		std::cout << "Big random test..." << std::endl;
		test_random(50'000);

		std::cout << "Big sequential test..." << std::endl;
		test_random(50'000, SEQ);

		std::cout << "All tests passed." << std::endl;
	}
	catch (const TestFailed &e)
	{
		std::cout << "Test failed: " << e.what() << std::endl;
	}
}
