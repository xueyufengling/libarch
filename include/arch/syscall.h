#ifndef _ARCH_SYSCALL
#define _ARCH_SYSCALL

#include <stdint.h>
#include <string.h>

#include <ppmp/loop.h>

#include "arch.h"
#include "os.h"

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

extern const syscall_type host_syscall_type;

/**
 * @brief 指定返回值类型的syscall函数指针
 */
template<typename _RetType = long>
using usyscall_t = _RetType (*)(...);

#if defined(__os_win__)
typedef long syscall_no_t;
typedef unsigned short syscall_stack_clean_t;
typedef long syscall_ret_t;
#endif

typedef usyscall_t<syscall_ret_t> syscall_t;

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
usyscall_t<_RetType> syscall(syscall_type type, void* mem, _SyscallNo syscall_num, _StackClean syscall_clean = 0)
{
	memcpy(mem, syscall_tpl(type), syscall_size(type)); //每个调用号都有自己的syscall函数内存
	*(_SyscallNo*)((char*)mem + syscall_no_offset(type)) = syscall_num; //替换模板中的syscall number
	if(syscall_cl_offset(type))
		*(_StackClean*)((char*)mem + syscall_cl_offset(type)) = syscall_clean; //如果需要自己清理栈，则填充清理的大小
	return (usyscall_t<_RetType> )mem;
}

/**
 * @brief 根据调用号构造当前操作系统的syscall函数。
 * 		  直接使用syscall指令进行内存分配，如果操作系统ring3库被注入，此函数也不会受到影响，但如果内核层ring0被注入或者有驱动，此函数亦将受影响.
 * 		  内核层注入属于最危险的漏洞，操作系统开发者几乎肯定会在未来的新版本中修复，因此本库只考虑绕开ring3的注入。
 */
inline syscall_t syscall(void* mem, syscall_no_t syscall_num, syscall_stack_clean_t syscall_clean = 0)
{
	return syscall<syscall_ret_t>(host_syscall_type, mem, syscall_num, syscall_clean);
}

}

#endif//_ARCH_SYSCALL
