#ifndef kode_cairo_utils_included
#define kode_cairo_utils_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/cairo/kode_cairo.h"

//----------------------------------------------------------------------

void check_cairo_errors(cairo_t* cairo) {
  cairo_status_t status = cairo_status(cairo);
  if (status != CAIRO_STATUS_SUCCESS) {
    KODE_Print("cairo status: %s\n",cairo_status_to_string(status));
  }
}

//----------

void check_cairo_surface_errors(cairo_surface_t* surface) {
  cairo_status_t status = cairo_surface_status(surface);
  if (status != CAIRO_STATUS_SUCCESS) {
    KODE_Print("cairo status: %s\n",cairo_status_to_string(status));
  }
}

//----------------------------------------------------------------------
#endif
