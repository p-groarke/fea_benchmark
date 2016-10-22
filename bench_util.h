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

#include <iomanip>
#include <iostream>
#include <string>
#include <chrono>

/**
 * escape(void*) and clobber() are from Chandler Carruth. More info:
 * https://www.youtube.com/watch?v=nXaxk27zwlk
 * Ty Chandler!
 */

namespace bench {
	std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
	std::chrono::duration<double> elapsedTime;

	void title(const std::string& title) {
		std::cout << std::endl << std::string(title.size(), '#') << std::endl;
		std::cout << title << std::endl;
		std::cout << std::string(title.size(), '#') << std::endl << std::endl;
	}

	void start(const std::string& message = "") {
		if (!message.empty()) {
			std::cout << std::endl << message << std::endl;
			std::cout << std::string(message.size(), '-') << std::endl;
		}
		startTime = std::chrono::system_clock::now();
	}

	void end(const std::string& message = "") {
		endTime = std::chrono::system_clock::now();
		elapsedTime = endTime - startTime;

		std::cout << std::setw(70) << std::left << message
				<< std::setiosflags(std::ios::fixed)
				<< elapsedTime.count() << "s" << std::endl;
	}

	/**
	 * This deactivates compiler optimizations for the passed pointer.
	 * It indicates that the pointer memory "could" have been modified.
	 *
	 * Usage: Pass the pointer to an allocated object you want to benchmark.
	 */
	static void escape(void *p) {
#ifdef _MSC_VER
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
	static void clobber() {
#ifdef _MSC_VER
		_ReadBarrier();
#else
		asm volatile("" : : : "memory");
#endif
	}
};