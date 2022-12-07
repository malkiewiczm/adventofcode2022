//#define PARTONE
#include "template.hpp"

#ifdef PARTONE

static bool check_four(const std::string &s, int i)
{
	FOR(a, 4) {
		FOR2(b, a + 1, 4) {
			if (s[i + a] == s[i + b])
				return false;
		}
	}
	return true;
}

static int lcount(const std::string &s)
{
	FOR(i, SIZE(s) - 3) {
		if (check_four(s, i))
			return i + 4;
	}
	return 0;
}

#else

static bool check_fourteen(const std::string &s, int i)
{
	std::set<char> u;
	FOR(a, 14) {
		u.emplace(s[i + a]);
	}
	return SIZE(u) == 14;
}

static int lcount(const std::string &s)
{
	FOR(i, SIZE(s) - 13) {
		if (check_fourteen(s, i))
			return i + 14;
	}
	return 0;
}

#endif

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::string line;
	std::getline(f, line);
	trace(lcount(line));
}
