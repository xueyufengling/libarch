#include <arch/syscall.h>

#include <string.h>

#include "syscall_defs.h"

using namespace arch;

// syscall_tpl.S导出符号
#define __decl_syscall_tpl_op__(i, begin_idx, end_idx, const_params, arch_os)\
	extern "C"\
	{\
	extern const long __syscall_no_offset__(arch_os);\
	extern const long __syscall_size__(arch_os);\
	extern const long __syscall_cl_offset__(arch_os);\
	extern long __syscall_tpl__(arch_os)(...);\
	}
#define __decl_syscall_tpl__(...)\
	__for_each__(0)(__decl_syscall_tpl_op__, , __VA_ARGS__)

__decl_syscall_tpl__(__syscall_types_arch_os__());

#undef __decl_syscall_tpl__
#undef __decl_syscall_tpl_op__

// 定义syscall函数模板信息数组
#define __decl_syscall_info_arr__(elem_type, arr_name, exp_macro, ...)\
	static elem_type arr_name[__sizeof__(__VA_ARGS__)] =\
	{\
			__for_each__(0)(exp_macro, , __VA_ARGS__)\
	};

#define __decl_syscall_info_arr_op_no_offset__(i, begin_idx, end_idx, const_params, arch_os)\
	__syscall_no_offset__(arch_os),

inline static size_t syscall_no_offset(syscall_type type)
{
	__decl_syscall_info_arr__(const long, syscall_no_offset, __decl_syscall_info_arr_op_no_offset__, __syscall_types_arch_os__())
	return syscall_no_offset[type];
}

#define __decl_syscall_info_arr_op_cl_offset__(i, begin_idx, end_idx, const_params, arch_os)\
	__syscall_cl_offset__(arch_os),

inline static size_t syscall_cl_offset(syscall_type type)
{
	__decl_syscall_info_arr__(const long, syscall_cl_offset, __decl_syscall_info_arr_op_cl_offset__, __syscall_types_arch_os__())
	return syscall_cl_offset[type];
}

#define __decl_syscall_info_arr_op_tpl__(i, begin_idx, end_idx, const_params, arch_os)\
	(const void*)&__syscall_tpl__(arch_os),

inline static const void* syscall_tpl(syscall_type type)
{
	__decl_syscall_info_arr__(const void*, syscall_tpl, __decl_syscall_info_arr_op_tpl__, __syscall_types_arch_os__())
	return syscall_tpl[type];
}

syscall_t arch::syscall(syscall_type type, void* mem, long syscall_num, unsigned short syscall_clean)
{
	memcpy(mem, syscall_tpl(type), syscall_size(type)); //每个调用号都有自己的syscall函数内存
	*(long*)((char*)mem + syscall_no_offset(type)) = syscall_num; //替换模板中的syscall number
	if(syscall_cl_offset(type))
		*(unsigned short*)((char*)mem + syscall_cl_offset(type)) = syscall_clean; //如果需要自己清理栈，则填充清理的大小
	return (syscall_t)mem;
}

#define __decl_syscall_info_arr_op_size__(i, begin_idx, end_idx, const_params, arch_os)\
	__syscall_size__(arch_os),

size_t arch::syscall_size(syscall_type type)
{
	__decl_syscall_info_arr__(const long, syscall_size, __decl_syscall_info_arr_op_size__, __syscall_types_arch_os__())
	return syscall_size[type];
}
