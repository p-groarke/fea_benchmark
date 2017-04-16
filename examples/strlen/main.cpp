#include "../../bench_util.h"
#include <stdio.h>
#include <emmintrin.h>

#if defined(_MSC_VER)
	#include <intrin.h> // Valid for >= 1300. Older VS not supported in fst.
#endif

// Test alex's
// #define haszero(v) (((v) - 0x01010101UL) & ~(v) & 0x80808080UL)

// __attribute__((__always_inline__)) size_t alex_strlen2(const char *s) {
// 	size_t* i = static_cast<size_t*>((void*)(s));
// 	while(!(word_has_zero_byte(*i++)));
// 	if(!(*(--i) & 0xFF000000)) { return (((char*)(i)) - 9) - s; }
// 	else if(!(*i & 0xFF0000)) { return (((char*)i) - 10) - s; }
// 	else if(!(*i & 0xFF00)) { return (((char*)i) - 11) - s; }
// 	else { return (((char*)i) - 12) - s; }
// }

size_t basic_strlen(const char *s) {
    const char *start = s;
    while(*s)
        s++;
    return s - start;
}

size_t simili_simd_strlen(const char *s) {
    size_t len = 0;
    for(;;) {
        unsigned x = *(unsigned*)s;
        if((x & 0xFF) == 0) return len;
        if((x & 0xFF00) == 0) return len + 1;
        if((x & 0xFF0000) == 0) return len + 2;
        if((x & 0xFF000000) == 0) return len + 3;
        s += 4, len += 4;
    }
}

inline std::size_t cross_platform_bsf(unsigned int mask)
{
#if defined(_MSC_VER)
	unsigned long pos;
	_BitScanForward(&pos, mask);
	return pos;
#else
	return __builtin_ctz(mask);
#endif
}

std::size_t simd_strlen(const char* str)
{
	/* Speed up edge case. */
	if (str == nullptr || *str == '\0')
		return 0;

	std::size_t len = 0;
	const char* ptr_bak = str;

	__m128i xmm0 = _mm_setzero_si128();
	__m128i xmm1;
	unsigned int mask = 0;

	/* Prepass to align data on 16 bytes. Deal with beginning data. */
	if (((intptr_t)str & (sizeof(__m128i) - 1)) != 0) {
		/* Use load unaligned to set everything up. */
		xmm1 = _mm_loadu_si128((const __m128i*)str);
		xmm1 = _mm_cmpeq_epi8(xmm1, xmm0);

		if ((mask = _mm_movemask_epi8(xmm1)) != 0) {
			/* Found the end in the first bits. */
			return cross_platform_bsf(mask);
		}
		str = (const char *)(0x10 + (intptr_t)str & ~0xF);
	}

	/* Search for 0. */
	for (;;) {
		xmm1 = _mm_load_si128((const __m128i*)str);
		xmm1 = _mm_cmpeq_epi8(xmm1, xmm0);
		if ((mask = _mm_movemask_epi8(xmm1)) != 0) {
			/* Got 0 somewhere within 16 bytes in xmm1, or within 16 bits in
			 * mask. Find index of first set bit. */
			len = str - ptr_bak;
			len += cross_platform_bsf(mask);
			break;
		}
		str += sizeof(__m128i);
	}
	return len;
}

const int bench_qty = 100000000;

int main()
{
	const char * str = ""
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"potato potato potato potato potato potato potato potato potato "
		"";
	int str_size = strlen(str);

	unsigned long long len = 0;
	bench::start();
	for (int i = 0; i < bench_qty; ++i) {
		len += strlen(str + (i % str_size));
	}
	printf("%lld\n", len);
	bench::stop("Platform strlen");

	len = 0;
	bench::start();
	for (int i = 0; i < bench_qty; ++i) {
		len += basic_strlen(str + (i % str_size));
	}
	printf("%lld\n", len);
	bench::stop("Basic strlen");

	len = 0;
	bench::start();
	for (int i = 0; i < bench_qty; ++i) {
		len += simili_simd_strlen(str + (i % str_size));
	}
	printf("%lld\n", len);
	bench::stop("Simili SIMD strlen");

	len = 0;
	bench::start();
	for (int i = 0; i < bench_qty; ++i) {
		len += simd_strlen(str + (i % str_size));
	}
	printf("%lld\n", len);
	bench::stop("SIMD strlen");

	return 0;
}
