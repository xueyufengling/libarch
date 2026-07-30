#include <arch/c_array.h>

#include <malloc.h>

array* alloc_array(size_t length)
{
	array* arr = (array*)malloc(length + sizeof(size_t));
	arr->size = length;
	return arr;
}

void free_array(array* arr)
{
	free(arr);
}
