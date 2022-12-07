//#define PARTONE
#include "template.hpp"

using Stack = std::deque<char>;

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<std::string> ins;
	{
		std::string line;
		while (std::getline(f, line)) {
			ins.push_back(line);
		}
	}
	const int num_rows = SIZE(ins);
	const int num_crates = (SIZE(ins[0]) + 1) / 4;
	std::vector<Stack> stacks(num_crates);
	int row = 0;
	for ( ; row < num_rows; ++row) {
		std::string &s = ins[row];
		if (!(s[0] == '[' || (s[0] == ' ' && s[1] == ' '))) {
			break;
		}
		FOR(i, num_crates) {
			const int index = i * 4 + 1;
			const char c = s[index];
			if (c != ' ') {
				stacks[i].push_front(c);
			}
		}
	}
	// skip two garbage rows
	row += 2;
#ifdef PARTONE
	for (; row < num_rows; ++row) {
		std::istringstream ss(ins[row]);
		std::string dummy[3];
		int amt, from, to;
		ss >> dummy[0] >> amt >> dummy[1] >> from >> dummy[2] >> to;
		--from;
		--to;
		FOR(i, amt) {
			const char c = stacks[from].back();
			stacks[from].pop_back();
			stacks[to].push_back(c);
		}
	}
#else
	for (; row < num_rows; ++row) {
		std::istringstream ss(ins[row]);
		std::string dummy[3];
		int amt, from, to;
		ss >> dummy[0] >> amt >> dummy[1] >> from >> dummy[2] >> to;
		--from;
		--to;
		std::vector<char> buf;
		FOR(i, amt) {
			const char c = stacks[from].back();
			stacks[from].pop_back();
			buf.push_back(c);
		}
		for (int i = SIZE(buf) - 1; i >= 0; --i) {
			const char c = buf[i];
			stacks[to].push_back(c);
		}
	}
#endif
	FOR(i, num_crates) {
		std::cout << stacks[i].back();
	}
	std::cout << std::endl;
}
