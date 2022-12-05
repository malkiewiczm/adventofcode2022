MAKEFLAGS += Rr
CXX := g++
CXXFLAGS := -Wall -Wextra -Wpedantic -Wshadow -std=c++17 -Ofast -march=native

%.exe: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.exe *.obj
