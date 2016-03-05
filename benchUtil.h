#include <iomanip>
#include <iostream>
#include <string>
#include <chrono>

namespace Bench {
	std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
	std::chrono::duration<double> elapsedTime;

	void title(const std::string& title) {
		std::cout << std::endl << std::string(title.size(), '-') << std::endl;
		std::cout << title << std::endl;
		std::cout << std::string(title.size(), '-') << std::endl;
	}

	void start(const std::string& message = "") {
		std::cout << message << std::endl;
		startTime = std::chrono::system_clock::now();
	}

	void end(const std::string& message = "") {
		endTime = std::chrono::system_clock::now();
		elapsedTime = endTime - startTime;
		std::cout << message << " took: " << std::setiosflags(std::ios::fixed) << elapsedTime.count() << "s" << std::endl;
	}
};