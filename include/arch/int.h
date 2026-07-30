#ifndef _ARCH_INT
#define _ARCH_INT

#include <stdint.h>

namespace arch
{
/**
 * @brief CPU硬中断
 */
namespace _int
{
// Interrupt Descriptor Table
typedef struct idt
{
	uint16_t limit = 0;
	void* base = nullptr;
} __attribute__((packed)) idt;

__attribute__((always_inline)) inline void __sidt(idt* t)
{
#if defined(__ARCH_X86__)
	__stidt__(t);
#elif defined(__ARCH_AARCH_64__)
#endif
}

}
}

#endif//_ARCH_INT
