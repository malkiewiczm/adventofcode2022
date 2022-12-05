//#define PARTONE

#include "template.hpp"

#define ROCK 1
#define PAPER 2
#define SCISSORS 3

#define WIN 6
#define DRAW 3
#define LOSE 0

static int to_score(const std::string &x)
{
	if (x == "A")
		return ROCK;
	if (x == "B")
		return PAPER;
	if (x == "C")
		return SCISSORS;
	if (x == "X")
		return ROCK;
	if (x == "Y")
		return PAPER;
	if (x == "Z")
		return SCISSORS;
	return 0;
}

static int score_round(const std::string &them, const std::string &you)
{
	if (to_score(you) == to_score(them))
		return DRAW;
	if (to_score(you) == ROCK && to_score(them) == SCISSORS)
		return WIN;
	if (to_score(you) == PAPER && to_score(them) == ROCK)
		return WIN;
	if (to_score(you) == SCISSORS && to_score(them) == PAPER)
		return WIN;
	return LOSE;
}

#ifndef PARTONE

static std::string predict(const std::string &them, const std::string &you)
{
	constexpr const char *rock = "X";
	constexpr const char *paper = "Y";
	constexpr const char *scissors = "Z";
	if (you == "X") {
		// Lose
		if (to_score(them) == ROCK)
			return scissors;
		if (to_score(them) == PAPER)
			return rock;
		if (to_score(them) == SCISSORS)
			return paper;
	}
	if (you == "Y") {
		// Draw
		if (to_score(them) == ROCK)
			return rock;
		if (to_score(them) == PAPER)
			return paper;
		if (to_score(them) == SCISSORS)
			return scissors;
	}
	if (you == "Z") {
		// Win
		if (to_score(them) == ROCK)
			return paper;
		if (to_score(them) == PAPER)
			return scissors;
		if (to_score(them) == SCISSORS)
			return rock;
	}
	return "??";
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
	{
		int score = 0;
		std::string them, you;
		while (f >> them >> you) {
#ifdef PARTONE
			score += to_score(you) + score_round(them, you);
#else
			score += to_score(predict(them, you)) + score_round(them, predict(them, you));
#endif
		}
		trace(score);
	}
}
