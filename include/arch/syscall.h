#ifndef _ARCH_SYSCALL
#define _ARCH_SYSCALL

#include <stdint.h>
#include <string.h>

#include <ppmp/semantic.h>

#include "arch.h"
#include "os.h"

namespace arch
{
#define __syscall_types_arch_os__() x86_win32, x86_64_win64

__enum_def__(0, syscall_type, , __syscall_types_arch_os__())

/**
 * syscall模板的信息，包含
 * size：函数大小；
 * no_offset：是调用号偏移量；
 * cl_offset：栈清理大小；
 * tpl：syscall函数的模板内容指针。
 */
#define __syscall_info_names__() __pack__(size_t, size), __pack__(size_t, no_offset), __pack__(size_t, cl_offset), __pack__(const void*, tpl)

// syscall_xxx(syscall_type type)系列函数，获取syscall信息
#define __decl_syscall_info_func_op_intl__(func_prefix, ret_type, info_name)\
	extern ret_type __cat__(2, func_prefix, info_name)(::arch::syscall_type type) noexcept;

#define __decl_syscall_info_func_op__(i, begin_idx, end_idx, func_prefix, info)\
	__call_exp__(0)(__decl_syscall_info_func_op_intl__, func_prefix, __unpack__(info))

#define __decl_syscall_info_func__(func_prefix, ...)\
	__for_each__(0)(__decl_syscall_info_func_op__, func_prefix, __VA_ARGS__)

__decl_syscall_info_func__(syscall_, __syscall_info_names__())

#undef __decl_syscall_info_func__
#undef __decl_syscall_info_func_op__
#undef __decl_syscall_info_func_op_intl__

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
