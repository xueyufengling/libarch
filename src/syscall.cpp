#include <arch/syscall.h>

#include "syscall_defs.h"

using namespace arch;

#if defined(__arch_os_x86_64_win64__)
const syscall_type arch::host_syscall_type = syscall_type::syscall_type_x86_64_win64;
#elif defined(__arch_os_x86_win32__)
const syscall_type arch::host_syscall_type = syscall_type::syscall_type_x86_win32;
#endif

// syscall_tpl.S导出符号
#define __decl_syscall_tpl_info_op__(i, begin_idx, end_idx, const_params, arch_os)\
	extern "C"\
	{\
	extern const long __syscall_no_offset__(arch_os);\
	extern const long __syscall_size__(arch_os);\
	extern const long __syscall_cl_offset__(arch_os);\
	extern long __syscall_tpl__(arch_os)(...);\
	}
#define __decl_syscall_tpl_info__(...)\
	__for_each__(0)(__decl_syscall_tpl_info_op__, , __VA_ARGS__)

__decl_syscall_tpl_info__(__syscall_types_arch_os__());

#undef __decl_syscall_tpl_info__
#undef __decl_syscall_tpl_info_op__

// 定义syscall函数模板信息数组

#define __decl_syscall_info_arr_elem__(i, begin_idx, end_idx, ret_type, info_name, arch_os)\
	__cat__(3, __syscall_, info_name, __)(arch_os)

#define __def_syscall_info_func_op_intl__(func_prefix, ret_type, info_name)\
	ret_type ::arch::__cat__(2, func_prefix, info_name)(::arch::syscall_type type) noexcept\
	{\
		static __exp_array_def__(1, ret_type, info_name, __decl_syscall_info_arr_elem__, __pack_list__(ret_type, info_name), __syscall_types_arch_os__())\
		return info_name[type];\
	}

#define __def_syscall_info_func_op__(i, begin_idx, end_idx, func_prefix, info)\
	__call_exp__(0)(__def_syscall_info_func_op_intl__, func_prefix, __unpack__(info))

#define __def_syscall_info_func__(func_prefix, ...)\
	__for_each__(0)(__def_syscall_info_func_op__, func_prefix, __VA_ARGS__)

__def_syscall_info_func__(syscall_, __syscall_info_names__())

#undef __def_syscall_info_func__
#undef __def_syscall_info_func_op__
#undef __def_syscall_info_func_op_intl__
#undef __decl_syscall_info_arr_elem__
