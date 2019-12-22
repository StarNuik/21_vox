#pragma once

#include "Types.h"
#include <string>
#include <unordered_map>
#include <chrono>

typedef std::chrono::high_resolution_clock::time_point __timepoint;
typedef std::chrono::high_resolution_clock::duration __duration;

#define __MS_DIV (1000000.f)
#define __S_DIV (1000000000.f)
#define __MS_MULT (0.000001f)
#define __S_MULT (0.000000001f)

class Profiler {
public:
	static void Prepare(std::string);
	static void Start(std::string);
	static void End(std::string);
	static void Add(std::string);
	static float GetMs(std::string);
	static float GetS(std::string);
	static float GetTotalMs(std::string);
	static float GetTotalS(std::string);
	static float GetAverageMs(std::string);
	static float GetAverageS(std::string);
public:
	class Pair {
	public:
		Pair();
		__timepoint start;
		__timepoint end;
		__duration total;
		uint64 count;
		__timepoint lastStart;
		__timepoint lastEnd;
		inline void PushBack();
	};
	static std::unordered_map<std::string, Pair> _map;
};
