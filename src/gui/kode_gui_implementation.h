#ifndef kode_gui_implementation_included
#define kode_gui_implementation_included
//----------------------------------------------------------------------

#include "kode.h"

//----------------------------------------------------------------------

#ifdef KODE_CAIRO
  #include "gui/cairo/kode_cairo_painter.h"
#endif

#ifdef KODE_XCB
  #include "gui/xcb/kode_xcb_image.h"
  #include "gui/xcb/kode_xcb_surface.h"
  #include "gui/xcb/kode_xcb_window.h"
  #include "gui/xcb/kode_xcb_painter.h"
#endif

//----------------------------------------------------------------------

#ifdef KODE_GUI_XCB
  typedef KODE_XcbImage     KODE_ImplementedImage;
  typedef KODE_XcbSurface   KODE_ImplementedSurface;
  typedef KODE_XcbWindow    KODE_ImplementedWindow;
  typedef KODE_XcbPainter   KODE_ImplementedPainter;
#endif

#ifdef KODE_GUI_CAIRO
  typedef KODE_XcbImage     KODE_ImplementedImage;
  typedef KODE_XcbSurface   KODE_ImplementedSurface;
  typedef KODE_XcbWindow    KODE_ImplementedWindow;
  typedef KODE_CairoPainter KODE_ImplementedPainter;
#endif

//----------------------------------------------------------------------
#endif
