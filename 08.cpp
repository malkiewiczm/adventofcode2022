//#define PARTONE
#include "template.hpp"

#define H SIZE(grid)
#define W SIZE(grid[0])

using Pair = std::pair<int, int>;

#ifdef PARTONE

static bool is_clear(const std::vector<std::string> &grid, Pair start, const Pair dir)
{
	const char val = grid[start.first][start.second];
	start.first += dir.first;
	start.second += dir.second;
	while (start.first >= 0 && start.first < H && start.second >= 0 && start.second < W) {
		if (grid[start.first][start.second] >= val) {
			return false;
		}
		start.first += dir.first;
		start.second += dir.second;
	}
	return true;
}

static bool is_all_clear(const std::vector<std::string> &grid, const Pair start)
{
	const std::pair<int, int> dirs[4] { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
	FOR(i, 4) {
		if (is_clear(grid, start, dirs[i])) {
			return true;
		}
	}
	return false;
}

static I64 go(const std::vector<std::string> &grid)
{
	I64 count = 0;
	FOR(y, H) {
		FOR(x, W) {
			if (is_all_clear(grid, std::make_pair(y, x))) {
				++count;
			}
		}
	}
	return count;
}

#else

static I64 score_dir(const std::vector<std::string> &grid, Pair start, const Pair dir)
{
	const char val = grid[start.first][start.second];
	start.first += dir.first;
	start.second += dir.second;
	I64 score = 1;
	while (start.first >= 0 && start.first < H && start.second >= 0 && start.second < W) {
		if (grid[start.first][start.second] < val) {
			++score;
		} else {
			return score;
		}
		start.first += dir.first;
		start.second += dir.second;
	}
	return score - 1;
}

static I64 get_score(const std::vector<std::string> &grid, const Pair start)
{
	const std::pair<int, int> dirs[4] { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
	I64 total = 1;
	FOR(i, 4) {
		const I64 v = score_dir(grid, start, dirs[i]);
		total *= v;
	}
	return total;
}

static I64 go(const std::vector<std::string> &grid)
{
	I64 best = 0;
	FOR(y, H) {
		FOR(x, W) {
			const I64 score = get_score(grid, std::make_pair(y, x));
			if (score > best) {
				best = score;
			}
		}
	}
	return best;
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
	std::vector<std::string> grid;
	{
		std::string line;
		while (std::getline(f, line)) {
			grid.push_back(line);
		}
	}
	const I64 ans = go(grid);
	trace(ans);
}
