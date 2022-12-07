#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <math.h>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

template <typename T>
using PriorityQueue = std::priority_queue<T, std::vector<T>, std::greater<T>>;

#define TRACE (std::cout << "[line " << __LINE__ << "] ")
#define trace(what)(TRACE << #what " = " << (what) << std::endl)
#define lintf(fmt, ...)(printf("[line %d] " fmt, __LINE__, __VA_ARGS__))

#define FOR(i, to) for (int i = 0; i < to; ++i)
#define FOR2(i, from, to) for (int i = from; i < to; ++i)
#define FOR3(i, from, to, step) for (int i = from; i < to; i += step)

#define die() do { TRACE << "die() called" << std::endl; std::exit(1); } while(0)

#define ALL(x) (x).begin(), (x).end()
#define CONTAINS(map, key) ((map).find(key) != (map).end())
#define SIZE(x) static_cast<int>((x).size())

template <typename T0, typename T1>
static inline std::ostream &operator<<(std::ostream &o, const std::pair<T0, T1> &rhs)
{
	o << '{' << rhs.first << ", " << rhs.second << '}';
	return o;
}

template <typename T>
static inline std::ostream &operator<<(std::ostream &o, const std::vector<T> &rhs)
{
	o << '{';
	for (const auto &item : rhs) {
		o << item << "; ";
	}
	o << '}';
	return o;
}

template <typename T>
static inline std::ostream &operator<<(std::ostream &o, const std::set<T> &rhs)
{
	o << '{';
	for (const auto &item : rhs) {
		o << item << "; ";
	}
	o << '}';
	return o;
}

template <typename K, typename V>
static inline std::ostream &operator<<(std::ostream &o, const std::map<K, V> &rhs)
{
	o << '{';
	for (const auto &item : rhs) {
		o << item << "; ";
	}
	o << '}';
	return o;
}

using I64 = long long int;
static_assert(sizeof(I64) == 8, "i64 not 8 bytes?");
