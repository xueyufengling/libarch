#include <arch/call.h>

#include <malloc.h>
#include <unwind.h>

using namespace arch;

/**
 * @brief 从Instruction Pointer获取该指令所属的函数的函数指针地址
 */
void* arch::enclosing_function(void* ip)
{
	return ::_Unwind_FindEnclosingFunction(ip); //Itanium ABI提供的语言无关实现
}

