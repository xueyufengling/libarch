#ifndef _ARCH_CALL
#define _ARCH_CALL

#include <stdint.h>

#include "arch.h"
#include "asm_def.h"

// 操作系统保留内存，所有系统均保留第一页（地址0~4095）
#if !defined(__os_reserved_addr__)
#define __os_reserved_addr__() ((void*)0x0FFF)
#endif

namespace arch
{
/**
 * @brief 从Instruction Pointer获取该指令所属的函数的函数指针地址
 */
extern void* enclosing_function(void* ip);

/**
 * @brief 机器的栈，栈是反向生长的，栈顶在低地址，栈底在高地址
 */
namespace machine_stack
{
/**
 * @brief 栈帧
 */
#pragma pack(push, 1)
struct frame
{
	frame* caller; //当前读到的rbp寄存器值，指向上一个栈帧的栈底
	void* ret_ip; //高地址，实际对应栈中prev下方的值，即返回地址，该地址是caller调用函数结束后的下一条指令地址，即caller的ip

	//不允许构造和析构，只能通过读取rbp寄存器的值来解析栈
	frame() = delete;
	~frame() = delete;

	/**
	 * @brief 栈帧的局部变量区域，使用字节作为偏移量单位。局部变量按照声明顺序从高地址到低地址排列。
	 */
	__attribute__((always_inline)) inline void* local_addr(long long offset)
	{
		return ((unsigned char*)this - offset);
	}

	__attribute__((always_inline)) inline long long caller_size()
	{
		return (unsigned char*)caller - (unsigned char*)this; //两个栈底之间的距离即栈帧空间
	}
};
#pragma pack(pop)

/**
 * @brief 当前栈底，即当前的栈帧。当前栈底的地址正是caller栈顶的地址。
 */
__attribute__((always_inline)) inline frame* __bp()
{
	frame* bottom;
#if defined(__arch_x86_64__)
	__stq__(bottom, %rbp);
#elif defined(__arch_x86__)
	__stl__(bottom, %ebp);
#elif defined(__arch_aarch_64__)
	__stq__(bottom, %fp);
#endif
	return bottom;
}

/**
 * @brief 栈顶，在push %rbp;压栈保存caller栈地址后，会先压栈调用前要保存的寄存器值，以便执行完毕后恢复现场，在此期间rsp寄存器的栈底持续增长，这部分是函数序言。
 * 		  函数序言之后，才为局部变量分配内存。栈帧内的内存是连续的，局部变量、保存的现场寄存器值、rbp、返回值地址都在栈上。
 * 		  函数结束返回时进入函数尾声，mov %ebp, %esp;将结束的这个函数栈帧的栈底ebp作为caller的栈顶esp。
 */
__attribute__((always_inline)) inline void* __sp()
{
	void* top;
#if defined(__arch_x86_64__)
	__stq__(top, %rsp);
#elif defined(__arch_x86__)
	__stl__(top, %esp);
#elif defined(__arch_aarch_64__)
	__stq__(top, %sp);
#endif
	return top;
}

/**
 * @brief 当前执行的栈帧的当前长度，rsp寄存器在调用过程中会随着函数序言压栈现场寄存器值和分配局部变量内存而增长，进入函数本体后保持不变。
 */
__attribute__((always_inline)) inline long long callee_size()
{
	return (unsigned char*)__bp() - (unsigned char*)__sp();
}

}

template<size_t _MaxDepth>
class call_stack;

class stack_frame
{
	template<size_t _MaxDepth>
	friend class call_stack;

private:
	size_t frame_depth;
	void** bp_addr;
	void** ip_addr;

public:
	inline size_t depth()
	{
		return frame_depth;
	}

	/**
	 * @brief 栈帧rbp。
	 */
	inline void* bp()
	{
		return *bp_addr;
	}

	inline void set_bp(void* bp)
	{
		if(frame_depth)
			*bp_addr = bp;
	}

	/**
	 * @brief 栈帧ip。如果是depth=0的顶层栈帧，则是调用call_stack构造函数处的ip
	 */
	inline void* ip()
	{
		// https://gcc.gnu.org/onlinedocs/gcc/Return-Address.html
		// 一些机器上储存的返回地址经过了编码，需要使用内建函数解码真实返回地址。
		return __builtin_extract_return_addr(*ip_addr);
	}

	inline void set_ip(void* ip)
	{
		if(frame_depth)
			*ip_addr = ip; //顶层栈帧不能设置ip。当从构造函数返回回到顶层栈帧时，顶层栈帧的ip已经从栈中弹出。
	}
};

/**
 * @brief 栈帧回溯
 */
template<size_t _UnwindDepth>
class call_stack
{
private:
	size_t frame_count = 0;  //实际的栈帧数目
	stack_frame frames[_UnwindDepth]; //栈帧数组

public:
	static constexpr size_t unwind_depth = _UnwindDepth;

	/**
	 * @brief 栈帧回溯。回溯得到的顶部栈帧是调用此构造函数的函数，构造函数本身的栈帧不包含在内。
	 * 		  得到的call_stack对象仅在调用此构造函数的函数内有效，随着外部函数的逐级返回，栈帧数据从顶至底依次失效。
	 * @detail 强制关闭内联且必须生成栈帧，关闭优化，防止构造函数没有栈帧导致错误。
	 * 		  如果不强制保留栈帧指针rbp，则构造函数序言为
	 * 		  push    rsi
	 * 		  push    rbx
	 * 		  sub     rsp, 38h
	 * 		  此种形式未将rbp压栈，而是直接移动rsp，将导致rbp依然是caller的rbp，但ip却是该构造函数的ip
	 */
	__attribute__((noinline, optimize("no-omit-frame-pointer"), optimize("O0"))) call_stack()
	{
		machine_stack::frame* machine_stack_frame = machine_stack::__bp(); //本构造函数的栈帧
		while(frame_count < unwind_depth)
		{
			stack_frame& unwind_frame = frames[frame_count];
			unwind_frame.frame_depth = frame_count;
			unwind_frame.bp_addr = (void**)&machine_stack_frame->caller;
			unwind_frame.ip_addr = &machine_stack_frame->ret_ip;
			++frame_count;
			// rbp < __os_reserved_addr__()则代表当前已经是最底层的栈帧
			machine_stack::frame* caller = machine_stack_frame->caller;
			if(caller > __os_reserved_addr__() && machine_stack_frame->ret_ip)
			{
				machine_stack_frame = caller; //如果该栈帧还有caller则继续迭代
			}
			else
			{
				break;
			}
		}
	}

	inline size_t count()
	{
		return frame_count;
	}

	/**
	 * @brief 获取指定栈深度的栈帧函数指针
	 */
	inline stack_frame& operator[](size_t depth)
	{
		return frames[depth];
	}

	inline void* ip(size_t depth)
	{
		return frames[depth].ip();
	}

	inline void* function(size_t depth)
	{
		return enclosing_function(ip(depth));
	}
};

}

#endif //_ARCH_CALL
