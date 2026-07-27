#ifndef _ARCH_ARCH
#define _ARCH_ARCH

// ***** CPU架构 *****

#if defined(__alpha__) || defined(__alpha) || defined(_M_ALPHA)
#define __arch_alpha__ 1
#define __arch_reg_size__ 8
#endif

//x86 64位
#if defined(__x86_64__) || defined(__x86_64) || defined(_M_X64) || defined(__amd64__) || defined(__amd64)
#define __arch_x86_64__ 1
#define __arch_reg_size__ 8
#endif

//x86 32位
#if defined(__i386__) || defined(__i386) || defined(i386) || defined(_M_IX86) || defined(_M_I86) || defined(__IA32__) || defined(__X86__) || defined(_X86_)
#define __arch_x86_32__ 1
#define __arch_reg_size__ 4
#endif

#if defined(__arch_x86_32__) || defined(__arch_x86_64__)
#define __arch_x86__ 1
#endif

#if (defined(__arm__) || defined(_M_ARM))
#define __arch_arm__ 1
//ARM 64位
#if defined(__aarch64__) || defined(_M_ARM64)
#define __arch_aarch_64__ 1
#define __arch_reg_size__ 8
#else
//ARM 32位
#define __arch_aarch_32__ 1
#define __arch_reg_size__ 4
#endif
#endif

#if defined(__powerpc) || defined(__powerpc__) || defined(__powerpc64__) || defined(__POWERPC__) || defined(__ppc__) || defined(__ppc64__) || defined(__PPC__) || defined(__PPC64__) || defined(_ARCH_PPC) || defined(_ARCH_PPC64) || defined(_M_PPC)
#define __arch_powerpc_64__ 1
#define __arch_reg_size__ 8
#endif

#endif //_ARCH_ARCH
