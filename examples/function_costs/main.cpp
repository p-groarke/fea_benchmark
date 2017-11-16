#include <functional>
#include <iostream>

#include <bench_util/bench_util.h>

using std::placeholders::_1;

const long qty = 1000000000;
double data = 3.14;

class Base {
public:
	virtual double func(double n);
};

double Base::func(double n)
{
	return n * 2;
}

class A : public Base {
public:
	virtual double func(double n);
};

double A::func(double n)
{
	return n * 2;
}

class Normal {
public:
	double func(double n);
};

double Normal::func(double n)
{
	return n * 2;
}

class IAbstract {
public:
	virtual double func(double n) = 0;
};

class NotAbstract : public IAbstract {
public:
	double func(double n);
};

double NotAbstract::func(double n) {
	return n * 2;
}

struct Functor {
	double operator()(double n) {
		return n * 2;
	}
};


double func(double n)
{
	return n * 2;
}

inline double func_inline(double n)
{
	return n * 2;
}


int main(int, char**)
{
	Normal normal_object;

	bench::title("Comparing different ways to call methods and functions.");

	std::cout << qty << " calls." << std::endl;

	bench::start("C calls");
	for (int n = 0; n < qty; n++) {
		func(data);
		bench::clobber();
	}
	bench::stop("Normal function");

	bench::start();
	for (int n = 0; n < qty; n++) {
		func_inline(data);
		bench::clobber();
	}
	bench::stop("Inline function");

	double (*c_callback)(double n) = func;
	bench::start();
	for (int n = 0; n < qty; n++) {
		c_callback(data);
		bench::clobber();
	}
	bench::stop("C callback function");

	double (Normal::*cpp_callback)(double) = &Normal::func;
	bench::start();
	for (int n = 0; n < qty; n++) {
		(normal_object.*cpp_callback)(data);
		bench::clobber();
	}
	bench::stop("C++ callback function");


	auto lambda = [](double n) { return n * 2; };
	bench::start("C++ functional");
	for (int n = 0; n < qty; n++) {
		lambda(data);
		bench::clobber();
	}
	bench::stop("Lambda");

	std::function<double(double)> free_func = func;
	bench::start();
	for (int n = 0; n < qty; n++) {
		free_func(data);
		bench::clobber();
	}
	bench::stop("Free function stored in std::function");

	std::function<double(double)> lambda_func = [](double n) { return n * 2.0; };
	bench::start();
	for (int n = 0; n < qty; n++) {
		lambda_func(data);
		bench::clobber();
	}
	bench::stop("Lambda stored in std::function");

	std::function<double(Normal&, double)> member_func = &Normal::func;
	bench::start();
	for (int n = 0; n < qty; n++) {
		member_func(normal_object, data);
		bench::clobber();
	}
	bench::stop("Stored member function in std::function");

	std::function<double()> bind_func = std::bind(func, data);
	bench::start();
	for (int n = 0; n < qty; n++) {
		bind_func();
		bench::clobber();
	}
	bench::stop("Parameters stored in std::function with std::bind");

	std::function<double(double)> member_and_object =
			std::bind(&Normal::func, normal_object, _1);
	bench::start();
	for (int n = 0; n < qty; n++) {
		member_and_object(data);
		bench::clobber();
	}
	bench::stop("Member and object stored in std::function with std::bind");

	std::function<double(double)> member_and_pointer =
			std::bind(&Normal::func, &normal_object, _1);
	bench::start();
	for (int n = 0; n < qty; n++) {
		member_and_pointer(data);
		bench::clobber();
	}
	bench::stop("Member and pointer stored in std::function with std::bind");

	std::function<double(double)> functor_func = Functor();
	bench::start();
	for (int n = 0; n < qty; n++) {
		functor_func(data);
		bench::clobber();
	}
	bench::stop("Functor stored in std::function");


	bench::start("Object-oriented");
	for (int n = 0; n < qty; n++) {
		normal_object.func(data);
		bench::clobber();
	}
	bench::stop("Object member function");

	Normal* p = new Normal();
	bench::start();
	for (int n = 0; n < qty; n++) {
		p->func(data);
		bench::clobber();
	}
	bench::stop("Pointer member function");

	A aobj;
	bench::start();
	for (int n = 0; n < qty; n++) {
		aobj.func(data);
		bench::clobber();
	}
	bench::stop("Object virtual member function");

	A* aRef = &aobj;
	bench::start();
	for (int n = 0; n < qty; n++) {
		aRef->func(data);
		bench::clobber();
	}
	bench::stop("Object reference virtual member function");

	// A& aCast = *aRef;
	// bench::start();
	// for (int n = 0; n < qty; n++) {
	// 	aCast.func(data);
	// 	bench::clobber();
	// }
	// bench::stop("Reference on pointer on virtual on object");

	A* a = new A();
	bench::start();
	for (int n = 0; n < qty; n++) {
		a->func(data);
		bench::clobber();
	}
	bench::stop("Pointer virtual member function");

	NotAbstract b;
	bench::start();
	for (int n = 0; n < qty; n++) {
		b.func(data);
		bench::clobber();
	}
	bench::stop("Abstract inherited Object");

	NotAbstract* c = new NotAbstract();
	bench::start();
	for (int n = 0; n < qty; n++) {
		c->func(data);
		bench::clobber();
	}
	bench::stop("Abstract inherited Poiner");
#ifdef _MSC_VER
	system("pause");
#endif
}
