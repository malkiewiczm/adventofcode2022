//#define PARTONE
#include "template.hpp"

using Vec3 = std::tuple<I64, I64, I64>;

#define getX std::get<0>
#define getY std::get<1>
#define getZ std::get<2>

static inline std::ostream &operator<<(std::ostream &o, const Vec3 &v)
{
	o << '(' << getX(v) << ", " << getY(v) << ", " << getZ(v) << ')';
	return o;
}

static inline Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs)
{
	return std::make_tuple(getX(lhs) + getX(rhs), getY(lhs) + getY(rhs), getZ(lhs) + getZ(rhs));
}


static inline Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs)
{
	return std::make_tuple(getX(lhs) - getX(rhs), getY(lhs) - getY(rhs), getZ(lhs) - getZ(rhs));
}


#ifdef PARTONE

static I64 count_sides(const Vec3 &target, const std::set<Vec3> &cubes)
{
	const Vec3 sides[6] = {
		{ 1, 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 }, { 0, 0, 1 }, { 0, 0, -1 }
	};
	I64 sum = 0;
	FOR(i, 6) {
		Vec3 testpos = target + sides[i];
		if (!CONTAINS(cubes, testpos)) {
			++sum;
		}
	}
	return sum;
}

#else

using Grid = std::map<Vec3, int>;
constexpr int border_color = 1;
constexpr int rock_color = 2;

static int get_color_at(const Grid &g, const Vec3 &pos)
{
	auto iter = g.find(pos);
	if (iter == g.end()) {
		return 0;
	} else {
		return iter->second;
	}
}

static void flood_fill(Grid &g, int color, const Vec3 first_pos)
{
	const Vec3 sides[6] = {
		{ 1, 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 }, { 0, 0, 1 }, { 0, 0, -1 }
	};
	std::vector<Vec3> to_search;
	to_search.push_back(first_pos);
	while (! to_search.empty()) {
		const Vec3 pos = to_search.back();
		to_search.pop_back();
		FOR(i, 6) {
			const Vec3 newpos = pos + sides[i];
			if (get_color_at(g, newpos) == 0) {
				g.emplace(newpos, color);
				to_search.push_back(newpos);
			}
		}
	}
}

static I64 count_sides(const Grid &g, const Vec3 &target)
{
	const Vec3 sides[6] = {
		{ 1, 0, 0 }, { -1, 0, 0 }, { 0, 1, 0 }, { 0, -1, 0 }, { 0, 0, 1 }, { 0, 0, -1 }
	};
	I64 sum = 0;
	FOR(i, 6) {
		Vec3 testpos = target + sides[i];
		if (get_color_at(g, testpos) == border_color) {
			++sum;
		}
	}
	return sum;
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
	std::set<Vec3> cubes;
	{
		std::string line;
		while (std::getline(f, line)) {
			std::ostringstream oss;
			for (const auto &c : line) {
				if (c == ',') {
					oss << ' ';
				} else {
					oss << c;
				}
			}
			std::istringstream iss(oss.str());
			Vec3 pos;
			iss >> getX(pos) >> getY(pos) >> getZ(pos);
			cubes.emplace(pos);
		}
	}
#ifdef PARTONE
	I64 ans = 0;
	for (const auto &cube : cubes) {
		ans += count_sides(cube, cubes);
	}
	trace(ans);
#else
	Vec3 mins = std::make_tuple(999, 999, 999);
	Vec3 maxs = std::make_tuple(-999, -999, -999);
	for (const auto &cube : cubes) {
		getX(mins) = std::min(getX(mins), getX(cube));
		getY(mins) = std::min(getY(mins), getY(cube));
		getZ(mins) = std::min(getZ(mins), getZ(cube));
		getX(maxs) = std::max(getX(maxs), getX(cube));
		getY(maxs) = std::max(getY(maxs), getY(cube));
		getZ(maxs) = std::max(getZ(maxs), getZ(cube));
	}
	Grid g;
	{
		Vec3 low = mins - std::make_tuple(2, 2, 2);
		Vec3 high = maxs + std::make_tuple(2, 2, 2);
		for (I64 z = getZ(low); z <= getZ(high); ++z) {
			for (I64 y = getY(low); y <= getY(high); ++y) {
				for (I64 x = getX(low); x <= getX(high); ++x) {
					if (x == getX(low) || x == getX(high) || y == getY(low) || y == getY(high) || z == getZ(low) || z == getZ(high)) {
						g.emplace(std::make_tuple(x, y, z), border_color);
					}
				}
			}
		}
	}
	for (const auto &cube : cubes) {
		g.emplace(cube, rock_color);
	}
	flood_fill(g, border_color, mins - std::make_tuple(1, 1, 1));
	I64 ans = 0;
	for (const auto &cube : cubes) {
		ans += count_sides(g, cube);
	}
	trace(ans);
#endif
}
