//#define PARTONE
#include "template.hpp"

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
	int best = 0;
	int sum = 0;
	while (std::getline(f, line)) {
		if (line.empty()) {
			best = std::max(sum, best);
			sum = 0;
		} else {
			const int num = std::atoi(line.c_str());
			sum += num;
		}
	}
	best = std::max(sum, best);
	trace(best);
#else
	std::string line;
	int sum = 0;
	std::vector<int> ins;
	while (std::getline(f, line)) {
		if (line.empty()) {
			ins.push_back(sum);
			sum = 0;
		} else {
			const int num = std::atoi(line.c_str());
			sum += num;
		}
	}
	ins.push_back(sum);
	std::sort(ins.begin(), ins.end(), std::greater<int>());
	sum = 0;
	FOR(i, 3) {
		sum += ins[i];
	}
	trace(sum);
#endif
}
