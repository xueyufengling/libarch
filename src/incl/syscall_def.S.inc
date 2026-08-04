#include "../syscall_defs.h"

/**
 * 定义.rodata的导出数据，如果在全局数组中静态初始化，需要手动指定.type和.size，否则编译时找不到直接填0
 */
.section .rodata
.globl __syscall_no_offset__(__arch_os__())
__syscall_no_offset__(__arch_os__()):
	.quad (__syscall_no_offset_val__(__arch_os__()))

.globl __syscall_size__(__arch_os__())
__syscall_size__(__arch_os__()):
	.quad (__syscall_size_val__(__arch_os__()))

.globl __syscall_cl_offset__(__arch_os__())
__syscall_cl_offset__(__arch_os__()):
	.quad (__syscall_cl_offset_val__(__arch_os__()))

/**
 * 定义一个syscall模板，需要拷贝其机器码并填充syscall num到可执行内存中执行
 * 导出了汇编时常量和syscall函数的符号
 */
.section .text
.globl __syscall_tpl__(__arch_os__())
__syscall_tpl__(__arch_os__()):

#undef __arch_os__
