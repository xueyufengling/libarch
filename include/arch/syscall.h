#ifndef _ARCH_SYSCALLTPL
#define _ARCH_SYSCALLTPL

#include <stdint.h>
#include <string.h>

#include <arch/arch.h>
#include <ppmp/loop.h>

namespace arch
{
#define __syscall_types_arch_os__() x86_win32, x86_64_win64

#define __syscall_type_enum_def_op__(i, begin_idx, end_idx, enum_name, arch_os) __cat__(3, enum_name, _, arch_os),

#define __syscall_type_enum_def__(enum_name, ...)\
	enum enum_name\
	{\
		__for_each__(0)(__syscall_type_enum_def_op__, enum_name, __VA_ARGS__)\
		__cat__(2, enum_name, _num)\
	};

__syscall_type_enum_def__(syscall_type, __syscall_types_arch_os__())

#undef __syscall_type_enum_def__
#undef __syscall_type_enum_def_op__

/**
 * @brief 指定返回值类型的syscall函数指针
 */
template<typename _RetType = long>
using syscall_t = _RetType (*)(...);

/**
 * @brief syscall函数的大小
 */
extern size_t syscall_size(syscall_type type);

extern size_t syscall_no_offset(syscall_type type);

extern size_t syscall_cl_offset(syscall_type type);

extern const void* syscall_tpl(syscall_type type);

/**
 * @brief 根据调用号在指定内存位置构造一个syscall函数。
 * 		  该内存必须有执行权限才能调用。
 */
template<typename _RetType = long, typename _SyscallNo = long, typename _StackClean = unsigned short>
syscall_t<_RetType> syscall(syscall_type type, void* mem, _SyscallNo syscall_num, _StackClean syscall_clean = 0)
{
	memcpy(mem, syscall_tpl(type), syscall_size(type)); //每个调用号都有自己的syscall函数内存
	*(_SyscallNo*)((char*)mem + syscall_no_offset(type)) = syscall_num; //替换模板中的syscall number
	if(syscall_cl_offset(type))
		*(_StackClean*)((char*)mem + syscall_cl_offset(type)) = syscall_clean; //如果需要自己清理栈，则填充清理的大小
	return (syscall_t<_RetType> )mem;
}

}

#endif//_ARCH_SYSCALLTPL
