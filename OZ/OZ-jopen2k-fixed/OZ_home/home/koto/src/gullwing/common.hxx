// common.hxx
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(COMMON_HXX__INCLUDED) // {
#define COMMON_HXX__INCLUDED
/////////////////////////////////////////////////////////////////////////////

#define UNIX		1
#define WINDOWS		2

#ifdef _MSC_VER
#  pragma warning ( disable: 4511 )	// 宰澎 際縦弩B
#  pragma warning ( disable: 4512 )	// 代入演算子を生成できません。
#  pragma warning ( disable: 4710 )	// 関数 '%s' は課弉狽
#  pragma warning ( disable: 4786 )	// ッ別子が '255' 文字に切り捨てられました (browser 情報内)。
#  ifndef OS_TYPE
#    define OS_TYPE		WINDOWS
#  endif
#  ifdef _MT
#    ifndef MULTI_THREAD
#      define MULTI_THREAD 1
#    endif
#    ifndef WITH_WINDOW
#      define WITH_WINDOW 1
#    endif
#  else
#    ifndef MULTI_THREAD
#      define MULTI_THREAD 0
#    endif
#    ifndef WITH_WINDOW
#      define WITH_WINDOW 0
#    endif
#  endif
#endif


#ifndef OS_TYPE
#  define OS_TYPE		UNIX
#endif
#ifndef MULTI_THREAD
#  define MULTI_THREAD 0
#endif
#ifndef WITH_WINDOW
#  define WITH_WINDOW 0
#endif
#ifndef LEARNING
#  define LEARNING 0
#endif

#ifndef NEVER_THROW
#  define NEVER_THROW 0
#endif

#if OS_TYPE == UNIX
#  if MULTI_THREAD || WITH_WINDOW
#    error
#  endif
#endif

///////////////////////////////////////////////////////////////////////

#include <assert.h>

/*inline void ASSERT(bool f) {
assert(f);
}*/
#define ASSERT		assert /* */
//#define ASSERT(l)

#ifdef NDEBUG
#define REPORT		true ? (void)0 : printf
#else
#define REPORT		false ? (void)0 : printf
#endif

#define report		printf

/////////////////////////////////////////////////////////////////////////////

// 標準ライブラリヘッダの二重インクルードの安全性が、仕様で保証されているか不 
// 明のため、ここで無差別に #include する。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <float.h>
#include <stdexcept>
#if NEVER_THROW
#  include <setjmp.h>
#endif

#include <string>
#include <list>
#include <vector>
#include <set>
#include <map>

#if OS_TYPE == UNIX

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define CLOSESOCKET(sock)   close(sock)
//typedef int socklen_t;

#elif OS_TYPE == WINDOWS

#define STRICT
//#include <windows.h>
#include <winsock2.h>
#define CLOSESOCKET(sock)   closesocket(sock)
typedef int socklen_t;

#else
#error
#endif

#ifdef _MSC_VER
#  pragma warning ( disable: 4511 )	// 宰澎 際縦弩B
#  pragma warning ( disable: 4512 )	// 代入演算子を生成できません。
#endif

///////////////////////////////////////////////////////////////////////

#define COUNT_OF(vector) (sizeof(vector)/sizeof((vector)[0]))

#undef max
#undef min
template <class T>
inline T max(T x, T y)
{
	return (x >= y) ? x : y;
}
template <class T>
inline T min(T x, T y)
{
	return (x <= y) ? x : y;
}
template <class T>
inline T suppress(T min, T x, T max)
{
	ASSERT(min <= max);
	return (x < min) ? min : ((max < x) ? max : x);
}
template <class T>
inline T abs(T x)
{
	return (x >= 0) ? x : -x;
}

inline int getBits(int value)
{
	int result = 0;
	while(value) {
		value = value >> 1;
		result++;
	}
	return result;
}

/////////////////////////////////////////////////////////////////////////////
#endif // } !defined(COMMON_HXX__INCLUDED)
