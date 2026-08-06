#ifndef _ARCH_BITFLAG
#define _ARCH_BITFLAG

namespace arch
{
template<typename _T>
inline constexpr _T bit_flag(int bit)
{
	return 0x1 << bit;
}

template<typename _T>
inline constexpr bool flag_bit(_T flags, _T bit_flag)
{
	return flags & bit_flag;
}

template<typename _T>
inline constexpr _T set_flag_bit(_T flags, _T flag, bool mark)
{
	return mark ? flags | flag : flags & (~flag);
}
}

#endif//_ARCH_BITFLAG
