//#define PARTONE
#include "template.hpp"

#define GRID_WIDTH 7


using Coord = std::pair<int, int>;
using Shape = std::vector<Coord>;

struct Grid {
	std::vector<std::vector<bool>> matrix;
	void resize(const Coord &pos);
	bool get(const Coord &pos);
	void set(const Coord &pos, bool value);
	int height(int prev_height) const;
	bool try_move(Coord &pos, const Shape &shape, const Coord delta);
	void mark(const Coord &pos, const Shape &shape);
	void printme() const {
		for (int y = SIZE(matrix) - 1; y >= 0; --y) {
			FOR(x, GRID_WIDTH) {
				if (matrix[y][x]) {
					std::cout << '#';
				} else {
					std::cout << '.';
				}
			}
			std::cout << std::endl;
		}
		std::cout << "XXXXXXX" << std::endl;
	}
};

void Grid::resize(const Coord &pos)
{
	while (pos.first >= SIZE(matrix)) {
		matrix.push_back(std::vector<bool>(GRID_WIDTH, false));
	}
}

bool Grid::get(const Coord &pos)
{
	if (pos.first < 0) {
		return true;
	}
	if (pos.second < 0 || pos.second >= GRID_WIDTH) {
		trace(pos);
		die();
	}
	resize(pos);
	return matrix[pos.first][pos.second];
}

void Grid::set(const Coord &pos, const bool value)
{
	resize(pos);
	matrix[pos.first][pos.second] = value;
}

int Grid::height(int prev_height) const
{
	FOR2(y, std::max(0, prev_height - 1), SIZE(matrix)) {
		bool row_is_clean = true;
		FOR(x, GRID_WIDTH) {
			if (matrix[y][x]) {
				row_is_clean = false;
				break;
			}
		}
		if (row_is_clean) {
			return y;
		}
	}
	return 0;
}

bool Grid::try_move(Coord &pos, const Shape &shape, const Coord delta)
{
	Coord new_move = std::make_pair(pos.first + delta.first, pos.second + delta.second);
	FOR(i, SIZE(shape)) {
		const Coord transient = std::make_pair(new_move.first + shape[i].first, new_move.second + shape[i].second);
		if (transient.second < 0 || transient.second >= GRID_WIDTH) {
			return false;
		}
		if (get(transient)) {
			return false;
		}
	}
	pos = new_move;
	return true;
}

void Grid::mark(const Coord &pos, const Shape &shape)
{
	FOR(i, SIZE(shape)) {
		set(std::make_pair(pos.first + shape[i].first, pos.second + shape[i].second), true);
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
	std::string ins;
	std::getline(f, ins);
	const std::vector<Shape> shapes = {
		// h line
		{ { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 } },
		// plus
		{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 0, 1 }, { 2, 1 } },
		// L
		{ { 0, 0 }, { 0, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 } },
		// v line
		{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
		// box
		{ { 0, 0 }, { 1, 0 }, { 0, 1 }, { 1, 1 } },
	};
	I64 height = 0;
	int wind_index = 0;
	int shape_index = 0;
	Grid grid;
#ifdef PARTONE
	FOR(shape_count, 2022) {
		const Shape &shape = shapes[shape_index];
		shape_index = (shape_index + 1) % SIZE(shapes);
		Coord pos = std::make_pair(height + 3, 2);
		for ( ; ; ) {
			const char wind = ins[wind_index];
			wind_index = (wind_index + 1) % SIZE(ins);
			if (wind == '<') {
				grid.try_move(pos, shape, std::make_pair(0, -1));
			} else if (wind == '>') {
				grid.try_move(pos, shape, std::make_pair(0, 1));
			}
			if (!grid.try_move(pos, shape, std::make_pair(-1, 0))) {
				break;
			}
		}
		grid.mark(pos, shape);
		height = grid.height(height);
	}
	trace(height);
#else
	auto step = [&]() {
					const Shape &shape = shapes[shape_index];
					shape_index = (shape_index + 1) % SIZE(shapes);
					Coord pos = std::make_pair(height + 3, 2);
					for ( ; ; ) {
						const char wind = ins[wind_index];
						wind_index = (wind_index + 1) % SIZE(ins);
						if (wind == '<') {
							grid.try_move(pos, shape, std::make_pair(0, -1));
						} else if (wind == '>') {
							grid.try_move(pos, shape, std::make_pair(0, 1));
						}
						if (!grid.try_move(pos, shape, std::make_pair(-1, 0))) {
							break;
						}
					}
					grid.mark(pos, shape);
					height = grid.height(height);
				};
	constexpr I64 starting_from = 500000;
	FOR(shape_count, starting_from) {
		step();
	}
	const I64 old_height = height;
	using Key = std::tuple<I64, I64, I64>;
	std::set<Key> seen;
	std::vector<I64> repeated;
	I64 height_diff = 0;
	for ( ; ; ) {
		step();
		I64 h = 0;
		FOR2(y, height - 9, height) {
			FOR(x, GRID_WIDTH) {
				h <<= 1;
				if (grid.get(std::make_pair(y, x))) {
					h |= 1;
				}
			}
		}
		Key key = std::make_tuple(shape_index, wind_index, h);
		if (CONTAINS(seen, key)) {
			height_diff = height - repeated[0];
			break;
		}
		repeated.push_back(height);
		seen.emplace(key);
	}
	constexpr I64 trillion = 1000000000000;
	const I64 ans = ((trillion - starting_from) / SIZE(repeated)) * height_diff + old_height + repeated[(trillion - starting_from) % SIZE(repeated)] - repeated[0];
	trace(ans);
#endif
}
