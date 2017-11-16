#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <bench_util/bench_util.h>

#ifndef _MSC_VER
	#define NO_INLINE __attribute__((noinline))
#else
	#define NO_INLINE __declspec(noinline)
#endif

struct IPotato {
	virtual void update(/*int a*/) = 0;
};

struct Potato : public IPotato {
	NO_INLINE void update(/*int a*/) {
		++test;// += a;
	}
	int test = 42;
};

struct NormalClass {
	NO_INLINE void update(/*int a*/) {
		++test;// += a;
	}
	int test = 42;
};

const int qty = 1000000000;

int main (int, char**) {

//	std::string qty_string;
//	std::cin >> qty_string;
//	int adder = std::stoi(qty_string);


	IPotato* ip = new Potato();
	bench::start();
	for (int i = 0; i < qty; ++i) {
		ip->update(/*adder*/);
	}
	bench::stop("Abstract pointer");

	NormalClass* n = new NormalClass();
	bench::start();
	for (int i = 0; i < qty; ++i) {
		n->update(/*adder*/);
	}
	bench::stop("Normal pointer");

	printf("Lets use the results %d %d\n", ((Potato*)ip)->test, n->test);
	//IPotato* ip = new Potato();
	//ip->update();
	//bench::clobber();
	//Potato p;
	//p.update();
	//NormalClass n;
	//n.update();
	return 0;
}
