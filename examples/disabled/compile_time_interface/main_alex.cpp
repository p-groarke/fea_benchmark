#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <bench_util/bench_util.h>

#ifndef _MSC_VER
	#define NO_INLINE __attribute__((noinline))
#else
	#define NO_INLINE __declspec(noinline)
#endif

class StructImplement;

class Struct {
public:
	Struct()
	{
	}

	void (*OnUpdate)(Struct* my_struct);
};

void StructImplement_OnUpdate(Struct* my_struct);

class StructImplement : public Struct {
public:
	StructImplement(int banana)
		: _banana(banana)
		, _sum(0)
	{
		OnUpdate = &StructImplement_OnUpdate;
	}

	int _banana;
	int _sum;
};

NO_INLINE void StructImplement_OnUpdate(Struct* my_struct)
{
	StructImplement* s = static_cast<StructImplement*>(my_struct);
	s->_sum += s->_banana;
}

class Interface {
public:
	Interface()
	{
	}

	virtual void OnUpdate() = 0;
};

class InterfaceImplement : public Interface {
public:
	InterfaceImplement(int banana)
		: _banana(banana)
		, _sum(0)
	{
	}

	NO_INLINE void OnUpdate()
	{
		_sum += _banana;
	}

	int _banana;
	int _sum;
};

int main(int, char**)
{

	StructImplement my_struct(10000);
	my_struct.OnUpdate = &StructImplement_OnUpdate;

	InterfaceImplement* tomato = new InterfaceImplement(10000);

	const int N_TIMES = 1000000000;

	bench::start("");
	for (int i = 0; i < N_TIMES; i++) {
		my_struct.OnUpdate(&my_struct);
	}
	bench::stop("struct of C pointer");

	bench::start("");
	for (int i = 0; i < N_TIMES; i++) {
		tomato->OnUpdate();
	}
	bench::stop("vtable");

	std::cout << my_struct._sum << std::endl;
	std::cout << tomato->_sum << std::endl;

	return 0;
}
