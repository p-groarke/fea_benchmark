#include <cstdio>
#include <vector>

#include "../benchUtil.h"

/*interface*/ struct IPotato {
	void* potato;
	int (*update)(void*);
};

struct ITomato {
	virtual int update() = 0;
};

struct Potato : ITomato {
	__attribute__((noinline)) int update() {
		return ++test;
	}
	int test = 42;
};

struct Tomato : ITomato {
	__attribute__((noinline)) int update() {
		return ++test;
	}
	int test = 42;
};

const int qty = 1000000000;
const int v_size = 100000000;

int main (int argc, char** argv) {
	std::vector<ITomato*> tom_vec(v_size);
	std::vector<IPotato> pot_vec(v_size);
	ITomato* it = new Tomato();
	Potato* p = new Potato();
	long** p_vtable = (long **)p;
	IPotato ip;
	ip.update = (int (*)(void*))p_vtable[0][0];

//	Bench::start();
//	for (int i = 0; i < qty; ++i) {
//		it->update();
//	}
//	Bench::end("vtable");
//
//	Bench::start();
//	for (int i = 0; i < qty; ++i) {
//		ip.update(p);
//	}
//	Bench::end("struct with C callback");

	for (int i = 0; i < tom_vec.size(); ++i) {
		tom_vec[i] = new Tomato();
	}

	// Not storing the object in vector, just methods.
	for (int i = 0; i < pot_vec.size(); ++i) {
		pot_vec[i].potato = new Potato();
		long** pot_vtable = (long **)(pot_vec[i].potato);
		pot_vec[i].update = (int (*)(void*))pot_vtable[0][0];
	}
	int tomato_result = 0;
	int potato_result = 0;

	Bench::start();
	for (const auto& x : tom_vec) {
		tomato_result += x->update();
	}
	Bench::end("vtable");

	Bench::start();
	for (const auto& x : pot_vec) {
		potato_result += x.update(x.potato);
	}
	Bench::end("struct");

	printf("Use values %d %d \n", tomato_result, potato_result);
//	printf("Use values %d %d \n", ((Tomato*)it)->test, p->test);
}
