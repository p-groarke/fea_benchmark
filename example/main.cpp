#include "../benchUtil.h"

int main (int argc, char** argv) {
	Bench::title("Your title");
	Bench::start("Optional string");
	// Do stuff
	Bench::end("Optional but really recommended string");

	Bench::start();
	// Do stuff
	Bench::end("Optional but really recommended string");

	Bench::start("Optional string");
	// Do more stuff
	Bench::end("Optional but really recommended string");
	Bench::start();
	// Do more stuff
	Bench::end("Optional but really recommended string");
	return 0;
}