#ifndef kode_attributes_included
#define kode_attributes_included
//----------------------------------------------------------------------

//------------------------------
// win32
// dllexport, dllimport, cdecl, stdcall
//------------------------------

#ifdef KODE_LINUX
  #define __KODE_DLLEXPORT
  #define __KODE_DLLIMPORT
  #define __KODE_CDECL
  #define __KODE_STDCALL
#endif

//#ifdef KODE_WIN32
//  #define __KODE_DLLEXPORT __attribute__ ((dllexport))
//  #define __KODE_DLLIMPORT __attribute__ ((dllimport))
//  #define __KODE_CDECL     __attribute__ ((cdecl))
//  #define __KODE_STDCALL   __attribute__ ((stdcall))
//#endif

//------------------------------
// linux
// dll public/local (-fvisibility=hidden)
//------------------------------

#ifdef KODE_LINUX
  #define __KODE_DLLPUBLIC __attribute__ ((visibility ("default")))
  #define __KODE_DLLLOCAL  __attribute__ ((visibility ("hidden")))
  #undef  __KODE_DLLEXPORT
  #define __KODE_DLLEXPORT __KODE_DLLPUBLIC
#endif

//#ifdef KODE_WIN32
//  #define __KODE_DLLPUBLIC
//  #define __KODE_DLLLOCAL
//#endif

//------------------------------
// attributes
//------------------------------

#define __KODE_ALIAS(name)         __attribute__ ((alias (name)))
#define __KODE_ALIGNED(x)          __attribute__ ((aligned (x)))
#define __KODE_ALIGNEDMAX          __attribute__ ((aligned))
#define __KODE_ALLOC_ALIGN(x)      __attribute__ ((alloc_align(x)))
#define __KODE_ALLOC_SIZE1(x)      __attribute__ ((alloc_size(x)))
#define __KODE_ALLOC_SIZE2(x,y)    __attribute__ ((alloc_size(x,y)))
#define __KODE_ALWAYS_INLINE       inline __attribute__ ((always_inline))
#define __KODE_ASM_VOLATILE        __asm__ __volatile__
#define __KODE_CLEANUP(x)          __attribute__ ((__cleanup__(x)))
#define __KODE_COLD                __attribute__ ((cold))
#define __KODE_CONSTRUCTOR         __attribute__ ((constructor))
//#define __KODE_CONSTRUCTOR_PRI(x)  __attribute__ ((constructor(x)))
#define __KODE_DEPRECATED          __attribute__ ((deprecated))
#define __KODE_DESTRUCTOR          __attribute__ ((destructor))
#define __KODE_ERROR(message)      __attribute__ ((error(message)))
#define __KODE_EXTERNC             extern "C"
#define __KODE_FLATTEN             __attribute__ ((flatten))
#define __KODE_HOT                 __attribute__ ((hot))
#define __KODE_HOTINLINE           ___always_inline __hot __optimize(3)
#define __KODE_INIT_PRIORITY(x)    __attribute__ ((init_priority(x)))
#define __KODE_MALLOC              __attribute__ ((malloc))
#define __KODE_MAY_ALIAS           __attribute__ ((may_alias))
#define __KODE_NO_REORDER          __attribute__ ((no_reorder))
#define __KODE_NOINLINE            __attribute__ ((noinline))
#define __KODE_NORETURN            __attribute__ ((noreturn))
#define __KODE_OPTIMIZE(level)     __attribute__ ((optimize (level)))
#define __KODE_PACKED              __attribute__ ((packed))
#define __KODE_PURE                __attribute__ ((pure))
#define __KODE_SHARED              __attribute__ ((shared))
#define __KODE_SECTION(name)       __attribute__ ((section (name)))
#define __KODE_SELECTANY           __attribute__ ((selectany))
#define __KODE_TARGET(value)       __attribute__ ((target(value)))
#define __KODE_TRANSPARENT_UNION   __attribute__ ((transparent_union))
#define __KODE_USED                __attribute__ ((used))
#define __KODE_UNUSED              __attribute__ ((unused))
#define __KODE_VECTOR(x)           __attribute__ ((vector_size (x)))
#define __KODE_VOLATILE            __volatile__
#define __KODE_VISIBILITY(type)    __attribute__ ((visibility(type)))
#define __KODE_WARNING(message)    __attribute__ ((warning(message)))
#define __KODE_WEAK                __attribute__ ((weak))
#define __KODE_WEAKREF(name)       __attribute__ ((weakref(name)))

#define __KODE_LIKELY(expr)        __builtin_expect(!!(expr),1)
#define __KODE_UNLIKELY(expr)      __builtin_expect(!!(expr),0)

//------------------------------

//#define __KODE_LIKELY(expr)        (expr)
//#define __KODE_UNLIKELY(expr)      (expr)

//----------------------------------------------------------------------
#endif
