#include "bench_util.h"
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

inline int mul2(int v) {
	// v * 2
	return v << 1;
}

inline int mul10(int v) {
	// v * 8 + v * 2
	return (v << 3) + (v << 1);
}

inline int mul100(int v) {
	// v * 64 + v * 32 + v * 4
	return (v << 6) + (v << 5) + (v << 2);
}

// http://www.hackersdelight.org/divcMore.pdf
inline unsigned int divu10(unsigned int n) {
	unsigned int q, r;
	q = (n >> 1) + (n >> 2);
	q = q + (q >> 4);
	q = q + (q >> 8);
	q = q + (q >> 16);
	q = q >> 3;
	// r = n - q * 10;
	r = n - mul10(q);
	return q + ((r + 6) >> 4);
}

inline unsigned int divu100(unsigned int n) {
	unsigned int q, r;
	q = (n >> 1) + (n >> 3) + (n >> 6) - (n >> 10) + (n >> 12) + (n >> 13) - (n >> 16);
	q = q + (q >> 20);
	q = q >> 6;
	// r = n - q * 100;
	r = n - mul100(q);
	return q + ((r + 28) >> 7);
	// return q + (r > 99);
}

inline int remu10(unsigned int n) {
	static char table[16] = {0, 1, 2, 2, 3, 3, 4, 5, 5, 6, 7, 7, 8, 8, 9, 0};
	n = (0x19999999 * n + (n >> 1) + (n >> 3)) >> 28;
	return table[n];
}

inline unsigned int remu100(int n) { return ((unsigned int)n) - (unsigned int)mul100((int)(divu100((unsigned int)n))); }

const char digit_pairs[201] = {
	"00010203040506070809"
	"10111213141516171819"
	"20212223242526272829"
	"30313233343536373839"
	"40414243444546474849"
	"50515253545556575859"
	"60616263646566676869"
	"70717273747576777879"
	"80818283848586878889"
	"90919293949596979899"};

// http://stackoverflow.com/questions/4351371/c-performance-challenge-integer-to-stdstring-conversion
int itostr(int value, char* str_buffer) {
	if (value == 0) {
		*(str_buffer++) = '0';
		*(str_buffer) = ' ';
		return 2;
	}

	int sign = -(value < 0);
	unsigned int val = (value ^ sign) - sign;

	int size;
	if (val >= 10000) {
		if (val >= 10000000) {
			if (val >= 1000000000)
				size = 10;
			else if (val >= 100000000)
				size = 9;
			else
				size = 8;
		} else {
			if (val >= 1000000)
				size = 7;
			else if (val >= 100000)
				size = 6;
			else
				size = 5;
		}
	} else {
		if (val >= 100) {
			if (val >= 1000)
				size = 4;
			else
				size = 3;
		} else {
			if (val >= 10)
				size = 2;
			else
				size = 1;
		}
	}
	size -= sign;

	char* c = str_buffer;
	*(c + size) = ' ';
	if (sign) *c = '-';

	c += size - 1;

	while (val >= 100) {
		// int pos = val % 100;
		int pos = (int)remu100(val);

		//    val /= 100;
		val = divu100(val);
		//*(short*)(c - 1) = *(short*)(digit_pairs + 2 * pos);
		*(short*)(c - 1) = *(short*)(digit_pairs + mul2(pos));
		c -= 2;
	}

	while (val > 0) {
		//*c-- = '0' + (val % 10);
		// val /= 10;
		*c-- = '0' + remu10(val);
		val = divu10(val);
	}

	return ++size;
}

int itostr(unsigned int val, char* str_buffer) {
	if (val == 0) {
		*(str_buffer++) = '0';
		*(str_buffer) = ' ';
		return 2;
	}

	int size;
	if (val >= 10000) {
		if (val >= 10000000) {
			if (val >= 1000000000)
				size = 10;
			else if (val >= 100000000)
				size = 9;
			else
				size = 8;
		} else {
			if (val >= 1000000)
				size = 7;
			else if (val >= 100000)
				size = 6;
			else
				size = 5;
		}
	} else {
		if (val >= 100) {
			if (val >= 1000)
				size = 4;
			else
				size = 3;
		} else {
			if (val >= 10)
				size = 2;
			else
				size = 1;
		}
	}

	//	s.resize(size);

	//	char* c = &s[size-1];
	*(str_buffer + size) = ' ';
	char* c = str_buffer + size - 1;

	while (val >= 100) {
		int pos = val % 100;
		val /= 100;
		*(short*)(c - 1) = *(short*)(digit_pairs + 2 * pos);
		c -= 2;
	}

	while (val > 0) {
		//		*c--='0' + (val % 10);
		*c-- = '0' + remu10(val);
		//		val /= 10;
		val = divu10(val);
	}

	return ++size;
}

template <typename T>
inline int p_format(char* str_buffer, int str_len, T value) {
	return str_len;
}

template <>
inline int p_format<int>(char* str_buffer, int str_len, int value) {
	return str_len + itostr(value, str_buffer + str_len);
	//  return str_len + sprintf(str_buffer + str_len, "%d ", value);
}

template <>
inline int p_format<unsigned int>(char* str_buffer, int str_len, unsigned int value) {
	return str_len + itostr(value, str_buffer + str_len);
	//  return str_len + sprintf(str_buffer + str_len, "%d ", value);
}

template <>
inline int p_format<const char*>(char* str_buffer, int str_len, const char* value) {
	strcpy(str_buffer + str_len, value);
	int t_len = str_len + strlen(value);
	*(str_buffer + t_len) = ' ';
	return t_len + 1;
}

template <>
inline int p_format<double>(char* str_buffer, int str_len, double value) {
	return str_len + sprintf(str_buffer + str_len, "%f ", value);
}

template <>
inline int p_format<float>(char* str_buffer, int str_len, float value) {
	return str_len + sprintf(str_buffer + str_len, "%f ", value);
}

inline void PrintChild(char* str_buffer, int str_len) {
	/// @todo Check str_len == 0.
	*(str_buffer + str_len - 1) = '\n';
	*(str_buffer + str_len) = '\0';
	// The values 0, 1, 2 can also be given, for standard input,
	// standard output & standard error, respectively.
	write(2, str_buffer, str_len);
}

template <typename T, typename... P>
inline void PrintChild(char* str_buffer, int str_len, T t, P... p) {
	PrintChild(str_buffer, p_format(str_buffer, str_len, t), p...);
}

template <typename T, typename... P>
inline void Print(T t, P... p) {
	char str_buffer[1024];
	PrintChild(str_buffer, p_format(str_buffer, 0, t), p...);
}


int main (int argc, char** argv) {
	bench::title("Fast print");

	const int qty = 100000;

	bench::start();
	for (int i = 0; i < qty; ++i) {
		Print("Mother", 100, "fucker", (unsigned int) 200);
		bench::clobber();
	}
	bench::stop("Fast print");

	bench::start();
	for (int i = 0; i < qty; ++i) {
		std::cerr << "Mother " << 100 << " fucker " << (unsigned int) 200 << std::endl;
		bench::clobber();
	}
	bench::stop("std::cout");

	bench::start();
	for (int i = 0; i < qty; ++i) {
		fprintf(stderr, "%s %d %s %u\n", "Mother", 100, "fucker", (unsigned int) 200);
		bench::clobber();
	}
	bench::stop("printf");

	//

	bench::start("Second try.");
	for (int i = 0; i < qty; ++i) {
		std::cerr << "Mother " << 100 << " fucker " << (unsigned int) 200 << std::endl;
		bench::clobber();
	}
	bench::stop("std::cout");

	bench::start();
	for (int i = 0; i < qty; ++i) {
		fprintf(stderr, "%s %d %s %u\n", "Mother", 100, "fucker", (unsigned int) 200);
		bench::clobber();
	}
	bench::stop("printf");

	bench::start();
	for (int i = 0; i < qty; ++i) {
		Print("Mother", 100, "fucker", (unsigned int) 200);
		bench::clobber();
	}
	bench::stop("Fast print");

	//

	bench::start("Third try.");
	for (int i = 0; i < qty; ++i) {
		fprintf(stderr, "%s %d %s %u\n", "Mother", 100, "fucker", (unsigned int) 200);
		bench::clobber();
	}
	bench::stop("printf");

	bench::start();
	for (int i = 0; i < qty; ++i) {
		Print("Mother", 100, "fucker", (unsigned int) 200);
		bench::clobber();
	}
	bench::stop("Fast print");

	bench::start();
	for (int i = 0; i < qty; ++i) {
		std::cerr << "Mother " << 100 << " fucker " << (unsigned int) 200 << std::endl;
		bench::clobber();
	}
	bench::stop("std::cout");

	return 0;
}
