Advent of Code 2022

https://adventofcode.com/2022

The programs read in input through a file called "in.txt" by default
but any file can be used if you pass a different file name through the
command line args.

If you define the compiler constant "PARTONE" then the program will
give you the part one solution, otherwise the default is part
two. #define PARTONE is left commented out for each day.

The makefile produces output files with suffix .exe, even on linux.

Example: compile and run day 25 with

$ make 25.exe
$ ./25.exe

The "stats" directory contains a simple frontend for exploring the
leaderboard json format.
