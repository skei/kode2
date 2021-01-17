#ifndef kode_debug_assert_included
#define kode_debug_assert_included
//----------------------------------------------------------------------

#ifdef KODE_DEBUG_ASSERT

  #include <assert.h>

  // c++11 : static_assert(x,"msg");
  //#include <cassert>

  #define KODE_Assert(x)      { if (!(x)) { KODE_Print("KODE_Assert : %s\n",#x); abort(); } }
  #define KODE_StaticAssert   static_assert

#else // KODE_DEBUG_ASSERT

  #define KODE_Assert(x)          {}
  #define KODE_StaticAssert(x,y)  {}


#endif // KODE_DEBUG_ASSERT

//----------------------------------------------------------------------
#endif
