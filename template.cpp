//#define PARTONE
#include "template.hpp"

int main(int argc, char **argv)
{
	const char *const fname = (argc >= 2) ? argv[1] : "in.txt";
#ifdef StringsInput
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<std::string> ins;
	{
		std::string line;
		while (std::getline(f, line)) {
			ins.push_back(line);
		}
	}
	trace(ins.size());
#endif
#ifdef IntegerInput
	std::ifstream f(fname);
	if (! f.good()) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<int> nums;
	{
		int num;
		while (f >> num) {
			nums.push_back(num);
		}
	}
	trace(nums.size());
#endif
#ifdef CStyleInput
	FILE *const f = fopen(fname, "r");
	if (f == nullptr) {
		TRACE << "file cannot be opened" << std::endl;
		die();
	}
	std::vector<int> nums;
	{
		int num;
		while (fscanf(f, "%d", &num) != EOF) {
			nums.push_back(num);
		}
		fclose(f);
	}
	trace(nums.size());
#endif
}
