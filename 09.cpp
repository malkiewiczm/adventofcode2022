//#define PARTONE
#include "template.hpp"

using Pos = std::tuple<I64, I64>;

static inline std::ostream &operator<<(std::ostream &o, const Pos &p)
{
	o << std::make_pair(std::get<0>(p), std::get<1>(p));
	return o;
}

static int sign(I64 val)
{
	if (val > 0)
		return 1;
	if (val < 0)
		return -1;
	return 0;
}

static bool is_touch(const Pos &a, const Pos &b)
{
	const I64 d0 = std::abs(std::get<0>(a) - std::get<0>(b));
	const I64 d1 = std::abs(std::get<1>(a) - std::get<1>(b));
	return d0 <= 1 && d1 <= 1;
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::string dir;
	int amt;
#ifdef PARTONE
	Pos head = std::make_tuple(0, 0);
	Pos tail = std::make_tuple(0, 0);
	std::set<Pos> visited;
	visited.emplace(0, 0);
	while (f >> dir >> amt) {
		FOR(i, amt) {
			switch(dir[0]) {
			case 'U':
				std::get<0>(head) -= 1;
				break;
			case 'D':
				std::get<0>(head) += 1;
				break;
			case 'L':
				std::get<1>(head) -= 1;
				break;
			case 'R':
				std::get<1>(head) += 1;
				break;
			}
			// tail rules
			if (! is_touch(head, tail)) {
				const Pos delta = std::make_tuple(std::get<0>(head) - std::get<0>(tail), std::get<1>(head) - std::get<1>(tail));
				std::get<0>(tail) += sign(std::get<0>(delta));
				std::get<1>(tail) += sign(std::get<1>(delta));
			}
			visited.emplace(tail);
		}
	}
	trace(SIZE(visited));
#else
	Pos knots[10];
	std::set<Pos> visited;
	visited.emplace(0, 0);
	while (f >> dir >> amt) {
		FOR(i, amt) {
			switch(dir[0]) {
			case 'U':
				std::get<0>(knots[0]) -= 1;
				break;
			case 'D':
				std::get<0>(knots[0]) += 1;
				break;
			case 'L':
				std::get<1>(knots[0]) -= 1;
				break;
			case 'R':
				std::get<1>(knots[0]) += 1;
				break;
			}
			// tail rules
			FOR2(j, 1, 10) {
				Pos &head = knots[j - 1];
				Pos &tail = knots[j];
				if (! is_touch(head, tail)) {
					const Pos delta = std::make_tuple(std::get<0>(head) - std::get<0>(tail), std::get<1>(head) - std::get<1>(tail));
					std::get<0>(tail) += sign(std::get<0>(delta));
					std::get<1>(tail) += sign(std::get<1>(delta));
				}
			}
			visited.emplace(knots[9]);
		}
	}
	trace(SIZE(visited));
#endif
}
