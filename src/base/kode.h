#ifndef kode_included
#define kode_included
//----------------------------------------------------------------------

#include "kode_config.h"

#include "base/kode_defines.h"
#include "base/kode_attributes.h"
#include "base/kode_types.h"
#include "base/kode_const.h"
#include "base/kode_enums.h"

#include "base/kode_stdlib.h"

#include "base/debug/kode_debug.h"

//----------------------------------------------------------------------
#endif

/*

compiler flags:

global
  -Wall
  -std=c++11
  -m64
  -Wl,--as-needed
  -I../src
  -I../src/extern/vst3

sanitizer
  -fsanitize=address

exe debug:
  -g
  -O0
  -Wl,-Bsymbolic
  -rdynamic
  -fno-pie

exe release:
  -O2
  -fno-pie

plugin debug
  -g
  -O0
  -fPIC
  -Wl,-Bsymbolic
  -rdynamic
  -shared ?

plugin release
  -fPIC
  -shared ?
  -fvisibility=hidden
  -ffunction-sections
  -fdata-sections
  -fexpensive-optimizations
  -msse3
  -mfpmath=sse
  -ffast-math

*/
