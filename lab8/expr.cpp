#include <iostream>
#include <string>
#include <utility>
#include <map>
#include <set>
#include <memory>
#include <sstream>
#include <cassert>
#include <iomanip>

#define LEVEL 3

using Environment = std::map<std::string, bool>;

struct Expression
{
	virtual bool eval(const Environment &env) const = 0;

	virtual std::shared_ptr<Expression> clone() const = 0;

	// nemusi friend
	friend std::ostream &operator<<(std::ostream &oss, const Expression &exp)
	{
		exp.print(oss);
		return oss;
	}

private:
	virtual void print(std::ostream &oss) const = 0;
};

struct Literal : public Expression
{ // explicit
	explicit Literal(bool value_) : value(value_) {}

	bool eval(const Environment &) const override
	{
		return value;
	}

	std::shared_ptr<Expression> clone() const override
	{
		return std::make_shared<Literal>(*this);
	}

private:
	bool value;
	void print(std::ostream &oss) const override
	{
		oss << value;
	}
};

struct Variable : public Expression
{ // explicit
	explicit Variable(const std::string &value_) : value(value_) {}

	bool eval(const Environment &env) const override
	{
		return env.at(value);
	}

	std::shared_ptr<Expression> clone() const override
	{
		return std::make_shared<Variable>(*this);
	}

private:
	std::string value;
	void print(std::ostream &oss) const override
	{
		oss << value;
	}
};

struct And : public Expression
{
	And(const Expression &lhs, const Expression &rhs) : lhs(lhs.clone()), rhs(rhs.clone())
	{
	}

	bool eval(const Environment &env) const override
	{
		return lhs->eval(env) && rhs->eval(env);
	}

	std::shared_ptr<Expression> clone() const override
	{
		return std::make_shared<And>(*this);
	}

private:
	std::shared_ptr<Expression> lhs;
	std::shared_ptr<Expression> rhs;
	void print(std::ostream &oss) const override
	{
		oss << "(" << *lhs << " && " << *rhs << ")";
	}
};

struct Or : public Expression
{
	Or(const Expression &lhs, const Expression &rhs) : lhs(lhs.clone()), rhs(rhs.clone())
	{
	}

	bool eval(const Environment &env) const override
	{
		return lhs->eval(env) || rhs->eval(env);
	}

	std::shared_ptr<Expression> clone() const override
	{
		return std::make_shared<Or>(*this);
	}

private:
	std::shared_ptr<Expression> lhs;
	std::shared_ptr<Expression> rhs;
	void print(std::ostream &oss) const override
	{
		oss << "(" << *lhs << " || " << *rhs << ")";
	}
};

struct Not : public Expression
{
	explicit Not(const Expression &val_) : val(val_.clone())
	{
	}

	bool eval(const Environment &env) const override
	{
		return !val->eval(env);
	}

	std::shared_ptr<Expression> clone() const override
	{
		return std::make_shared<Not>(*this);
	}

private:
	std::shared_ptr<Expression> val;
	void print(std::ostream &oss) const override
	{
		oss << "!" << *val;
	}
};

#if LEVEL >= 4
/**
 * Evaluate the expression for all possible values of variables and print the result
 * @param expr Expression to evaluate
 */
void evaluate(const Expression &expr)
{
	auto vars = expr.get_vars();

	std::ostringstream oss;
	for (const auto &var : vars)
	{
		oss << "| " << var << " ";
	}
	oss << "| " << expr << " |";

	int maxWidth = static_cast<int>(oss.str().size());
	std::cout << std::setw(maxWidth) << std::setfill('-') << "" << std::endl;
	std::cout << oss.str() << std::endl;
	std::cout << std::setw(maxWidth) << std::setfill('-') << "" << std::endl;

	std::cout << std::setfill(' ');
	auto varsComb = 1 << vars.size();
	for (int i = 0; i < varsComb; ++i)
	{
		Environment env;
		int j = 0;
		for (const auto &var : vars)
		{
			env[var] = (i >> j) & 1;
			++j;
		}

		oss.str("");

		for (const auto &var : vars)
		{
			oss << "| " << env[var] << " ";
		}
		oss << "| ";
		std::cout << oss.str();
		int width = static_cast<int>(oss.str().size());
		int fill = std::max(maxWidth - width - 2, 0);

		std::cout << std::setw(fill) << std::right << expr.eval(env) << " |" << std::endl;
	}
	std::cout << std::setw(maxWidth) << std::setfill('-') << "" << std::endl;
}
#endif

int main()
{
#if LEVEL >= 1
	// Level 1: Bez proměných
	{
		const Environment empty;
		assert(Literal(true).eval(empty) == true);
		assert(Literal(false).eval(empty) == false);
		assert(And(Literal(true), Literal(true)).eval(empty) == true);
		assert(And(Literal(true), Literal(false)).eval(empty) == false);
		assert(And(Literal(false), Literal(true)).eval(empty) == false);
		assert(And(Literal(false), Literal(false)).eval(empty) == false);
		assert(Or(Literal(true), Literal(true)).eval(empty) == true);
		assert(Or(Literal(true), Literal(false)).eval(empty) == true);
		assert(Or(Literal(false), Literal(true)).eval(empty) == true);
		assert(Or(Literal(false), Literal(false)).eval(empty) == false);
		assert(Not(Literal(true)).eval(empty) == false);
		assert(Not(Literal(false)).eval(empty) == true);
	}

	std::cout << "Level 1 OK" << std::endl;
#endif

#if LEVEL >= 2
	// Level 2: Proměnné
	{
		Variable a("a");
		Variable b("b");

		auto expr = And(a, b);

		Environment env = {
			{"a", true},
			{"b", true}};
		assert(expr.eval(env) == true);
		env["b"] = false;
		assert(expr.eval(env) == false);

		auto expr2 = Or(Not(a), b);

		env = {
			{"a", true},
			{"b", true}};
		assert(expr2.eval(env) == true);
		env["b"] = false;
		assert(expr2.eval(env) == false);
		env["a"] = false;
		assert(expr2.eval(env) == true);
	}

	std::cout << "Level 2 OK" << std::endl;
#endif

#if LEVEL >= 3
	{
#define TEST_PRINT(expr, expected)                              \
	do                                                          \
	{                                                           \
		std::ostringstream oss;                                 \
		oss << expr;                                            \
		if (oss.str() != expected)                              \
		{                                                       \
			std::cout << "Expected: " << expected << std::endl; \
			std::cout << "Got: " << oss.str() << std::endl;     \
			assert(0);                                          \
		}                                                       \
	} while (0)

		// Level 3: Print
		TEST_PRINT(Literal(true), "1");
		TEST_PRINT(Literal(false), "0");
		TEST_PRINT(And(Variable("a"), Variable("b")), "(a && b)");
		TEST_PRINT(Or(Variable("a"), Variable("b")), "(a || b)");
		TEST_PRINT(Not(Variable("a")), "!a");
		TEST_PRINT(Not(And(Variable("a"), Variable("b"))), "!(a && b)");
		TEST_PRINT(Or(Not(Variable("a")), Variable("b")), "(!a || b)");
		TEST_PRINT(Or(Variable("a"), Not(Variable("b"))), "(a || !b)");
		TEST_PRINT(Or(And(Variable("a"), Literal(false)), Variable("b")), "((a && 0) || b)");
	}
	std::cout << "Level 3 OK" << std::endl;
#endif

#if LEVEL >= 4
	{
		auto expr = Or(
			And(
				Variable("a"),
				Variable("b")),
			Impl(
				Or(
					Variable("a"),
					Variable("b")),
				Variable("b")));

		evaluate(expr);
	}
	std::cout << "Level 4 OK" << std::endl;
#endif

#if LEVEL >= 5
	// test rychlosti
	{
		auto expr = Or(Variable("c"), Variable("a"));
		for (int i = 0; i < 10'000; i++)
			expr = Or(Not(Variable("b")), std::move(expr));

		const Environment env = {{"a", 1}, {"b", 1}, {"c", 0}};
		assert(expr.eval(env) == true);

		auto expr2 = And(expr, Not(expr));
		assert(expr2.eval(env) == false);
	}
#endif
}
