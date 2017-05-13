#include "bench_util.h"

int main (int argc, char** argv) {
	bench::title("Your title");
	bench::start("Optional string");
	// Do stuff
	bench::stop("Optional but really recommended string");

	bench::start();
	// Do stuff
	bench::stop("Optional but really recommended string");

	bench::start("Optional string");
	// Do more stuff
	bench::stop("Optional but really recommended string");
	bench::start();
	// Do more stuff
	bench::stop("Optional but really recommended string");
	return 0;
}