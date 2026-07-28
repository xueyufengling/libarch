#ifndef _ARCH_SYSCALLTPL
#define _ARCH_SYSCALLTPL

#include <stdint.h>

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

typedef long (*syscall_t)(...);

/**
 * @brief 根据调用号在指定内存位置构造一个syscall函数。
 * 		  该内存必须有执行权限才能调用。
 */
extern syscall_t syscall(syscall_type type, void* mem, long syscall_num, unsigned short syscall_clean = 0);

/**
 * @brief syscall函数的大小
 */
extern size_t syscall_size(syscall_type type);
}

#endif//_ARCH_SYSCALLTPL
