#include <cassert>
#include <cstdlib>

#define lli long long int

lli fibImpl(lli n, lli * memo)
{
	if(memo[n] > 0) return memo[n];
	if(n < 2) return memo[n] = n;
	return memo[n] = fibImpl(n - 2, memo) + fibImpl(n - 1, memo);
}

lli fib(lli n)
{
	if(n < 0) return 0;
	auto memo = new lli[n + 1]();
	auto result = fibImpl(n, memo);
	delete[] memo;
	return result;
}

int main()
{
	assert(fib(0) == 0LL);
	assert(fib(1) == 1LL);
	assert(fib(5) == 5LL);
	assert(fib(10) == 55LL);
	assert(fib(50) == 12586269025LL);

	return EXIT_SUCCESS;
}