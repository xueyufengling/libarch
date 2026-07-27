#ifndef _ARCH_CC
#define _ARCH_CC

// ***** 编译器类型 *****

#if defined(_MSC_VER)

#define __cc_msvc_compatible__ 1

#if defined(__INTEL_COMPILER)
#define __cc_icc__ 1
#else
// MSVC(Visual Studio)
#define __cc_msvc__ 1
#endif

#endif

#if defined(__GNUC__)

#define __cc_gcc_compatible__ 1

#if defined(__clang__)
// Clang(Xcode, Android NDK, LLVM)
// Clang会同时定义__GNUC__和__clang__
#define __cc_clang__ 1
#elif defined(__INTEL_COMPILER)
// Intel ICC也会定义__GNUC__
#define __cc_icc__ 1
#else
#define __cc_gcc__ 1
#endif

#endif

#endif//_ARCH_CC
