//#define PARTONE
#include "template.hpp"

using Coord = std::pair<I64, I64>;

static void read_pair(const std::string &s, Coord &c0, Coord &c1)
{
	std::ostringstream oss;
	for (const auto &c : s) {
		if (c == '=') {
			oss << " = ";
		} else {
			oss << c;
		}
	}
	std::istringstream iss(oss.str());
	int i = 0;
	I64 n[4] = {0};
	for ( ; ; ) {
		std::string chunk;
		if (!(iss >> chunk)) {
			break;
		}
		if (chunk == "=") {
			iss >> n[i++];
		}
	}
	c0.second = n[0];
	c0.first = n[1];
	c1.second = n[2];
	c1.first = n[3];
}

static I64 dist(const Coord &c0, const Coord &c1)
{
	return std::abs(c0.first - c1.first) + std::abs(c0.second - c1.second);
}

static bool is_blocked(const std::vector<Coord> &sensors, const std::vector<I64> &distances, const Coord candidate)
{
	FOR(i, SIZE(sensors)) {
		if (dist(sensors[i], candidate) <= distances[i]) {
			return true;
		}
	}
	return false;
}

#ifndef PARTONE

static bool taxi_index(const I64 index, const Coord &center, const I64 width, Coord &out)
{
	const I64 section = index / width;
	const I64 sindex = index % width;
	if (section == 0) {
		out = std::make_pair(center.first - sindex, center.second - sindex + width);
		return true;
	}
	if (section == 1) {
		out = std::make_pair(center.first - sindex + width, center.second - sindex);
		return true;
	}
	if (section == 2) {
		out = std::make_pair(center.first + sindex, center.second - sindex - width);
		return true;
	}
	if (section == 3) {
		out = std::make_pair(center.first + sindex - width, center.second + sindex);
		return true;
	}
	return false;
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
	std::vector<Coord> sensors;
	std::set<Coord> beacons;
	std::vector<I64> distances;
	{
		std::string line;
		Coord c0, c1;
		while (std::getline(f, line)) {
			read_pair(line, c0, c1);
			sensors.push_back(c0);
			beacons.emplace(c1);
			distances.push_back(dist(c0, c1));
		}
	}
#ifdef PARTONE
	I64 left = beacons.begin()->second;
	I64 right = left;
	for (const auto &b : beacons) {
		left = std::min(left, b.second);
		right = std::max(right, b.second);
	}
	FOR(i, SIZE(sensors)) {
		left = std::min(left, sensors[i].second);
		right = std::max(right, sensors[i].second);
	}
	I64 ans = 0;
	constexpr I64 y = 2000000;
	//constexpr I64 y = 10;
	left -= y;
	right += y;
	for (I64 x = left; x <= right; ++x) {
		const Coord c = std::make_pair(y, x);
		if (CONTAINS(beacons, c)) {
			continue;
		}
		if (is_blocked(sensors, distances, c)) {
			++ans;
		}
	}
	trace(ans);
#else
	//constexpr I64 dim = 20;
	constexpr I64 dim = 4000000;
	FOR(i, SIZE(sensors)) {
		const Coord &sensor = sensors[i];
		I64 index = 0;
		const I64 width = distances[i] + 1;
		Coord c;
		while (taxi_index(index++, sensor, width, c)) {
			if (c.first >= 0 && c.first <= dim && c.second >= 0 && c.second <= dim && !is_blocked(sensors, distances, c)) {
				trace(c);
				trace(c.first + (c.second * 4000000));
			}
		}
	}
#endif
}
