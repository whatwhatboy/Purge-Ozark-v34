#pragma once
#pragma warning(disable: 4996)
#pragma warning(disable: 4309)

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include <cassert>
#include <bitset>
#include <array>
#include <stdlib.h>


#include "util/hooking/callspoofer.h"
#pragma comment(lib, "Winmm.lib")

#pragma comment(lib, "libMinHook.x64.lib")
#pragma comment(lib, "VMProtectSDK64")
#undef min
#undef max



#define GET_XOR_KEYUI8  ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT8_MAX )
#define GET_XOR_KEYUI16 ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT16_MAX )
#define GET_XOR_KEYUI32 ( ( CONST_HASH( __FILE__ __TIMESTAMP__ ) + __LINE__ ) % UINT32_MAX )



#define VERSION 4
// #define DEV_MODE
// #define TESTER_MODE
#define RELEASE_MODE
#define MENU_BUILD_DATE __DATE__
#define MENU_BUILD_TIME __TIME__

#pragma message("========== Building Purge ==========")
#pragma message("Date -> " MENU_BUILD_DATE)
#pragma message("Time -> " MENU_BUILD_TIME)
#pragma message("======================================")

#ifdef DEV_MODE
#undef RELEASE_MODE
#undef TESTER_MODE
#define VERSION_TYPE "DEV"
#endif

#ifdef TESTER_MODE
#undef DEV_MODE
#undef RELEASE_MODE
#define VERSION_TYPE "TESTER"
#endif

#ifdef RELEASE_MODE
#undef DEV_MODE
#undef TESTER_MODE
#define VERSION_TYPE ""
#endif

#define TRANSLATE(name) name.get().c_str()

template<typename T, int N>
constexpr int NUMOF(T(&)[N]) { return N; }

template<typename T>
inline bool is_valid_ptr(T ptr) {
	uint64_t address = (uint64_t)ptr;
	if (address < 0x5000) return false;
	if ((address & 0xFFFFFFFF) == 0xFFFFFFFF) return false;
	if ((address & 0xFFFFFFFF) <= 0xFF) return false;

	if (*(uint8_t*)((uint64_t)&address + 6) != 0 || *(uint8_t*)((uint64_t)&address + 7) != 0) return false;

	return true;
}

#define is_valid_vtable(address) \
	[=]() -> bool { \
		if (!is_valid_ptr<uint64_t>(address)) return false; \
		return address > global::vars::g_game_address.first && address < (global::vars::g_game_address.first + global::vars::g_game_address.second); \
	}()

constexpr char CharacterMap[] = {
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_', '_',
	'_', '_', '_', '_', '_', '_', '_', '.', '/', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'_', '_',  '_', '_', '_', '_',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
};

constexpr static __forceinline uint32_t JenkinsHash32(const char* String, uint32_t CurrentHashValue = NULL) {
	while (*String != NULL) {
		CurrentHashValue += CharacterMap[*(String++)];
		CurrentHashValue += (CurrentHashValue << 10);
		CurrentHashValue ^= (CurrentHashValue >> 6);
	}

	CurrentHashValue += (CurrentHashValue << 3);
	CurrentHashValue ^= (CurrentHashValue >> 11);
	CurrentHashValue += (CurrentHashValue << 15);

	return CurrentHashValue;
}

#define joaat(String) \
    []( ) -> auto { \
        constexpr auto HashValue = JenkinsHash32( String ); \
        \
        return HashValue; \
    }( )


#pragma once

#include <string>
#include <utility>

namespace
{
	constexpr int const_atoi(char c)
	{
		return c - '0';
	}
}

#ifdef _MSC_VER
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE __attribute__((always_inline))
#endif

template<typename _string_type, size_t _length>
class _Basic_XorStr
{
	using value_type = typename _string_type::value_type;
	static constexpr auto _length_minus_one = _length - 1;

public:
	constexpr ALWAYS_INLINE _Basic_XorStr(value_type const (&str)[_length])
		: _Basic_XorStr(str, std::make_index_sequence<_length_minus_one>())
	{

	}

	inline auto c_str() const
	{
		decrypt();

		return data;
	}

	inline auto str() const
	{
		decrypt();

		return _string_type(data, data + _length_minus_one);
	}

	inline operator _string_type() const
	{
		return str();
	}

private:
	template<size_t... indices>
	constexpr ALWAYS_INLINE _Basic_XorStr(value_type const (&str)[_length], std::index_sequence<indices...>)
		: data{ crypt(str[indices], indices)..., '\0' },
		encrypted(true)
	{

	}

	static constexpr auto XOR_KEY = static_cast<value_type>(
		const_atoi(__TIME__[7]) +
		const_atoi(__TIME__[6]) * 10 +
		const_atoi(__TIME__[4]) * 60 +
		const_atoi(__TIME__[3]) * 600 +
		const_atoi(__TIME__[1]) * 3600 +
		const_atoi(__TIME__[0]) * 36000
		);

	static ALWAYS_INLINE constexpr auto crypt(value_type c, size_t i)
	{
		return static_cast<value_type>(c ^ (XOR_KEY + i));
	}

	inline void decrypt() const
	{
		if (encrypted)
		{
			for (size_t t = 0; t < _length_minus_one; t++)
			{
				data[t] = crypt(data[t], t);
			}
			encrypted = false;
		}
	}

	mutable value_type data[_length];
	mutable bool encrypted;
};
//---------------------------------------------------------------------------
template<size_t _length>
using XorStrA = _Basic_XorStr<std::string, _length>;
template<size_t _length>
using XorStrW = _Basic_XorStr<std::wstring, _length>;
template<size_t _length>
using XorStrU16 = _Basic_XorStr<std::u16string, _length>;
template<size_t _length>
using XorStrU32 = _Basic_XorStr<std::u32string, _length>;
//---------------------------------------------------------------------------
template<typename _string_type, size_t _length, size_t _length2>
inline auto operator==(const _Basic_XorStr<_string_type, _length>& lhs, const _Basic_XorStr<_string_type, _length2>& rhs)
{
	static_assert(_length == _length2, "XorStr== different length");

	return _length == _length2 && lhs.str() == rhs.str();
}
//---------------------------------------------------------------------------
template<typename _string_type, size_t _length>
inline auto operator==(const _string_type& lhs, const _Basic_XorStr<_string_type, _length>& rhs)
{
	return lhs.size() == _length && lhs == rhs.str();
}
//---------------------------------------------------------------------------
template<typename _stream_type, typename _string_type, size_t _length>
inline auto& operator<<(_stream_type& lhs, const _Basic_XorStr<_string_type, _length>& rhs)
{
	lhs << rhs.c_str();

	return lhs;
}
//---------------------------------------------------------------------------
template<typename _string_type, size_t _length, size_t _length2>
inline auto operator+(const _Basic_XorStr<_string_type, _length>& lhs, const _Basic_XorStr<_string_type, _length2>& rhs)
{
	return lhs.str() + rhs.str();
}
//---------------------------------------------------------------------------
template<typename _string_type, size_t _length>
inline auto operator+(const _string_type& lhs, const _Basic_XorStr<_string_type, _length>& rhs)
{
	return lhs + rhs.str();
}
//---------------------------------------------------------------------------
template<size_t _length>
constexpr ALWAYS_INLINE auto XOR(char const (&str)[_length])
{
	return XorStrA<_length>(str);
}
//---------------------------------------------------------------------------
template<size_t _length>
constexpr ALWAYS_INLINE auto XOR(wchar_t const (&str)[_length])
{
	return XorStrW<_length>(str);
}
//---------------------------------------------------------------------------
template<size_t _length>
constexpr ALWAYS_INLINE auto XOR(char16_t const (&str)[_length])
{
	return XorStrU16<_length>(str);
}
//---------------------------------------------------------------------------
template<size_t _length>
constexpr ALWAYS_INLINE auto XOR(char32_t const (&str)[_length])
{
	return XorStrU32<_length>(str);
}
//---------------------------------------------------------------------------