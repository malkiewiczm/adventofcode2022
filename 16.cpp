// THIS DOES NOT WORK!!!

//#define PARTONE
#include "template.hpp"

struct Node {
	I64 rate;
	std::vector<I64> adj;
};

static inline std::ostream &operator<<(std::ostream &o, const Node &n)
{
	o << n.rate << " -- " << n.adj;
	return o;
}

using Graph = std::vector<Node>;

static I64 label_to_index(const std::string &label)
{
	static std::map<std::string, I64> label_to_index_mapping;
	auto iter = label_to_index_mapping.find(label);
	if (iter == label_to_index_mapping.end()) {
		const I64 index = SIZE(label_to_index_mapping);
		label_to_index_mapping.emplace(label, index);
		return index;
	}
	return iter->second;
}

#define CHECK_OPEN(index, open_state) (((open_state >> index) & 1) != 0)
#define SET_OPEN(index, open_state) (open_state | (1 << index))

static I64 h(const Graph &g, const I64 current, const I64 budget, const I64 open_state)
{
	const Node &n = g[current];
	if (budget <= 1) {
		return 0;
	}
	if (budget == 2) {
		if (CHECK_OPEN(current, open_state)) {
			return 0;
		} else {
			return n.rate;
		}
	}
	using Key = std::tuple<I64, I64, I64>;
	static std::map<Key, I64> memo;
	const Key key = std::make_tuple(current, budget, open_state);
	{
		auto iter = memo.find(key);
		if (iter != memo.end()) {
			return iter->second;
		}
	}
	I64 best = 0;
	for (const auto &adj : n.adj) {
		const I64 not_taken = h(g, adj, budget - 1, open_state);
		I64 taken = 0;
		if (n.rate > 0 && !CHECK_OPEN(current, open_state)) {
			taken = h(g, adj, budget - 2, SET_OPEN(current, open_state));
			taken += n.rate * (budget - 1);
		}
		best = std::max(best, not_taken);
		best = std::max(best, taken);
	}
	memo.emplace(key, best);
	return best;
}

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::map<std::string, Node> dumb_g;
	std::string line;
	while (std::getline(f, line)) {
		std::ostringstream oss;
		for (const auto &c : line) {
			if (c == '=') {
				oss << ' ' << c << ' ';
			} else if (c != ';' && c != ',') {
				oss << c;
			}
		}
		std::istringstream iss(oss.str());
		std::string chunk;
		iss >> chunk;
		std::string label;
		iss >> label;
		while (iss >> chunk) {
			if (chunk == "=") {
				break;
			}
		}
		Node n;
		iss >> n.rate;
		while (iss >> chunk) {
			if (chunk == "valves" || chunk == "valve") {
				break;
			}
		}
		while (iss >> chunk) {
			n.adj.push_back(label_to_index(chunk));
		}
		dumb_g.emplace(label, n);
	}
	Graph g(SIZE(dumb_g));
	for (const auto &kv : dumb_g) {
		g[label_to_index(kv.first)] = kv.second;
	}
	const I64 start = label_to_index("AA");
	const I64 ans = h(g, start, 30, 0);
	trace(ans);
}
