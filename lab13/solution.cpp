#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cassert>

struct HashTable
{
	explicit HashTable(size_t buckets)
		: cap_(buckets), table_(new Item *[cap_])
	{
		for (size_t i = 0; i < cap_; i++)
			table_[i] = nullptr;
	}

	HashTable(const HashTable &) = delete;
	HashTable &operator=(const HashTable &) = delete;

	~HashTable()
	{
		for (size_t i = 0; i < cap_; i++)
			delete table_[i];
		delete[] table_;
	}

	bool contains(const std::string &key) const
	{
		size_t h = hash(key, cap_);
		for (Item *i = table_[h]; i; i = i->next_in_bucket)
		{
			if (i->key == key)
				return true;
		}
		return false;
	}

	bool insert(const std::string &key, const std::string &value)
	{
		if (contains(key))
			return false;
		size_t h = hash(key, cap_);
		auto new_item = new Item{key, value, nullptr, nullptr};
		table_[h] = new_item;
		if (last_inserted_)
			last_inserted_->next_inserted = new_item;
		else
			first_inserted_ = table_[h];
		last_inserted_ = new_item;
		return true;
	}

	friend std::ostream &operator<<(std::ostream &out, const HashTable &t)
	{
		out << "{ ";
		for (Item *i = t.first_inserted_; i; i = i->next_inserted)
		{
			out << i->key << ": " << i->val;
			if (i->next_inserted)
				out << ", ";
		}
		return out << " }";
	}

private:
	static size_t hash(const std::string &key, size_t m)
	{
		size_t ret = 1241;
		for (unsigned char c : key)
			ret += (ret << 5) + c;
		return ret % m;
	}

	struct Item
	{
		~Item()
		{
			delete next_in_bucket;
		}

		std::string key, val;
		Item *next_in_bucket;
		Item *next_inserted;
	};

	size_t cap_;
	Item **table_;
	Item *first_inserted_ = nullptr;
	Item *last_inserted_ = nullptr;
};

int main()
{
	HashTable t(30);
	assert(t.insert("PA1", "ok"));
	assert(t.insert("PA2", "in progress"));
	assert(t.insert("AG1", "todo"));
	assert(!t.insert("AG1", "done"));

	std::ostringstream s;
	s << t;
	assert(s.str() == "{ PA1: ok, PA2: in progress, AG1: todo }");

	return 0;
}
