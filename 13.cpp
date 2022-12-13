//#define PARTONE
#include "template.hpp"

struct Expr {
	bool is_list;
	int value;
	std::vector<Expr*> list;
};

static inline std::ostream &operator<<(std::ostream &o, const Expr *expr)
{
	if (expr->is_list) {
		o << "[";
		FOR(i, SIZE(expr->list)) {
			o << " " << expr->list[i];
		}
		o << " ]";
	} else {
		o << expr->value;
	}
	return o;
}

static std::string streamify(const std::string &s)
{
	std::ostringstream ss;
	for (const auto &c : s) {
		if (c >= '0' && c <= '9') {
			ss << c;
		} else if (c == ',') {
			ss << " ";
		} else {
			ss << " " << c << " ";
		}
	}
	return ss.str();
}

static Expr* read_expr(std::istream &ss)
{
	Expr *e = new Expr;
	std::string chunk;
	ss >> chunk;
	if (chunk == "[") {
		e->is_list = true;
		for ( ; ; ) {
			Expr *next_expr = read_expr(ss);
			if (next_expr == nullptr) {
				break;
			}
			e->list.push_back(next_expr);
		}
	} else if (chunk == "]") {
		return nullptr;
	} else {
		e->is_list = false;
		e->value = std::atoi(chunk.c_str());
	}
	return e;
}

static int cmp(Expr *lhs, Expr *rhs)
{
	if (!lhs->is_list && !rhs->is_list) {
		return lhs->value - rhs->value;
	}
	if (lhs->is_list && rhs->is_list) {
		const int lhs_size = SIZE(lhs->list);
		const int rhs_size = SIZE(rhs->list);
		const int to = std::min(lhs_size, rhs_size);
		FOR(i, to) {
			const int diff = cmp(lhs->list[i], rhs->list[i]);
			if (diff != 0) {
				return diff;
			}
		}
		return lhs_size - rhs_size;
	}
	if (lhs->is_list) {
		auto temp = std::make_unique<Expr>();
		temp->is_list = true;
		temp->list.push_back(rhs);
		return cmp(lhs, temp.get());
	}
	if (rhs->is_list) {
		auto temp = std::make_unique<Expr>();
		temp->is_list = true;
		temp->list.push_back(lhs);
		return cmp(temp.get(), rhs);
	}
	return 0;
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<Expr*> exprs;
	std::string line;
	while (std::getline(f, line)) {
		if (line.empty()) {
			continue;
		}
		std::istringstream ss(streamify(line));
		exprs.push_back(read_expr(ss));
	}
#ifdef PARTONE
	int ans = 0;
	FOR3(i, 1, SIZE(exprs), 2) {
		const int res = cmp(exprs[i - 1], exprs[i]);
		if (res < 0) {
			ans += (i / 2) + 1;
		}
	}
	trace(ans);
#else
	line = "[[2]]";
	Expr *divider0 = nullptr;
	{
		std::istringstream ss(streamify(line));
		divider0 = read_expr(ss);
	}
	line = "[[6]]";
	Expr *divider1 = nullptr;
	{
		std::istringstream ss(streamify(line));
		divider1 = read_expr(ss);
	}
	exprs.push_back(divider0);
	exprs.push_back(divider1);
	int ans = 1;
	std::sort(ALL(exprs), [](Expr *lhs, Expr *rhs) { return cmp(lhs, rhs) < 0; });
	FOR(i, SIZE(exprs)) {
		if (exprs[i] == divider0 || exprs[i] == divider1) {
			ans *= i + 1;
		}
	}
	trace(ans);
#endif
}
