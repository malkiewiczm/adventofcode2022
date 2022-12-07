//#define PARTONE
#include "template.hpp"

struct File {
	std::string name;
	I64 size;
};

struct Dir {
	std::string name;
	std::vector<Dir*> dirs;
	std::vector<File> files;
	I64 size;
};

static I64 fill_sizes(Dir *const root)
{
	root->size = 0;
	FOR(i, SIZE(root->dirs)) {
		root->size += fill_sizes(root->dirs[i]);
	}
	FOR(i, SIZE(root->files)) {
		root->size += root->files[i].size;
	}
	return root->size;
}

#ifdef PARTONE
static int p1count(const Dir *const root, I64 amt)
{
	I64 sum = 0;
	if (root->size <= amt) {
		sum += root->size;
	}
	FOR(i, SIZE(root->dirs)) {
		sum += p1count(root->dirs[i], amt);
	}
	return sum;
}
#else

static void find_all_dir_sizes(const Dir *const root, std::vector<I64> &vec)
{
	FOR(i, SIZE(root->dirs)) {
		vec.push_back(root->dirs[i]->size);
		find_all_dir_sizes(root->dirs[i], vec);
	}
}

#endif

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
	std::ifstream infile(fname);
	if (! infile.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	Dir root;
	std::vector<Dir*> dirstack;
	dirstack.push_back(&root);
	std::string line;
	while (std::getline(infile, line)) {
		std::istringstream ss(line);
		if (line[0] == '$') {
			std::string cmd;
			ss >> cmd;
			ss >> cmd;
			if (cmd == "cd") {
				std::string name;
				ss >> name;
				if (name == "/") {
					dirstack.clear();
					dirstack.push_back(&root);
				} else if (name == "..") {
					dirstack.pop_back();
				} else {
					Dir *d = new Dir;
					d->name = name;
					dirstack.back()->dirs.push_back(d);
					dirstack.push_back(d);
				}
			}
		} else {
			std::string maybedir;
			ss >> maybedir;
			if (maybedir == "dir") {
			} else {
				File f;
				f.size = std::atoi(maybedir.c_str());
				ss >> f.name;
				dirstack.back()->files.push_back(f);
			}
		}
	}
	fill_sizes(&root);
#ifdef PARTONE
	TRACE << p1count(&root, 100000) << std::endl;
#else
	std::vector<I64> vec;
	find_all_dir_sizes(&root, vec);
	std::sort(ALL(vec));
	const I64 needed_size = 30000000 - (70000000 - root.size);
	for (const auto item : vec) {
		if (item > needed_size) {
			trace(item);
			break;
		}
	}
#endif
}
