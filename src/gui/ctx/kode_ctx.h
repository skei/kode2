#ifndef kode_ctx_included
#define kode_ctx_included
//----------------------------------------------------------------------

#define STB_TRUETYPE_IMPLEMENTATION
#include "extern/stb/stb_truetype.h"

//#define CTX_LIMIT_FORMATS 1
//#define CTX_ENABLE_BGRA8 1

//#define CTX_SHAPE_CACHE         1
//#define CTX_SHAPE_CACHE_DIM     (16*16)
//#define CTX_SHAPE_CACHE_ENTRIES 160

#define CTX_IMPLEMENTATION

//----------------------------------------------------------------------

#ifdef CAIRO_H

  #undef CAIRO_H
    #include "extern/ctx/ctx-font-regular.h"
    #include "extern/ctx/ctx.h"
  #define CAIRO_H

#else // !CAIRO_H

  #include "extern/ctx/ctx-font-regular.h"
  #include "extern/ctx/ctx.h"

#endif // CAIRO_H

//----------------------------------------------------------------------
#endif

