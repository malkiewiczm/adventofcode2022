//#define PARTONE
#include "template.hpp"

using Grid = std::vector<std::string>;

#define INF 99999999
#define H SIZE(grid)
#define W SIZE(grid[0])

static int rank(char c)
{
	if (c == 'S')
		c = 'a';
	if (c == 'E')
		c = 'z';
	if (c >= 'a' && c <= 'z')
		return static_cast<int>(c);
	return INF;
}

static I64 go(const Grid &grid, std::pair<int, int> start, std::pair<int, int> goal)
{
	using Tuple = std::tuple<I64, int, int>;
	PriorityQueue<Tuple> pq;
	std::set<std::pair<int, int>> visited;
	const int dy[4] { 1, -1, 0, 0 };
	const int dx[4] { 0, 0, 1, -1 };
	pq.push(std::make_tuple(0, start.first, start.second));
	while (! pq.empty()) {
		const I64 steps = std::get<0>(pq.top());
		const std::pair<int, int> pos = std::make_pair(std::get<1>(pq.top()), std::get<2>(pq.top()));
		pq.pop();
		if (pos == goal)  {
			return steps;
		}
		if (CONTAINS(visited, pos)) {
			continue;
		}
		visited.emplace(pos);
		FOR(i, 4) {
			const std::pair<int, int> newpos = std::make_pair(pos.first + dy[i], pos.second + dx[i]);
			if (newpos.first < 0 || newpos.first >= H || newpos.second < 0 || newpos.second > W) {
				continue;
			}
			if (CONTAINS(visited, newpos)) {
				continue;
			}
			const int val = rank(grid[pos.first][pos.second]);
			const int newval = rank(grid[newpos.first][newpos.second]);
			if (newval - 1 > val) {
				continue;
			}
			pq.push(std::make_tuple(steps + 1, newpos.first, newpos.second));
		}
	}
	return INF;
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	Grid grid;
	{
		std::string line;
		while (std::getline(f, line)) {
			grid.push_back(line);
		}
	}
#ifdef PARTONE
	std::pair<int, int> start, goal;
	FOR(y, H) {
		FOR(x, W) {
			if (grid[y][x] == 'S') {
				start = std::make_pair(y, x);
			} else if (grid[y][x] == 'E') {
				goal = std::make_pair(y, x);
			}
		}
	}

	const I64 ans = go(grid, start, goal);
	trace(ans);
#else
	std::vector<std::pair<int, int>> starts;
	std::pair<int, int> goal;
	FOR(y, H) {
		FOR(x, W) {
			if (grid[y][x] == 'S' || grid[y][x] == 'a') {
				starts.push_back(std::make_pair(y, x));
			} else if (grid[y][x] == 'E') {
				goal = std::make_pair(y, x);
			}
		}
	}
	I64 ans = go(grid, starts[0], goal);
	FOR2(i, 1, SIZE(starts)) {
		ans = std::min(ans, go(grid, starts[i], goal));
	}
	trace(ans);
#endif
}
