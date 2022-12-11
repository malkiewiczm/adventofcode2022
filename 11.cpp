//#define PARTONE
#include "template.hpp"

static I64 global_mod = 0;

struct Expr {
	bool is_old;
	I64 value;
	I64 value_or_old(I64 old) const;
};

struct AddOrMul {
	bool is_add;
	Expr lhs;
	Expr rhs;
	I64 apply(I64 old) const;
};

struct Monkey {
	std::deque<int> list;
	AddOrMul formula;
	int mod;
	I64 activity;
	int on_true;
	int on_false;
	bool step(Monkey *others);
	void step_all(Monkey *others);
};

static inline std::ostream &operator<<(std::ostream &o, const Expr &e)
{
	if (e.is_old) {
		o << "old";
	} else {
		o << e.value;
	}
	return o;
}


static inline std::ostream &operator<<(std::ostream &o, const AddOrMul &formula)
{
	if (formula.is_add) {
		o << formula.lhs << " + " << formula.rhs;
	} else {
		o << formula.lhs << " * " << formula.rhs;
	}
	return o;
}

static inline std::ostream &operator<<(std::ostream &o, const Monkey &m)
{
	o << m.formula << ", mod " << m.mod << ", true " << m.on_true << ", false " << m.on_false;
	o << ", list " << std::vector<int>(ALL(m.list));
	return o;
}

I64 Expr::value_or_old(I64 old) const
{
	if (is_old)
		return old;
	return value;
}

I64 AddOrMul::apply(I64 old) const
{
	I64 a = lhs.value_or_old(old);
	I64 b = rhs.value_or_old(old);
#ifdef PARTONE
	if (is_add)
		return (a + b);
	else
		return (a * b);
#else
	a %= global_mod;
	b %= global_mod;
	if (is_add)
		return (a + b) % global_mod;
	else
		return (a * b) % global_mod;
#endif
}

bool Monkey::step(Monkey *const others)
{
	if (list.empty()) {
		return false;
	}
	int item = list.front();
	list.pop_front();
	item = formula.apply(item);
#ifdef PARTONE
	item /= 3;
#endif
	if (item % mod == 0) {
		others[on_true].list.push_back(item);
	} else {
		others[on_false].list.push_back(item);
	}
	++activity;
	return true;
}

void Monkey::step_all(Monkey *const others)
{
	while (step(others)) {}
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<Monkey> monkeys;
	{
		std::string line;
		while (std::getline(f, line)) {
			std::istringstream ss(line);
			std::string token;
			ss >> token;
			if (token == "Monkey") {
				Monkey m;
				m.activity = 0;
				monkeys.push_back(m);
			} else if (token == "Starting") {
				std::string chunk;
				ss >> chunk;
				while (ss >> chunk) {
					monkeys.back().list.push_back(std::atoi(chunk.c_str()));
				}
			} else if (token == "Operation:") {
				std::string chunk;
				ss >> chunk;
				ss >> chunk;
				ss >> chunk;
				Expr &lhs = monkeys.back().formula.lhs;
				Expr &rhs = monkeys.back().formula.rhs;
				if (chunk == "old") {
					lhs.is_old = true;
				} else {
					lhs.is_old = false;
					lhs.value = std::atoi(chunk.c_str());
				}
				ss >> chunk;
				monkeys.back().formula.is_add = (chunk == "+");
				ss >> chunk;
				if (chunk == "old") {
					rhs.is_old = true;
				} else {
					rhs.is_old = false;
					rhs.value = std::atoi(chunk.c_str());
				}
			} else if (token == "Test:") {
				std::string chunk;
				ss >> chunk;
				ss >> chunk;
				ss >> monkeys.back().mod;
			} else if (token == "If") {
				std::string chunk;
				ss >> chunk;
				int &on =  (chunk == "true:") ? monkeys.back().on_true :  monkeys.back().on_false;
				ss >> chunk;
				ss >> chunk;
				ss >> chunk;
				ss >> on;
			}
		}
	}
	global_mod = 1;
#ifdef PARTONE
	constexpr int to = 20;
#else
	FOR(i, SIZE(monkeys)) {
		global_mod *= monkeys[i].mod;
	}
	constexpr int to = 10000;
#endif
	FOR(round, to) {
		FOR(i, SIZE(monkeys)) {
			monkeys[i].step_all(&monkeys[0]);
		}
	}
	std::sort(ALL(monkeys), [](const Monkey &lhs,  const Monkey &rhs) { return lhs.activity > rhs.activity; });
	const I64 ans = monkeys[0].activity * monkeys[1].activity;
	trace(ans);
}
