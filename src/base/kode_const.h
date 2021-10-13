#ifndef kode_const_included
#define kode_const_included
//----------------------------------------------------------------------

//#include "base/kode_types.h"
//#include <inttypes.h> // uint8_t

//----------------------------------------------------------------------

//const uint8_t KODE_NULL_ID[16]    = {0};
const char    KODE_HEX_TABLE[]    = "0123456789ABCDEF";
const char    KODE_CSYMBOLS[]     = "________________________________________________0123456789_______ABCDEFGHIJKLMNOPQRSTUVWXYZ______abcdefghijklmnopqrstuvwxyz_____";

// 4         4    5         5
// 123456789abcdef0123456789a
// abcdefghijklmnopqrstuvwxyz
// 5f = _

#define KODE_MAGIC        0x4b5f0000    // 'K_'   // + 0x0000
#define KODE_MAGIC_USER   0x6b5f0000    // 'k_'   // + 0x0000
#define KODE_MAGIC_K_PL   0x4b5f504c    // K_PL   // plugin
#define KODE_MAGIC_K_ED   0x4b5f4544    // K_ED   // editor

#ifdef KODE_CPP11
  #define KODE_NULL nullptr
#else
  #define KODE_NULL 0
#endif

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

#define KODE_ALIGNMENT_SIMD           16
#define KODE_ALIGNMENT_CACHE          64

#define KODE_AUDIO_MAX_EVENTS         256
#define KODE_AUDIO_MAX_MIDI_EVENTS    256
#define KODE_AUDIO_MAX_NODES          256
#define KODE_AUDIO_TICKSIZE           16
#define KODE_AUDIO_NUM_TICK_BUFFERS   16

#define KODE_BITMAP_RASTER_MAX_HEIGHT 2048

#define KODE_GUI_DBLCLICK_MS          200

#define KODE_MAX_PATH_LENGTH          512
#define KODE_MAX_STRING_LENGTH        256

//----------------------------------------------------------------------
// midi
//----------------------------------------------------------------------

#define KODE_MIDI_NOTE_OFF            0x80
#define KODE_MIDI_NOTE_ON             0x90
#define KODE_MIDI_POLY_AFTERTOUCH     0xA0
#define KODE_MIDI_CONTROL_CHANGE      0xB0
#define KODE_MIDI_PROGRAM_CHANGE      0xC0
#define KODE_MIDI_CHANNEL_AFTERTOUCH  0xD0
#define KODE_MIDI_PITCHBEND           0xE0
#define KODE_MIDI_SYS                 0xF0

//----------------------------------------------------------------------
// math
//----------------------------------------------------------------------

#define KODE_PI             3.1415926535
#define KODE_PI2            (KODE_PI * 2.0)
#define KODE_PI05           (KODE_PI * 0.5)
#define KODE_INVPI          (1.0 / KODE_PI)
#define KODE_INVPI2         (1.0 / KODE_PI2)
#define KODE_INVPI05        (1.0 / KODE_PI05)

#define KODE_TINY           (0.001)
#define KODE_VERY_TINY      (0.00001)

#define KODE_INV360F        (1.0 / 360.0)
#define KODE_INV12F         (1.0 / 12.0)
#define KODE_INV16F         (1.0 / 16.0)
#define KODE_INV32F         (1.0 / 32.0)
#define KODE_INV127F        (1.0 / 127.0)
#define KODE_INV255F        (1.0 / 255.0)
#define KODE_INV256F        (1.0 / 256.0)
#define KODE_INV8192F       (1.0 / 8192.0)

#define KODE_SQRT2          sqrtf(2.0f)

//----------------------------------------------------------------------
// type sizes
//----------------------------------------------------------------------

#define KODE_CHAR_BIT       8
#define KODE_CHAR_MAX       0x7f                /* 127 */
#define KODE_SHORT_MAX      0x7fff              /* 32767 */
#define KODE_INT32_MAX      0x7fffffff          /* 2147483647 */
#define KODE_UINT32_MAX     0xffffffff          /* 8589934591U */
#define KODE_INT64_MAX      0x7fffffffffffffff  /* 9223372036854775807LL */
#define KODE_UINT64_MAX     0x7fffffffffffffff  /* 18446744073709551615ULL */
#define KODE_FLOAT_MAX      3.40282347e+38F
#define KODE_FLOAT_MIN      1.17549435E-38F
#define KODE_FLOAT_EPSILON  1.19209290E-7F
//#define KODE_FLOAT_DENORM   1.40129846E-45F
#define KODE_DOUBLE_MAX     1.7976931348623157e+308
#define KODE_DOUBLE_MIN     2.2250738585072014E-308
#define KODE_DOUBLE_EPSILON 2.2204460492503131E-16
#define KODE_DOUBLE_DENORM  4.9406564584124654E-324

#ifdef KODE_32BIT
  #define KODE_INT_MAX      KODE_INT32_MAX   // 2147483647L
  #define KODE_UINT_MAX     KODE_UINT32_MAX  // 8589934591UL
#endif

#ifdef KODE_64BIT
  #define KODE_INT_MAX      KODE_INT64_MAX   // 9223372036854775807L
  #define KODE_UINT_MAX     KODE_UINT64_MAX  // 18446744073709551615UL
#endif

#define KODE_RANDMAX        RAND_MAX  // 2147483647
#define KODE_INVRANDMAX     (1.0 / (float)KODE_RANDMAX)

//----------------------------------------------------------------------
#endif
