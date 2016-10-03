#include <cstdio>
#include <string>
#include <iostream>
#include <vector>
#include "../benchUtil.h"

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

int main (int argc, char** argv) {

//	std::string qty_string;
//	std::cin >> qty_string;
//	int adder = std::stoi(qty_string);


	IPotato* ip = new Potato();
	Bench::start();
	for (int i = 0; i < qty; ++i) {
		ip->update(/*adder*/);
	}
	Bench::end("Abstract pointer");

	NormalClass* n = new NormalClass();
	Bench::start();
	for (int i = 0; i < qty; ++i) {
		n->update(/*adder*/);
	}
	Bench::end("Normal pointer");

	printf("Lets use the results %d %d\n", ((Potato*)ip)->test, n->test);
	//IPotato* ip = new Potato();
	//ip->update();
	//Bench::clobber();
	//Potato p;
	//p.update();
	//NormalClass n;
	//n.update();
	return 0;
}
