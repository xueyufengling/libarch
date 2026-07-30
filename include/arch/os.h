#ifndef _ARCH_OS
#define _ARCH_OS

#include "arch.h"

// ***** 操作系统 *****

#if defined (_WIN32)

#define __os_win__ 1
#define __os_page_size__ 4096

#if defined (_WIN64)

#define __os_win64__ 1
#define __lp_size__ 8

#else

#define __os_win32__ 1
#define __lp_size__ 4

#endif

#endif

#if defined(__unix__) || defined(__unix) || defined(unix)

#define __os_unix__ 1
#define __os_page_size__ 4096

#if defined(__arch_x86_32__) || defined(__arch_aarch_32__)
#define __os_unix32__ 1
#define __lp_size__ 4
#elif defined(__arch_x86_64__) || defined(__arch_aarch_64__) || defined(__arch_powerpc_64__) || defined(__arch_alpha__)
#define __os_unix64__ 1
#define __lp_size__ 8
#endif

#endif

// ***** CPU架构+操作系统 *****

#if defined(__arch_x86_32__)

#if defined(__os_win32__)
#define __arch_os_x86_32_win32__ 1
#endif

#if defined(__os_unix32__)
#define __arch_os_x86_32_unix32__ 1
#endif

#endif

// x86_64
#if defined(__arch_x86_64__)

#if defined(__os_win64__)
#define __arch_os_x86_64_win64__ 1
#endif

#if defined(__os_win32__)
#define __arch_os_x86_64_win32__ 1
#endif

#if defined(__os_unix64__)
#define __arch_os_x86_64_unix64__ 1
#endif

#if defined(__os_unix32__)
#define __arch_os_x86_64_unix32__ 1
#endif

#endif

#if defined(__arch_os_x86_32_win32__) || defined(__arch_os_x86_64_win32__)
#define __arch_os_x86_win32__ 1
#endif

#endif //_ARCH_OS
