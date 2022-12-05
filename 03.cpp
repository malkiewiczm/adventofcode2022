//#define PARTONE
#include "template.hpp"

static int priority(char c)
{
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 1;
	return c - 'A' + 27;
}

#ifndef PARTONE

static int unique_in_three(const std::string &x0, const std::string &x1, const std::string &x2)
{
	std::set<char> c0(x0.begin(), x0.end());
	std::set<char> c1(x1.begin(), x1.end());
	for (const auto &c : x2) {
		if (CONTAINS(c0, c) && CONTAINS(c1, c)) {
			return priority(c);
		}
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
#ifdef PARTONE
	std::string line;
	int sum = 0;
	while (std::getline(f, line)) {
		const int half = line.size() / 2;
		std::set<char> c0;
		std::set<char> c1;
		FOR(i, line.size()) {
			if (i < half) {
				c0.emplace(line[i]);
			} else {
				c1.emplace(line[i]);
			}
		}
		for (const auto c : c0) {
			if (CONTAINS(c1, c)) {
				sum += priority(c);
			}
		}
	}
	trace(sum);
#else
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(f, line)) {
		lines.push_back(line);
	}
	int sum = 0;
	for (int i = 0; i < static_cast<int>(lines.size()); i += 3) {
		sum += unique_in_three(lines[i], lines[i + 1], lines[i + 2]);
	}
	trace(sum);
#endif
}
