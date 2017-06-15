#include <functional>

#include "bench_util.h"

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

std::function<double(double)> funcValue = [](double n) -> double {
	if (n < 2)
		return n;
	return funcValue(n - 1) + funcValue(n - 2);
};

int main (int, char**) {
	bench::title("Comparing different ways to call methods and functions.");

	bench::start();
	func(qty);
	bench::stop("Completely normal function call");

	bench::start();
	funcInline(qty);
	bench::stop("Inline function call");

	bench::start();
	funcValue(qty);
	bench::stop("std::function and lambda");

	Normal obj;
	bench::start();
	obj.func(qty);
	bench::stop("Function called from an object");

	Normal* p = new Normal();
	bench::start();
	p->func(qty);
	bench::stop("Function called from a pointer");

	A aobj;
	bench::start();
	aobj.func(qty);
	bench::stop("Virtual on object");

	A* a = new A();
	bench::start();
	a->func(qty);
	bench::stop("Virtual on pointer");

#ifdef _MSC_VER
	system("pause");
#endif
}
