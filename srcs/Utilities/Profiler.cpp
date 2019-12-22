#include "Utilities/Profiler.h"

std::unordered_map<std::string, Profiler::Pair> Profiler::_map;

inline void Profiler::Pair::PushBack() {
	lastStart = start;
	lastEnd = end;
};

Profiler::Pair::Pair() {
	start = std::chrono::high_resolution_clock::now();
	end = start;
	total = std::chrono::high_resolution_clock::duration::zero();
	lastStart = start;
	lastEnd = start;
	count = 0;
};

void Profiler::Prepare(std::string key) {
	Pair _new;
	_map[key] = _new;
}

void Profiler::Start(std::string key) {
	_map[key].start = std::chrono::high_resolution_clock::now();
}

void Profiler::End(std::string key) {
	_map[key].end = std::chrono::high_resolution_clock::now();
	_map[key].PushBack();
}

void Profiler::Add(std::string key) {
	Profiler::Pair& get = _map[key];
	get.end = std::chrono::high_resolution_clock::now();
	get.total += get.end - get.start;
	get.count++;
	get.PushBack();
}

float Profiler::GetMs(std::string key) {
	Profiler::Pair& get = _map[key];
	return std::chrono::duration_cast<std::chrono::nanoseconds>(get.lastEnd - get.lastStart).count() / __MS_DIV;
}

float Profiler::GetS(std::string key) {
	Profiler::Pair& get = _map[key];
	return std::chrono::duration_cast<std::chrono::nanoseconds>(get.lastEnd - get.lastStart).count() / __S_DIV;
}

float Profiler::GetTotalMs(std::string key) {
	Profiler::Pair& get = _map[key];
	return std::chrono::duration_cast<std::chrono::nanoseconds>(get.total).count() / __MS_DIV;
};

float Profiler::GetTotalS(std::string key) {
	Profiler::Pair& get = _map[key];
	return std::chrono::duration_cast<std::chrono::nanoseconds>(get.total).count() / __S_DIV;
};

float Profiler::GetAverageMs(std::string key) {
	Profiler::Pair& get = _map[key];
	return (std::chrono::duration_cast<std::chrono::nanoseconds>(get.total).count() / (float)get.count) / __MS_DIV;
};

float Profiler::GetAverageS(std::string key) {
	Profiler::Pair& get = _map[key];
	return (std::chrono::duration_cast<std::chrono::nanoseconds>(get.total).count() / (float)get.count) / __S_DIV;
};
