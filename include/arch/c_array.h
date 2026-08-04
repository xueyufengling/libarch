#ifndef _ARCH_CARRAY
#define _ARCH_CARRAY

#include <stdint.h>

extern "C"
{
struct c_array
{
	const size_t size;
	uint8_t data[1]; //数据起始地址，长度为length
};

__declspec(dllexport) extern c_array* alloc_c_array(size_t length);

__declspec(dllexport) extern void free_c_array(c_array* arr);
}

#endif //_ARCH_CARRAY
