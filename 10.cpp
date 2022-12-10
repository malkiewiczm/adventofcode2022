//#define PARTONE
#include "template.hpp"

struct Cmd {
	int cycles;
	int arg;
};

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<Cmd> cmds;
	std::string mne;
	while (f >> mne) {
		Cmd cmd;
		if (mne == "addx") {
			f >> cmd.arg;
			cmd.cycles = 2;
		} else {
			cmd.arg = 0;
			cmd.cycles = 1;
		}
		cmds.push_back(cmd);
	}
#ifdef PARTONE
	int X = 1;
	int cycle = 0;
	I64 sum = 0;
	for (auto &cmd : cmds) {
		while (cmd.cycles > 0) {
			--cmd.cycles;
			++cycle;
			if (cycle == 20 || cycle == 60 || cycle == 100 || cycle == 140 || cycle == 180 || cycle == 220) {
				sum += X * cycle;
			}
		}
		X += cmd.arg;
	}
	trace(sum);
#else
	int X = 1;
	int cycle = 0;
	for (auto &cmd : cmds) {
		while (cmd.cycles > 0) {
			--cmd.cycles;
			++cycle;
			if (std::abs(((cycle - 1) % 40) - X) <= 1) {
				std::cout << '8';
			} else {
				std::cout << ' ';
			}
			if (cycle % 40 == 0) {
				std::cout << std::endl;
			}
		}
		X += cmd.arg;
	}
	std::cout << std::endl;
#endif
}
