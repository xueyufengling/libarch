#include "syscall_defs.h"

#include <arch/arch.h>

#如果CPU架构不匹配，则只保留符号不生成机器码
#define __arch_os__() x86_win32
#include __syscall_def__()
#if defined(__arch_x86__) || defined(__arch_x86_64__)
#stdcall，参数全压栈
	movl $__syscall_no_placeholder__(), %eax
	__syscall_noff__(x86_win32, 4)
	testb $0x1, %ds:__win_kuser_shared_data__() + __win_syscall_test_offset__()
	jnz 1f
	sysenter
	jmp 2f
1:
	int $0x2E
2:
	ret $__syscall_clean_placeholder__()												#stdcall需要被调用者清理栈，执行后esp+=clean_size
	__callee_clean__(x86_win32)														#ret指令接收2字节整数作为清理栈的大小
	__syscall_endp__(x86_win32)
#else
#define __arch_os__() x86_win32
#include __syscall_not_available__()
#endif

#位置无关的syscall调用函数，参考ntdll.dll反汇编结果
#define __arch_os__() x86_64_win64
#include __syscall_def__()
#if defined(__arch_x86__) || defined(__arch_x86_64__)
	movq %rcx, %r10																		#Windows调用约定参数为rcx, rdx, r8, r9，剩余参数压栈；syscall调用约定为r10, rdx, r8, r9，剩余参数压栈.因此需要手动将函数参数rcx移入syscall参数寄存器r10
	movl $__syscall_no_placeholder__(), %eax											#移入调用号
	__syscall_noff__(x86_64_win64, 4)													#记录syscall number偏移量
	testb $0x1, %ds:__win_kuser_shared_data__() + __win_syscall_test_offset__()			#检查DS段寄存器0x7FFE0308位置处的数据_KUSER_SHARED_DATA.SystemCall与0x1的与运算结果，即检查最低位是否为0
	jnz 1f																				#最低位ZF非0，则跳转到中断实现的syscall，而非直接使用syscall指令
	syscall																				#现代使用syscall指令进行系统调用
	retq
1:
	int $0x2E																			#旧时代遗留的兼容方案，使用中断进行系统调用，中断号固定为0x2E
	retq
	__caller_clean__(x86_64_win64)														#x64 windows不需要被调用者清理栈
	__syscall_endp__(x86_64_win64)
#else
#define __arch_os__() x86_64_win64
#include __syscall_not_available__()
#endif
