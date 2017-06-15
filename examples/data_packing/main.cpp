//#define BENCH_SHUTUP 1
#include "bench_util.h"

struct NotPacked {
	char c = 'c';
	long l = 42;
	char a = 'a';
	char b = 'b';
	int i = 42;
	double d = 3.22;
};

struct Packed {
	char c = 'c';
	long l = 42;
	char a = 'a';
	char b = 'b';
	int i = 42;
	double d = 3.22;
} __attribute((packed));

const int qty = 10000000;
int main (int, char**)
{
	int temp = 0;
	Packed pack;
	NotPacked npack;

	bench::title("Iterating over packed data vs non-packed.");
	BENCH_PRINT("%d iterations. Packed size : %zu bytes, Non-packed size : %zu bytes\n",
			qty, sizeof(Packed), sizeof(NotPacked));

	bench::start();
	for (int i = 0; i < qty; ++i) {
		temp += npack.c;
		temp += npack.l;
		temp += npack.a;
		temp += npack.b;
		temp += npack.i;
		temp += npack.d;
		bench::clobber();
	}
	bench::stop("Non-Packed Data");

	bench::start();
	for (int i = 0; i < qty; ++i) {
		temp += pack.c;
		temp += pack.l;
		temp += pack.a;
		temp += pack.b;
		temp += pack.i;
		temp += pack.d;
		bench::clobber();
	}
	bench::stop("Packed Data");

	printf("%d\n", temp);
	return 0;
}
