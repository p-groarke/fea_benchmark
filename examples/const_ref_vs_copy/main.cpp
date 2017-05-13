#include <vector>

#include "bench_util.h"

double constFib(const double& n) {
	if (n < 2)
		return n;
	return constFib(n - 1) + constFib(n - 2);
}

double copyFib(double n) {
	if (n < 2)
		return n;
	return copyFib(n - 1) + copyFib(n - 2);
}

int main(int argc, char** argv) {
	bench::title("const reference VS. copy -- using doubles");

	bench::start();
	std::cout << "Fib = " << constFib(42) << std::endl;
	bench::stop("const double&");

	bench::start();
	std::cout << "Fib = " << copyFib(42) << std::endl;
	bench::stop("copy");

	return 0;
}


// const int qty = 10000000;
//
// void constFunc(const int& i) {
// 	int bleh = i + i;
// }
//
// void copyFunc(int i) {
// 	int bleh = i + i;
// }
//
//int main(int argc, char** argv) {
//	std::vector<int> vec(qty);
//	std::chrono::time_point<std::chrono::system_clock> startTime, endTime;
//	std::chrono::duration<double> elapsedTime;
//
//	std::fill(vec.begin(), vec.end(), 1);
//
//	// const ref test
//	startTime = std::chrono::system_clock::now();
//	for (int i = 0; i < qty; ++i) {
//		constFunc(vec[i]);
//	}
//	endTime = std::chrono::system_clock::now();
//	elapsedTime = endTime - startTime;
//	std::cout << "Const reference function took: " << elapsedTime.count() << "s" << std::endl;
//
//	// copy test
//	startTime = std::chrono::system_clock::now();
//	for (int i = 0; i < qty; ++i) {
//		copyFunc(vec[i]);
//	}
//	endTime = std::chrono::system_clock::now();
//	elapsedTime = endTime - startTime;
//	std::cout << "Copy funtion took: " << elapsedTime.count() << "s" << std::endl;
//
//	return 0;
//}