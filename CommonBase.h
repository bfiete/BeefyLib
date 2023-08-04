#pragma once

#include <algorithm>
#include <functional>

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef unsigned int uint;

typedef intptr_t intptr;
typedef uintptr_t uintptr;

#pragma once

#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <algorithm>
#include <functional>

inline size_t HashBytes(const uint8* ptr, size_t count) noexcept
{
#ifdef BF64
	const size_t _FNV_offset_basis = 14695981039346656037ULL;
	const size_t _FNV_prime = 1099511628211ULL;
#else
	const size_t _FNV_offset_basis = 2166136261U;
	const size_t _FNV_prime = 16777619U;
#endif

	size_t val = _FNV_offset_basis;
	for (size_t _Next = 0; _Next < count; ++_Next)
	{
		val ^= (size_t)ptr[_Next];
		val *= _FNV_prime;
	}
	return (val);
}

template <typename T>
struct BeefHash : std::hash<T>
{
};

template<class T>
struct BeefHash<T*>
{
	size_t operator()(T* val) const
	{
		return (size_t)val ^ (size_t)val >> 11;
	}
};

template <>
struct BeefHash<int>
{
	size_t operator()(int val)
	{
		return (size_t)val;
	}
};

template <>
struct BeefHash<int64>
{
	size_t operator()(int64 val)
	{
		return (size_t)val;
	}
};

struct uint128
{
	uint64 mLo;
	uint64 mHigh;
};

struct int128
{
	uint64 mLo;
	uint64 mHigh;
};

#define BF_MIN(x, y) (((x) < (y)) ? (x) : (y))
#define BF_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define BF_CLAMP(val, minVal, maxVal) (((val) < (minVal)) ? (minVal) : ((val) > (maxVal)) ? (maxVal) : (val))
#define BF_SWAP(a, b) { auto _a = (a); (a) = (b); (b) = (_a); }

extern int gBFArgC;
extern char** gBFArgV;

#define NS_BF_BEGIN namespace Beefy {
#define NS_BF_END }
#define USING_NS_BF using namespace Beefy
#define BF_ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))
#define BF_CLEAR_VALUE(val) memset(&val, 0, sizeof(val))
#define BF_ALIGN(intVal, alignSize) (((intVal) + ((alignSize) - 1)) & ~((alignSize) - 1))
#define BF_DISALLOW_COPY(name) name(const name& from) = delete;

#if defined _DEBUG || defined BF_DEBUG_ASSERTS
#define BF_ASSERT(_Expression) (void)( (!!(_Expression)) || (Beefy::BFFatalError(#_Expression, __FILE__, __LINE__), 0) )
#else
#define BF_ASSERT(_Expression) (void)(0)
#endif

#define BF_FATAL(msg) (void) ((Beefy::BFFatalError(msg, __FILE__, __LINE__), 0) )

#define BF_ASSERT_CONCAT_(a, b) a##b
#define BF_ASSERT_CONCAT(a, b) BF_ASSERT_CONCAT_(a, b)
#ifdef __COUNTER__
#define BF_STATIC_ASSERT(e) \
    ;enum { BF_ASSERT_CONCAT(static_assert_, __COUNTER__) = 1/(int)(!!(e)) }
#else
#define BF_STATIC_ASSERT(e) \
    ;enum { BF_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(int)(!!(e)) }
#endif

NS_BF_BEGIN;

#ifndef max
template <typename T>
constexpr const T& Max(const T& lhs, const T& rhs) noexcept
{
	return (lhs < rhs) ? rhs : lhs;
}

template <typename T>
constexpr const T& Min(const T& lhs, const T& rhs) noexcept
{
	return (lhs < rhs) ? lhs : rhs;
}
#endif

class StringImpl;

template <const int TBufSize>
class StringT;
class UTF16String;

typedef StringT<16> String;

#ifdef BF64
#define V_32_64(v32, v64) v64
#else
#define V_32_64(v32, v64) v32
#endif

#define BF_PI 3.14159265359f
#define BF_PI_D 3.14159265359

inline float BFRound(float aVal)
{
	if (aVal < 0)
		return (float)(int)(aVal - 0.5f);
	else
		return (float)(int)(aVal + 0.5f);
}

inline float BFClamp(float val, float min, float max)
{
	return (val <= min) ? min : (val >= max) ? max : val;
}

inline int BFClamp(int val, int min, int max)
{
	return (val <= min) ? min : (val >= max) ? max : val;
}

// uint32 BFTickCount();
// void BFFatalError(const char* message, const char* file, int line);
// void BFFatalError(const StringImpl& message, const StringImpl& file, int line);

template<typename T>
struct RemoveTypePointer
{
};

template<typename T>
struct RemoveTypePointer<T*>
{
	typedef T type;
};

#ifndef BF_SMALL
template <typename F>
struct BF_Defer {
	F f;
	BF_Defer(F f) : f(f) {}
	~BF_Defer() { f(); }
};

template <typename F>
BF_Defer<F> BF_defer_func(F f) {
	return BF_Defer<F>(f);
}

#define DEFER_1(x, y) x##y
#define DEFER_2(x, y) DEFER_1(x, y)
#define DEFER_3(x)    DEFER_2(x, __COUNTER__)
#define defer(code)   auto DEFER_3(_defer_) = BF_defer_func([&](){code;})
#endif //BF_SMALL

void BFFatalError(const char* message, const char* file, int line);

NS_BF_END
