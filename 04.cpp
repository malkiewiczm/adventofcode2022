//#define PARTONE
#include "template.hpp"

using Pair = std::pair<int, int>;


#ifdef PARTONE

static bool pair_contains(const Pair &v0, const Pair &v1)
{
	return v0.first >= v1.first && v0.second <= v1.second;
}

#else

static bool pair_contains(const Pair &v0, const Pair &v1)
{
	if (v0.first >= v1.first && v0.second <= v1.second)
		return true;
	if (v0.first >= v1.first && v0.first <= v1.second)
		return true;
	if (v0.second >= v1.first && v0.second <= v1.second)
		return true;
	return false;
}

#endif

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	FILE *const f = fopen(fname, "r");
	if (f == nullptr) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	Pair v0, v1;
	int count = 0;
	while (fscanf(f, "%d-%d,%d-%d", &v0.first, &v0.second, &v1.first, &v1.second) == 4) {
		if (pair_contains(v0, v1) || pair_contains(v1, v0)) {
			++count;
		}
	}
	fclose(f);
	trace(count);
}
