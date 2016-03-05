#include "../benchUtil.h"

int main (int argc, char** argv) {
	Bench::title("Your title");
	Bench::start("Optional string");
	// Do stuff
	Bench::end("Optional but really recommended string");
	return 0;
}