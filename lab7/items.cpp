#include <iostream>
#include <stdexcept>
#include <memory>
#include <string>
#include <list>
#include <cassert>
#include <sstream>
#include <vector>

#define LVL 1

struct Item
{
	virtual ~Item() = default;

	virtual size_t weight() const = 0;


	virtual void print(std::ostream& out) const = 0;


	friend std::ostream &operator<<(std::ostream &out, const Item &item)
	{
		// TODO
		item.print(out);
		return out;
	}
};

struct SimpleItem : Item
{
	static std::shared_ptr<SimpleItem> create(const std::string &name, size_t weight)
	{
		return std::shared_ptr<SimpleItem>{new SimpleItem(name, weight)};
		// Nebo
		// return std::make_shared<SimpleItem>(name, weight);
	}

	friend std::ostream &operator<<(std::ostream &out, const SimpleItem &item)
	{
		// TODO
		out << item.m_name << "(" << item.m_weight << ")";
		return out;
	}

	size_t weight() const override
	{
		return m_weight;
	}


	void print (std:: ostream & out) const override
	{
		out << m_name ;
	}
private:
	SimpleItem(const std::string &name, size_t weight) : m_name(name), m_weight(weight)
	{
		// TODO
	}

	std::string m_name;
	size_t m_weight;
};

// Bag sám o sobě váží 0
struct Bag : Item
{

	static std::shared_ptr<Bag> create(const std::string &name, size_t max_items, size_t max_weight)
	{
		// TODO
		return std::shared_ptr<Bag>{new Bag(name, max_items, max_weight)};
	}

	size_t weight() const override
	{
		size_t total = 0;
		for (auto &item : items)
		{
			total += item->weight();
		}
		return total;
	}

	friend void print(std::ostream &out) override
	{
		// TODO
		out << item.m_name << "bag of " << item.items.size() << "items (" << item.weight() << ")";

		for (const auto &it : item.items)
		{
			out << "\n  " << *it;
		}
		return out;
	}

	void add(std::shared_ptr<SimpleItem> item)
	{
		items.push_back(item);
	}

private:
	Bag(const std::string &name, size_t max_items, size_t max_weight) : m_name(name), m_max_items(max_items), m_max_weight() {}

	std::vector<std::shared_ptr<SimpleItem>> items;

	std::string m_name;
	size_t m_max_items;
	size_t m_max_weight;
};

#define TRY_CATCH(expr, exception, msg) \
	try                                 \
	{                                   \
		expr;                           \
		assert(false);                  \
	}                                   \
	catch (const exception &e)          \
	{                                   \
		assert(e.what() == msg);        \
	}

int main()
{
	using std::operator""s;

	auto batoh = Bag::create("Batoh", 15, 5'000); // Jméno, max počet věcí, max hmotnost
	assert(batoh->weight() == 0);
	batoh->add(SimpleItem::create("C++ v kostce", 500)); // Jméno, hmotnost
	batoh->add(SimpleItem::create("Termoska", 1'500));
	assert(batoh->weight() == 2000);

	std::cout << *batoh << std::endl;
	// Vypíše:
	// Batoh: bag of 2 items (2000):
	//   C++ v kostce (500)
	//   Termoska (1500)

	std::ostringstream oss;
	oss << *batoh << std::endl;
	assert(oss.str() == "Batoh: bag of 2 items (2000):\n"
						"  C++ v kostce (500)\n"
						"  Termoska (1500)\n");

#if LVL >= 2
	// Umíme vložit Bag do Bag a hezky vypsat

	std::cout << "Level 2" << std::endl;
	auto penezenka = Bag::create("Peněženka", 5, 100);
	batoh->add(penezenka);
	penezenka->add(SimpleItem::create("Stokoruna", 3));
	penezenka->add(SimpleItem::create("Stokoruna", 3));
	penezenka->add(SimpleItem::create("Pětikoruna", 15));
	penezenka->add(SimpleItem::create("Pětikoruna", 15));
	auto petikoruna = SimpleItem::create("Pětikoruna", 15);
	penezenka->add(petikoruna);

	assert(penezenka->weight() == 51);
	assert(batoh->weight() == 2051);

	std::cout << *batoh << std::endl;
	// Vypíše:
	// Batoh: bag of 3 items (2051):
	//   C++ v kostce (500)
	//   Termoska (1500)
	//   Peněženka: bag of 5 items (51):
	//     Stokoruna (3)
	//     Stokoruna (3)
	//     Pětikoruna (15)
	//     Pětikoruna (15)
	//     Pětikoruna (15)

	oss.clear();
	oss.str("");
	oss << *batoh << std::endl;
	assert(oss.str() == "Batoh: bag of 3 items (2051):\n"
						"  C++ v kostce (500)\n"
						"  Termoska (1500)\n"
						"  Peněženka: bag of 5 items (51):\n"
						"    Stokoruna (3)\n"
						"    Stokoruna (3)\n"
						"    Pětikoruna (15)\n"
						"    Pětikoruna (15)\n"
						"    Pětikoruna (15)\n");
#endif

#if LVL >= 3
	// Výjimka pokud bychom přidali příliš mnoho věcí nebo batoh
	// přetížili
	std::cout << "Level 3" << std::endl;
	TRY_CATCH(
		penezenka->add(SimpleItem::create("Fotka", 2)),
		std::overflow_error, "Too many items"s)

	TRY_CATCH(
		batoh->add(SimpleItem::create("Zlatá cihla", 2'950)),
		std::overflow_error, "Too heavy"s)

	// Stejný výpis jako v levelu 2
	std::cout << *batoh << std::endl;
#endif

#if LVL >= 4
	// Magické věci mají hmotnost 0 a pokud něco obsahuje
	// magickou věc, je to také magické
	std::cout << "Level 4" << std::endl;
	assert(!batoh->is_magical());
	batoh->add(MagicalItem::create("Kouzelnická hůlka"));
	assert(batoh->is_magical());
#endif

#if LVL >= 5
	// MagicalBag je Bag, který je magický, takže se do něj vejde
	// libovolné množství věcí a nikdy nic neváží
	std::cout << "Level 5" << std::endl;
	{
		auto magicky_pytlik = MagicalBag::create("Magický pytlík");
		assert(magicky_pytlik->weight() == 0);
		magicky_pytlik->add(batoh);
		std::cout << *magicky_pytlik << std::endl;
		assert(magicky_pytlik->weight() == 0);
		assert(magicky_pytlik->is_magical());
	}
#endif

#if LVL >= 6
	// Item může být v jednu chvíli jen v jednom jiném Bagu
	// Přímo:
	std::cout << "Level 6" << std::endl;
	TRY_CATCH(
		batoh->add(penezenka),
		std::logic_error, "Already in container"s)
#endif

#if LVL >= 7
	// Nepřímo:
	std::cout << "Level 7" << std::endl;
	TRY_CATCH(
		batoh->add(petikoruna),
		std::logic_error, "Already in container"s)
#endif

#if LVL >= 8
	// Po zániku vnějšího Bagu, lze objekt vložit do jiného Bagu
	std::cout << "Level 8" << std::endl;
	for (int i = 0; i < 2; i++)
	{
		auto pytel = Bag::create("Pytel", 100, 100'000);
		pytel->add(batoh);
		std::cout << *pytel << std::endl;
	}
#endif

#if LVL >= 9
	// Bag nemůže být sám v sobě
	std::cout << "Level 9" << std::endl;
	TRY_CATCH(
		batoh->add(batoh),
		std::logic_error, "Cannot put Bag into itself"s)
#endif

#if LVL >= 99
	// A to ani nepřímo
	std::cout << "Level 99" << std::endl;
	auto pytel = Bag::create("Pytel 1", 100, 100'000);
	pytel->add(batoh);

	for (int i = 2; i < 20; i++)
	{
		char buf[100];
		sprintf(buf, "Pytel %d", i);
		auto dalsi_pytel = Bag::create(buf, 100, 100'000);
		dalsi_pytel->add(pytel);
		pytel = dalsi_pytel;
	}

	TRY_CATCH(
		batoh->add(pytel),
		std::logic_error, "Cannot put Bag into itself"s)
#endif

	std::cout << "Level " << LVL << " done" << std::endl;
}
