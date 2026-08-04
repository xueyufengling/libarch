#ifndef _ARCH_SYSCALLDEFS
#define _ARCH_SYSCALLDEFS

#include <arch/asm.h>

// 系统调用号的占位符，运行时需要替换该占位符为正确的调用号
#define __syscall_no_placeholder__() (('s' << 24) | ('c' << 16) |  ('n' << 8) | 'o')

#define __syscall_clean_placeholder__() (0xc1ea)

// syscall模板函数名称
#define __syscall_tpl__(arch_os) __cat__(2, __syscall_tpl_, arch_os)

// 调用号的偏移量
#define __syscall_no_offset__(arch_os) __cat__(2, __syscall_no_offset_, arch_os)
#define __syscall_no_offset_val__(arch_os) __cat__(2, __syscall_no_offset_val_, arch_os)

// syscall函数机器码总字节长度
#define __syscall_size__(arch_os) __cat__(2, __syscall_size_, arch_os)
#define __syscall_size_val__(arch_os) __cat__(2, __syscall_size_val_, arch_os)

// 清理栈的栈偏移量，仅win32需要被调用者清理栈
#define __syscall_cl_offset__(arch_os) __cat__(2, __syscall_cl_offset_, arch_os)
#define __syscall_cl_offset_val__(arch_os) __cat__(2, __syscall_cl_offset_val_, arch_os)

// 记录当前位置向后n字节的偏移量
#define __offset_markb__(arch_os, out, n) .equ out, . - __syscall_tpl__(arch_os) + n
// 标记函数大小
#define __syscall_endp__(arch_os) __offset_markb__(arch_os, __syscall_size_val__(arch_os), 0)
// 标记调用号偏移量
#define __syscall_noff__(arch_os, syscall_no_size) __offset_markb__(arch_os, __syscall_no_offset_val__(arch_os), -syscall_no_size)
// 标记不需要被调用者清理栈
#define __caller_clean__(arch_os) .equ __syscall_cl_offset_val__(arch_os), 0
// 标记被调用者自行清理栈
#define __callee_clean__(arch_os) __offset_markb__(arch_os, __syscall_cl_offset_val__(arch_os), -2)

#define __win_kuser_shared_data__() 0x7FFE0300
#define __win_syscall_test_offset__() 8

#define __syscall_def__() "incl/syscall_def.S.inc"

#define __syscall_not_available__() "incl/syscall_not_available.S.inc"

#endif //_ARCH_SYSCALLDEFS
