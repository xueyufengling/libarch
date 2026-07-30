#ifndef _ARCH_CARRAY
#define _ARCH_CARRAY

#include <stdint.h>

extern "C"
{
struct array
{
	size_t size;
	uint8_t data[1]; //数据起始地址，长度为length
};

__declspec(dllexport) extern array* alloc_array(size_t length);

__declspec(dllexport) extern void free_array(array* arr);
}

#endif //_ARCH_CARRAY
