#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <sstream>
#include <cassert>
#include <iomanip>
#include <map>
#include <set>
#include <algorithm>
#include <deque>

#define LEVEL 5

using Id = std::string;

enum class ValidRelationships
{
	ALL,
	PATERNAL,
	MATERNAL
};

class SnailRegistry
{
public:
	bool AddSnail(const Id &id, const Id &father_id, const Id &mother_id)
	{
		if (id == father_id || id == mother_id || father_id == mother_id)
			return false;
		if (data[id].parents_known)
			return false;

		data[father_id].father_of.push_back(id);
		data[mother_id].mother_of.push_back(id);

		data[id].parents_known = true;
		return true;
	}
	std::vector<Id> Descendants(const Id &id, const ValidRelationships rel) const
	{
		if (!data.contains(id))
			return {};

		std::set<Id> seen = {};
		std::deque<Id> Q = {id};

		auto add_snail = [&](const Id &s)
		{
			auto [_, succ] = seen.insert(s);
			if (succ)
				Q.push_back(s);
		};

		while (!Q.empty())
		{
			auto snail = Q.front();
			Q.pop_front();

			using enum ValidRelationships;
			if (rel != MATERNAL)
				std::ranges::for_each(data.at(snail).father_of, add_snail);
			if (rel != PATERNAL)
				std::ranges::for_each(data.at(snail).mother_of, add_snail);
		}
		return {seen.begin(), seen.end()};
	}

	bool IsInbred(const Id &id) const
	{
		if (!data.contains(id))
			return {};

		std::set<Id> seen = {};
		std::deque<Id> Q = {id};

		bool foubd_inbred = false;
		auto add_snail = [&](const Id &s)
		{
			auto [_, succ] = seen.insert(s);
			if (succ)
				Q.push_back(s);
			else
				foubd_inbred = true; // could be implemented with exception, now iteration to the end, not fully effective
		};

		while (!Q.empty() && !foubd_inbred)
		{
			auto snail = Q.front();
			Q.pop_front();

			std::ranges::for_each(data.at(snail).father_of, add_snail);
			std::ranges::for_each(data.at(snail).mother_of, add_snail);
		}
		return foubd_inbred;
	}

private:
	struct Snail
	{
		bool parents_known = false;
		std::vector<Id> mother_of, father_of;
	};

	std::map<Id, Snail> data;
};

int main()
{
#if LEVEL >= 1
	// Level 1: Tvorba registru
	SnailRegistry registry;
	{
		assert(registry.AddSnail("Cerbere", "Andromalius", "Belphegor"));
		assert(registry.AddSnail("Deimos", "Belphegor", ""));
		assert(registry.AddSnail("Eurynome", "Andromalius", "Deimos"));
		assert(registry.AddSnail("Garuda", "Furfur", "Deimos"));
		assert(registry.AddSnail("Haborym", "Eurynome", "Garuda"));
		assert(!registry.AddSnail("Azazel", "Deimos", "Deimos"));
		assert(!registry.AddSnail("Cerbere", "Abaddon", "Bael"));
		assert(!registry.AddSnail("Cali", "Cali", "Deimos"));
	}

	std::cout << "Level 1 OK" << std::endl;
#endif

#if LEVEL >= 2
	// Level 2: správnost BFS
	auto snail = [](unsigned i)
	{
		return "Snail_" + std::to_string(i);
	};

	{
		SnailRegistry r;
		for (unsigned i = 1; i < 60; i++)
		{
			assert(r.AddSnail(snail(2 * i), snail(2 * i - 1), snail(2 * i - 2)));
			assert(r.AddSnail(snail(2 * i + 1), snail(2 * i - 1), snail(2 * i - 2)));
		}

		auto d = r.Descendants(snail(0), ValidRelationships::ALL);
		assert(d.size() == 118);
	}

	std::cout << "Level 2 OK" << std::endl;
#endif

#if LEVEL >= 3
	// Level 3: Hledání potomků, nezávisí na typu vztahů
	{
		assert(registry.Descendants("Andromalius", ValidRelationships::ALL) == std::vector<Id>({"Cerbere", "Eurynome", "Haborym"}));
		assert(registry.Descendants("Belphegor", ValidRelationships::ALL) == std::vector<Id>({"Cerbere", "Deimos", "Eurynome", "Garuda", "Haborym"}));
		assert(registry.Descendants("Cerbere", ValidRelationships::ALL) == std::vector<Id>({}));
		assert(registry.AddSnail("Ipes", "Garuda", "Cerbere"));
		assert(registry.Descendants("Belphegor", ValidRelationships::ALL) == std::vector<Id>({"Cerbere", "Deimos", "Eurynome", "Garuda", "Haborym", "Ipes"}));
		assert(registry.Descendants("Cerbere", ValidRelationships::ALL) == std::vector<Id>({"Ipes"}));
	}

	std::cout << "Level 3 OK" << std::endl;
#endif

#if LEVEL >= 5
	// Level 5: Nejasné příbuzenské vztahy
	{
		assert(registry.IsInbred("Belphegor"));
		assert(registry.IsInbred("Deimos"));
		assert(!registry.IsInbred("Furfur"));
		assert(!registry.IsInbred("Ipes"));
		assert(registry.AddSnail("Lamia", "Garuda", "Furfur"));
		assert(registry.IsInbred("Furfur"));
	}

	std::cout << "Level 5 OK" << std::endl;
#endif

#if LEVEL >= 4
	// Level 4: Hledání potomků, zajímá nás typy vztahů
	{
		registry.AddSnail("Lamia", "Garuda", "Furfur");
		assert(registry.Descendants("Andromalius", ValidRelationships::PATERNAL) == std::vector<Id>({"Cerbere", "Eurynome", "Haborym"}));
		assert(registry.Descendants("Belphegor", ValidRelationships::PATERNAL) == std::vector<Id>({"Deimos"}));
		assert(registry.Descendants("Deimos", ValidRelationships::MATERNAL) == std::vector<Id>({"Eurynome", "Garuda", "Haborym"}));
		assert(registry.Descendants("Cerbere", ValidRelationships::MATERNAL) == std::vector<Id>({"Ipes"}));
		assert(registry.Descendants("Garuda", ValidRelationships::MATERNAL) == std::vector<Id>({"Haborym"}));
		assert(registry.Descendants("Furfur", ValidRelationships::MATERNAL) == std::vector<Id>({"Lamia"}));
	}
	std::cout << "Level 4 OK" << std::endl;
#endif
}
