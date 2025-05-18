#include <algorithm>
#include <cassert>
#include <compare>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#define LVL 2

struct Set
{
	using T = int;

private:
	auto find_ (const T & value) const {
		auto found = std::lower_bound(data_.begin(), data_.end(), value);
		return std::pair{found != data_.end() && *found == value, found};
	}

	// TODO
public:
	bool empty() const { return data_.empty(); }

	size_t size() const { return data_.size(); }

	bool insert(const T &value)
	{
		if (contains(value))
			return false;

		auto found = std::lower_bound(data_.begin(), data_.end(), value);
		data_.insert(found, value);
		return true;
	}

	bool contains(const T &value) const
	{
		// return std::binary_search(data_.begin(), data_.end(), value);
	}

private:
	std::vector<T> data_;
};

#define DOES_NOT_COMPILE(...) assert(!requires { __VA_ARGS__; });

int main()
{
	Set A;
	(void)A;

	// empty, size, insert
#if LVL >= 1
	assert(A.empty());
	bool s = A.insert(5); // true, pokud prvek byl vložen
	assert(s);
	s = A.insert(5); // false, pokud už v množině byl
	assert(!s);
	assert(A.size() == 1);
	s = A.insert(42);
	assert(s);
	assert(A.size() == 2);
	assert(!std::as_const(A).empty());
#endif

	// contains
#if LVL >= 2
	assert(A.contains(5));
	assert(A.contains(42));
	assert(!A.contains(43));
	assert(!A.contains(6));
	assert(!A.contains(4));
#endif

	// remove
#if LVL >= 3
	for (int i = 0; i < 10; i++)
		A.insert(2 * i);
	for (int i = 0; i < 10; i++)
		assert(A.contains(2 * i));
	for (int i = 3; i < 10; i++)
	{
		s = A.remove(2 * i); // true, pokud byl prvek smazán (tedy byl v množině)
		assert(s);
	}
	for (int i = 0; i < 10; i++)
		assert(A.contains(2 * i) == (i < 3));
#endif

	// Rychlostní test (contains)
#if LVL >= 4
	{
		Set B;
		for (int i = 0; i < 10'000; i++)
			B.insert(31 * i % 10'000);

		for (int i = 0; i < 1'000'000; i++)
			assert(B.contains(i) == (i < 10'000));

		for (int i = 30; i < 10'000; i += 100)
		{
			bool s = B.remove(i);
			assert(s);
		}

		for (int i = 0; i < 1'000'000; i++)
			assert(B.contains(i) == (i < 10'000 && i % 100 != 30));
	}
#endif

	// výpis oprátorem <<
	// prvky jsou vypisovány od nejmenšího
#if LVL >= 5
	{
		std::ostringstream out;
		out << A;
		assert(out.str() == "{ 0, 2, 4, 5, 42 }");
	}
	std::cout << A << std::endl;
#endif

	// operátor [] pro přístup k prvkům
	// stejně jako výpis indexuje od nejmenších prvků
#if LVL >= 6
	assert(A[0] == 0);
	assert(A[4] == 42);
	assert(std::as_const(A)[4] == 42);

	try
	{
		A[A.size()];
		assert(0);
	}
	catch (const std::out_of_range &)
	{ /* OK */
	}

	try
	{
		A[233];
		assert(0);
	}
	catch (const std::out_of_range &)
	{ /* OK */
	}

	[](auto &A)
	{
		DOES_NOT_COMPILE(A[1] = 5);
	}(A);
#endif

	// operátor bool
#if LVL >= 7
	{
		Set B;
		assert(!B);
		B.insert(0);
		assert(B);
		B.remove(1);
		assert(std::as_const(B));
		B.remove(0);
		assert(!B);
	}
#endif

	// operátory - a -= (rozdíl)
#if LVL >= 8
	out.str("");
	out << (A - B);
	std::cout << out.str() << std::endl;
	assert(out.str() == "{ 0, 2, 4, 42 }");

	A -= B;
	out.str("");
	out << A;
	assert(out.str() == "{ 0, 2, 4, 42 }");
#endif

	// operátory & a &= (průnik)
#if LVL >= 9
	out.str("");
	out << (B & A);
	std::cout << out.str() << std::endl;
	assert(out.str() == "{ 5 }");

	out.str("");
	out << (C &= B);
	std::cout << out.str() << std::endl;
	assert(out.str() == "{ 3, 5, 17, 24 }");
#endif

	// operátory | a |= (sjednocení)
#if LVL >= 10
	Set B;
	for (int x : {3, 5, 17, 24})
		B.insert(x);
	std::ostringstream out;
	out << (A | B);
	std::cout << out.str() << std::endl;
	assert(out.str() == "{ 0, 2, 3, 4, 5, 17, 24, 42 }");

	Set C;
	C.insert(15);
	C |= B;
	out.str("");
	out << C;
	std::cout << out.str() << std::endl;
	assert(out.str() == "{ 3, 5, 15, 17, 24 }");
#endif

	// operátory porovnání
#if LVL >= 11
	assert(A < B);
	assert(B >= A);
	assert(A != B);
	assert(A == A);
	assert(!(A == B));
	assert(!(A > B));
	assert(!(A >= B));
	assert(A <= B);
	assert((A <=> B) < 0);
#endif

	// bonus: iterace pomocí range-for
	// jsou potřeba metody begin a end
#if LVL >= 12
	for (auto x : A)
		std::cout << x << std::endl;
#endif

	std::cout << "Successfully completed level " << LVL << ".\n";
}
