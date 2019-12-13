#include "Utilities/Profiler.h"

std::unordered_map<std::string, Profiler::Pair> Profiler::_map;
Profiler::Resolution Profiler::_res = Profiler::Nanoseconds;

void Profiler::SetResolution(Profiler::Resolution resolution) {
	_res = resolution;
}

void Profiler::Prepare(std::string key) {
	_map[key] = Pair();
}

void Profiler::Start(std::string key) {
	_map[key].start = std::chrono::high_resolution_clock::now();
}

void Profiler::End(std::string key) {
	_map[key].end = std::chrono::high_resolution_clock::now();
}

int64 Profiler::Geti(std::string key) {
	Profiler::Pair& get = _map[key];
	switch (_res) {
		case Nanoseconds:
			return std::chrono::duration_cast<std::chrono::nanoseconds>(get.end - get.start).count();
		case Microseconds:
			return std::chrono::duration_cast<std::chrono::microseconds>(get.end - get.start).count();
		case Milliseconds:
			return std::chrono::duration_cast<std::chrono::milliseconds>(get.end - get.start).count();
	}
}

float Profiler::Getf(std::string key) {
	Profiler::Pair& get = _map[key];
	switch (_res) {
		case Nanoseconds:
			return std::chrono::duration_cast<std::chrono::nanoseconds>(get.end - get.start).count() * 0.000000001;
		case Microseconds:
			return std::chrono::duration_cast<std::chrono::microseconds>(get.end - get.start).count() * 0.000001;
		case Milliseconds:
			return std::chrono::duration_cast<std::chrono::milliseconds>(get.end - get.start).count() * 0.001;
	}
}
