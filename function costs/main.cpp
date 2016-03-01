#include <functional>

#include "../benchUtil.h"

int qty = 50;

class Base {
public:
	virtual double func(double n);
};

double Base::func(double n) {
	if (n < 2)
		return n;
	std::cout << "b";
	return func(n - 1) + func(n - 2);
}

class A : public Base {
public:
	virtual double func(double n);
};

double A::func(double n) {
	if (n < 2)
		return n;
	// std::cout << "a";
	return func(n - 1) + func(n - 2);
}

class Normal {
public:
	double func(double n);
};

double Normal::func(double n) {
	if (n < 2)
		return n;
	return func(n - 1) + func(n - 2);
}

inline double funcInline(double n) {
	if (n < 2)
		return n;
	return funcInline(n - 1) + funcInline(n - 2);
}

double func(double n) {
	if (n < 2)
		return n;
	return func(n - 1) + func(n - 2);
}

std::function<double(double)> funcValue = [&](double n) -> double {
	if (n < 2)
		return n;
	return funcValue(n - 1) + funcValue(n - 2);
};

int main (int argc, char** argv) {
	Bench::title("Comparing different ways to call methods and functions.");

	Bench::start();
	func(qty);
	Bench::end("Completely normal function call");

	Bench::start();
	funcInline(qty);
	Bench::end("Inline function call");

	Bench::start();
	funcValue(qty);
	Bench::end("std::function and lambda");

	Normal obj;
	Bench::start();
	obj.func(qty);
	Bench::end("Function called from an object");

	Normal* p = new Normal();
	Bench::start();
	p->func(qty);
	Bench::end("Function called from a pointer");

	A aobj;
	Bench::start();
	aobj.func(qty);
	Bench::end("Virtual on object");

	A* a = new A();
	Bench::start();
	a->func(qty);
	Bench::end("Virtual on pointer");

	// a = (Base*)a;
	// Bench::start();
	// ((Base *)a)->func(qty);
	// Bench::end("Virtual called on paren");

#ifdef _MSC_VER
	system("pause");
#endif
}
