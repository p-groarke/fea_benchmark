/**
 * Copyright (c) 2019, Philippe Groarke <philippe.groarke@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#pragma once
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstring>
#include <vector>

#ifdef _MSC_VER
#include <intrin.h>
#endif

#if BENCH_SHUTUP
#define BENCH_PRINT(...)
#else
#define BENCH_PRINT_STREAM(stream, format, ...) \
	fprintf(stream, "" format "", ##__VA_ARGS__)
#endif

namespace bench {
// Compiler freaks out with macro-only usage.
template <typename T>
void m_unused(const T&) {
}

static std::chrono::time_point<std::chrono::high_resolution_clock> start_time,
		end_time;

static inline void title(const char* message, FILE* stream = stdout) {
	m_unused(message);
	BENCH_PRINT_STREAM(stream, "%.*s\n", (int)strlen(message),
			"############################################################");
	BENCH_PRINT_STREAM(stream, "%s\n", message);
	BENCH_PRINT_STREAM(stream, "%.*s\n", (int)strlen(message),
			"############################################################");
}

static inline void start(const char* message = "", FILE* stream = stdout) {
	if (strlen(message) != 0) {
		BENCH_PRINT_STREAM(stream, "\n%s\n", message);
		BENCH_PRINT_STREAM(stream, "%.*s\n", (int)strlen(message),
				"--------------------------------------------------------");
	}

	start_time = std::chrono::high_resolution_clock::now();
}

static inline double stop(const char* message = "", FILE* stream = stdout) {
	m_unused(message);
	end_time = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> elapsed_time = end_time - start_time;

	BENCH_PRINT_STREAM(stream, "%s%*fs\n", message, 70 - (int)strlen(message),
			elapsed_time.count());
	return elapsed_time.count();
}

/**
 * This deactivates compiler optimizations for the passed pointer.
 * It indicates that the pointer memory "could" have been modified.
 *
 * Usage: Pass the pointer to an allocated object you want to benchmark.
 * https://www.youtube.com/watch?v=nXaxk27zwlk
 */
static inline void escape(void* p) {
#ifdef _MSC_VER
	m_unused(p);
	_WriteBarrier();
#else
	asm volatile("" : : "g"(p) : "memory");
#endif
}

/**
 * This method deactivates compiler optimizations by indicating all memory
 * could have been modified.
 *
 * Usage: Use after a call, to make sure the compiler doesn't remove the call.
 * https://www.youtube.com/watch?v=nXaxk27zwlk
 */
static inline void clobber() {
#ifdef _MSC_VER
	_ReadBarrier();
#else
	asm volatile("" : : : "memory");
#endif
}

struct suite {
	void title(const char* message) {
		_title = message;
	}

	template <class Func>
	void benchmark(const char* message, Func&& func, size_t num_runs = 1) {
		if (num_runs == 0) {
			return;
		}

		std::chrono::duration<double> elapsed_time = std::chrono::seconds(0);

		for (size_t i = 0; i < num_runs; ++i) {
			std::chrono::time_point<std::chrono::high_resolution_clock>
					_start_time, _end_time;

			_start_time = std::chrono::high_resolution_clock::now();
			func();
			_end_time = std::chrono::high_resolution_clock::now();

			elapsed_time += _end_time - _start_time;
		}

		_results.push_back(
				{ message, elapsed_time.count() / double(num_runs) });
	}

	void print(FILE* stream = stdout) {
		if (_title != nullptr) {
			BENCH_PRINT_STREAM(stream, "%.*s\n", (int)strlen(_title),
					"##########################################################"
					"##");
			BENCH_PRINT_STREAM(stream, "%s\n", _title);
			BENCH_PRINT_STREAM(stream, "%.*s\n", (int)strlen(_title),
					"##########################################################"
					"##");
		}

		if (_results.size() < 2)
			return;

		std::sort(_results.begin(), _results.end(),
				[](const pair& lhs, const pair& rhs) {
					return lhs.time > rhs.time;
				});

		for (const pair& p : _results) {
			double ratio = _results.front().time / p.time;
			BENCH_PRINT_STREAM(stream, "%s%*fs        %fx\n", p.message,
					70 - int(strlen(p.message)), p.time, ratio);
		}
		BENCH_PRINT_STREAM(stream, "\n");
	}

	void clear() {
		_title = nullptr;
		_results.clear();
	}

private:
	struct pair {
		const char* message{ nullptr };
		double time{ 0.0 };
	};

	const char* _title{ nullptr };
	std::vector<pair> _results;
};
}; // namespace bench
