#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include <bench_util/bench_util.h>

class IPotato {
public:
	virtual void update(/*int a*/) = 0;
};


class Potato : public IPotato {
public:
	__attribute__((noinline)) void update(/*int a*/) {
		++test;// += a;
	}
	int test = 42;
};

class NormalClass {
public:
	__attribute__((noinline)) void update(/*int a*/) {
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
