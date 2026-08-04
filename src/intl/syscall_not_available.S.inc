#include "../syscall_defs.h"

/**
 * 当前不是目标CPU架构时，将导出的模板数据全部置零
 */
 .type __syscall_no_offset_val__(__arch_os__()), @object
__syscall_no_offset_val__(__arch_os__()):
	.quad 0

.type __syscall_size_val__(__arch_os__()), @object
__syscall_size_val__(__arch_os__()):
	.quad 0

.type __syscall_cl_offset_val__(__arch_os__()), @object
__syscall_cl_offset_val__(__arch_os__()):
	.quad 0

#undef __arch_os__
