#include "Utilities/Profiler.h"

std::unordered_map<std::string, Profiler::Pair> Profiler::_map;
Profiler::Resolution Profiler::_res = Profiler::Nanoseconds;

inline void Profiler::Pair::PushBack() {
	lastStart = start;
	lastEnd = end;
};

void Profiler::SetResolution(Profiler::Resolution resolution) {
	_res = resolution;
}

void Profiler::Prepare(std::string key) {
	Pair _new;
	_new.start = std::chrono::high_resolution_clock::now();
	_new.end = _new.start;
	_map[key] = _new;
}

void Profiler::Start(std::string key) {
	_map[key].start = std::chrono::high_resolution_clock::now();
}

void Profiler::End(std::string key) {
	_map[key].end = std::chrono::high_resolution_clock::now();
	_map[key].PushBack();
}

int64 Profiler::Geti(std::string key) {
	Profiler::Pair& get = _map[key];
	switch (_res) {
		case Nanoseconds:
			return std::chrono::duration_cast<std::chrono::nanoseconds>(get.lastEnd - get.lastStart).count();
		case Microseconds:
			return std::chrono::duration_cast<std::chrono::microseconds>(get.lastEnd - get.lastStart).count();
		case Milliseconds:
			return std::chrono::duration_cast<std::chrono::milliseconds>(get.lastEnd - get.lastStart).count();
	}
}

float Profiler::Getf(std::string key) {
	Profiler::Pair& get = _map[key];
	switch (_res) {
		case Nanoseconds:
			return std::chrono::duration_cast<std::chrono::nanoseconds>(get.lastEnd - get.lastStart).count() * 0.000001f;
		case Microseconds:
			return std::chrono::duration_cast<std::chrono::microseconds>(get.lastEnd - get.lastStart).count() * 0.001f;
		case Milliseconds:
			return std::chrono::duration_cast<std::chrono::milliseconds>(get.lastEnd - get.lastStart).count();
	}
}
