//#define PARTONE
#include "template.hpp"

#define ROCK '#'
#define SAND 'o'
#define AIR ' '

struct Point {
	int x;
	int y;
	int cmp(const Point &rhs) const {
		if (x != rhs.x)
			return x - rhs.x;
		return y - rhs.y;
	}
	bool operator<(const Point &rhs) const {
		return cmp(rhs) < 0;
	}
	bool operator<=(const Point &rhs) const {
		return cmp(rhs) <= 0;
	}
	bool operator>(const Point &rhs) const {
		return cmp(rhs) > 0;
	}
	bool operator>=(const Point &rhs) const {
		return cmp(rhs) >= 0;
	}
	bool operator==(const Point &rhs) const {
		return cmp(rhs) == 0;
	}
	bool operator!=(const Point &rhs) const {
		return cmp(rhs) != 0;
	}
	Point operator+(const Point &rhs) const {
		return Point{x + rhs.x, y + rhs.y};
	}
	Point operator-(const Point &rhs) const {
		return Point{x - rhs.x, y - rhs.y};
	}
	void operator+=(const Point &rhs) {
		x += rhs.x;
		y += rhs.y;
	}
};

static inline std::ostream &operator<<(std::ostream &o, const Point &p)
{
	o << "(" << p.x << ", " << p.y << ")";
	return o;
}

struct Line {
	Point p0;
	Point p1;
};

using Grid = std::map<Point, char>;

static int sign(int x)
{
	if (x < 0)
		return -1;
	if (x > 0)
		return 1;
	return 0;
}

#ifndef PARTONE
static int floor_level = 0;
#endif

static char grid_find(const Grid &grid, const Point p)
{
#ifndef PARTONE
	if (p.y == floor_level) {
		return ROCK;
	}
#endif
	auto iter = grid.find(p);
	if (iter == grid.end()) {
		return AIR;
	}
	return iter->second;
}

static bool try_move(const Grid &grid, Point p, Point delta)
{
	p += delta;
	return grid_find(grid, p) == AIR;
}

static bool is_at_rest(const Grid &grid, Point point, Point &where)
{
	const Point down{0, 1};
	const Point down_left{-1, 1};
	const Point down_right{1, 1};
	where = point + down;
	if (try_move(grid, point, down)) {
		return false;
	}
	where = point + down_left;
	if (try_move(grid, point, down_left)) {
		return false;
	}
	where = point + down_right;
	if (try_move(grid, point, down_right)) {
		return false;
	}
	return true;
}

#ifdef PARTONE

static bool simulate_from(Grid &grid, Point p, const int height_limit)
{
	for ( ; ; ) {
		if (p.y > height_limit) {
			return false;
		}
		Point where;
		if (is_at_rest(grid, p, where)) {
			grid.emplace(p, SAND);
			return true;
		}
		p.x = where.x;
		p.y = where.y;
	}
}

#else

static bool simulate_from(Grid &grid, Point p, const int height_limit)
{
	const Point source = p;
	for ( ; ; ) {
		Point where;
		if (is_at_rest(grid, p, where)) {
			grid.emplace(p, SAND);
			return p != source;
		}
		p.x = where.x;
		p.y = where.y;
	}
}

#endif

static inline void print_me(const Grid &grid)
{
	constexpr int inf = 0x3f3f3f3f;
	int left = inf;
	int right = -inf;
	int top = inf;
	int bottom = -inf;
	for (const auto &kv : grid) {
		const int x = kv.first.x;
		const int y = kv.first.y;
		left = std::min(left, x);
		right = std::max(right, x);
		top = std::min(top, y);
		bottom = std::max(bottom, y);
	}
	for (int y = top; y <= bottom; ++y) {
		for (int x = left; x <= right; ++x) {
			std::cout << grid_find(grid, Point{x, y});
		}
		std::cout << std::endl;
	}
}


int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<Line> lines;
	{
		std::string line;
		while (std::getline(f, line)) {
			std::istringstream ss(line);
			std::string chunk;
			std::vector<Point> points;
			while (ss >> chunk) {
				if (chunk == "->") {
					continue;
				}
				std::ostringstream oss;
				for (const auto &c : chunk) {
					if (c >= '0' && c <= '9') {
						oss << c;
					} else {
						oss << ' ';
					}
				}
				std::istringstream iss(oss.str());
				Point p;
				iss >> p.x >> p.y;
				points.push_back(p);
			}
			if (SIZE(points) <= 1) {
				continue;
			}
			Line l;
			FOR2(i, 1, SIZE(points)) {
				l.p0 = points[i - 1];
				l.p1 = points[i];
				lines.push_back(l);
			}
		}
	}
	Grid grid;
	int height_limit = 0;
	FOR(i, SIZE(lines)) {
		height_limit = std::max(height_limit, lines[i].p0.y);
		height_limit = std::max(height_limit, lines[i].p1.y);
		Point step = lines[i].p1 - lines[i].p0;
		step.x = sign(step.x);
		step.y = sign(step.y);
		for (Point p = lines[i].p0; p != lines[i].p1; p += step) {
			grid.emplace(p, '#');
		}
		grid.emplace(lines[i].p1, '#');
	}
#ifndef PARTONE
	floor_level = height_limit + 2;
#endif
	const Point start{500, 0};
	while (simulate_from(grid, start, height_limit)) {}
	int ans = 0;
	for (const auto &kv : grid) {
		if (kv.second == SAND) {
			++ans;
		}
	}
	//print_me(grid);
	trace(ans);
}
