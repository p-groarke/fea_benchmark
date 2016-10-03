#include <functional>
#include "../benchUtil.h"

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


int main(int argc, char** argv)
{
	Normal normal_object;

	Bench::title("Comparing different ways to call methods and functions.");

	std::cout << qty << " calls." << std::endl;

	Bench::start("C calls");
	for (int n = 0; n < qty; n++) {
		func(data);
		Bench::clobber();
	}
	Bench::end("Normal function");

	Bench::start();
	for (int n = 0; n < qty; n++) {
		func_inline(data);
		Bench::clobber();
	}
	Bench::end("Inline function");

	double (*c_callback)(double n) = func;
	Bench::start();
	for (int n = 0; n < qty; n++) {
		c_callback(data);
		Bench::clobber();
	}
	Bench::end("C callback function");

	double (Normal::*cpp_callback)(double) = &Normal::func;
	Bench::start();
	for (int n = 0; n < qty; n++) {
		(normal_object.*cpp_callback)(data);
		Bench::clobber();
	}
	Bench::end("C++ callback function");


	auto lambda = [](double n) { return n * 2; };
	Bench::start("C++ functional");
	for (int n = 0; n < qty; n++) {
		lambda(data);
		Bench::clobber();
	}
	Bench::end("Lambda");

	std::function<double(double)> free_func = func;
	Bench::start();
	for (int n = 0; n < qty; n++) {
		free_func(data);
		Bench::clobber();
	}
	Bench::end("Free function stored in std::function");

	std::function<double(double)> lambda_func = [](double n) { return n * 2.0; };
	Bench::start();
	for (int n = 0; n < qty; n++) {
		lambda_func(data);
		Bench::clobber();
	}
	Bench::end("Lambda stored in std::function");

	std::function<double(Normal&, double)> member_func = &Normal::func;
	Bench::start();
	for (int n = 0; n < qty; n++) {
		member_func(normal_object, data);
		Bench::clobber();
	}
	Bench::end("Stored member function in std::function");

	std::function<double()> bind_func = std::bind(func, data);
	Bench::start();
	for (int n = 0; n < qty; n++) {
		bind_func();
		Bench::clobber();
	}
	Bench::end("Parameters stored in std::function with std::bind");

	std::function<double(double)> member_and_object =
			std::bind(&Normal::func, normal_object, _1);
	Bench::start();
	for (int n = 0; n < qty; n++) {
		member_and_object(data);
		Bench::clobber();
	}
	Bench::end("Member and object stored in std::function with std::bind");

	std::function<double(double)> member_and_pointer =
			std::bind(&Normal::func, &normal_object, _1);
	Bench::start();
	for (int n = 0; n < qty; n++) {
		member_and_pointer(data);
		Bench::clobber();
	}
	Bench::end("Member and pointer stored in std::function with std::bind");

	std::function<double(double)> functor_func = Functor();
	Bench::start();
	for (int n = 0; n < qty; n++) {
		functor_func(data);
		Bench::clobber();
	}
	Bench::end("Functor stored in std::function");


	Bench::start("Object-oriented");
	for (int n = 0; n < qty; n++) {
		normal_object.func(data);
		Bench::clobber();
	}
	Bench::end("Object member function");

	Normal* p = new Normal();
	Bench::start();
	for (int n = 0; n < qty; n++) {
		p->func(data);
		Bench::clobber();
	}
	Bench::end("Pointer member function");

	A aobj;
	Bench::start();
	for (int n = 0; n < qty; n++) {
		aobj.func(data);
		Bench::clobber();
	}
	Bench::end("Object virtual member function");

	A* aRef = &aobj;
	Bench::start();
	for (int n = 0; n < qty; n++) {
		aRef->func(data);
		Bench::clobber();
	}
	Bench::end("Object reference virtual member function");

	// A& aCast = *aRef;
	// Bench::start();
	// for (int n = 0; n < qty; n++) {
	// 	aCast.func(data);
	// 	Bench::clobber();
	// }
	// Bench::end("Reference on pointer on virtual on object");

	A* a = new A();
	Bench::start();
	for (int n = 0; n < qty; n++) {
		a->func(data);
		Bench::clobber();
	}
	Bench::end("Pointer virtual member function");

	NotAbstract b;
	Bench::start();
	for (int n = 0; n < qty; n++) {
		b.func(data);
		Bench::clobber();
	}
	Bench::end("Abstract inherited Object");

	NotAbstract* c = new NotAbstract();
	Bench::start();
	for (int n = 0; n < qty; n++) {
		c->func(data);
		Bench::clobber();
	}
	Bench::end("Abstract inherited Poiner");
#ifdef _MSC_VER
	system("pause");
#endif
}
