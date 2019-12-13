#pragma once

#include "Types.h"
#include <string>
#include <unordered_map>
#include <chrono>

typedef std::chrono::high_resolution_clock::time_point __timepoint;

class Profiler {
public:
	enum Resolution {
		Nanoseconds,
		Microseconds,
		Milliseconds
	};
	static void SetResolution(Resolution);
	static void Prepare(std::string);
	static void Start(std::string);
	static void End(std::string);
	static int64 Geti(std::string);
	static float Getf(std::string);
private:
	class Pair {
	public:
		__timepoint start;
		__timepoint end;
	};
	static Resolution _res;
	static std::unordered_map<std::string, Pair> _map;
};
