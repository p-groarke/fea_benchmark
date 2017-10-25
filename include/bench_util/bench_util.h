/**
 * Copyright (c) 2016, Philippe Groarke <philippe.groarke@gmail.com>.
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
#include <chrono>
#include <cstdio>
#include <cstring>

#ifdef _MSC_VER
	#include <intrin.h>
#endif

/* Deprecated */
#include <iostream>
#include <string>

/**
 * escape(void*) and clobber() are from Chandler Carruth. More info:
 * https://www.youtube.com/watch?v=nXaxk27zwlk
 */

#if BENCH_SHUTUP
	#define BENCH_PRINT(...)
#else
	#define BENCH_PRINT(stream, format, ...) fprintf(stream, "" format "", ##__VA_ARGS__)
#endif

namespace bench {
	// Compiler freaks out with macro-only usage.
	template <typename T> void m_unused(const T&) {}

	static std::chrono::time_point<std::chrono::high_resolution_clock> start_time, end_time;

	static inline void title(const char* message, FILE* stream = stdout) {
		m_unused(message);
		BENCH_PRINT(stream, "%.*s\n", (int)strlen(message),
				"############################################################");
		BENCH_PRINT(stream, "%s\n", message);
		BENCH_PRINT(stream, "%.*s\n", (int)strlen(message),
				"############################################################");
	}
	[[deprecated("Please use 'title(const char* message)' instead.")]]
	static inline void title(const std::string& message) { title(message.c_str()); }

	static inline void start(const char* message = "", FILE* stream = stdout) {
		if (strlen(message) != 0) {
			BENCH_PRINT(stream, "\n%s\n", message);
			BENCH_PRINT(stream, "%.*s\n", (int)strlen(message),
					"--------------------------------------------------------");
		}

		start_time = std::chrono::high_resolution_clock::now();
	}
	[[deprecated("Please use 'start(const char* message)' instead.")]]
	static inline void start(const std::string& message) { start(message.c_str()); }

	static inline double stop(const char* message = "", FILE* stream = stdout) {
		m_unused(message);
		end_time = std::chrono::high_resolution_clock::now();
		const std::chrono::duration<double> elapsed_time = end_time - start_time;

		BENCH_PRINT(stream, "%s%*fs\n", message, 70 - (int)strlen(message), elapsed_time.count());
		return elapsed_time.count();
	}
	[[deprecated("Please use 'stop(const char* message)' instead.")]]
	static inline void end(const std::string& message) { stop(message.c_str()); }

	/**
	 * This deactivates compiler optimizations for the passed pointer.
	 * It indicates that the pointer memory "could" have been modified.
	 *
	 * Usage: Pass the pointer to an allocated object you want to benchmark.
	 */
	static inline void escape(void *p) {
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
	 */
	static inline void clobber() {
#ifdef _MSC_VER
		_ReadBarrier();
#else
		asm volatile("" : : : "memory");
#endif
	}
};
