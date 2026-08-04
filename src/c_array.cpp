#include <arch/c_array.h>

#include <malloc.h>

c_array* alloc_c_array(size_t length)
{
	c_array* arr = (c_array*)malloc(length + sizeof(size_t));
	*(size_t*)&(arr->size) = length;
	return arr;
}

void free_c_array(c_array* arr)
{
	free(arr);
}
