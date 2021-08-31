#ifndef kode_xcb_utils_included
#define kode_xcb_utils_included
//----------------------------------------------------------------------

#include "kode.h"
#include "gui/xcb/kode_xcb.h"

//----------------------------------------------------------------------

struct KODE_XcbPolyText8 {
  uint8_t*  data  = nullptr;
  size_t    used  = 0;
};

//----------

//xcb_atom_t        kode_xcb_get_intern_atom(xcb_connection_t *conn, const char *name);
//xcb_cursor_t      kode_xcb_create_font_cursor(xcb_connection_t *conn, uint16_t glyph);
//xcb_visualtype_t* kode_xcb_find_visual(xcb_connection_t* c, xcb_visualid_t visual);
//void              kode_xcb_add_string_text8(KODE_XcbPolyText8* pt, char const* s);
//void              kode_xcb_add_delta_text8(KODE_XcbPolyText8* pt, uint8_t delta);
//void              kode_xcb_add_font_text8(KODE_XcbPolyText8* pt, xcb_font_t font);

//void* xcb_event_thread_proc(void* AWindow);
//void* xcb_timer_thread_proc(void* AWindow);

//----------------------------------------------------------------------

xcb_atom_t kode_xcb_get_intern_atom(xcb_connection_t *conn, const char *name) {
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn ,0, strlen(name), name);
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(conn, cookie, NULL);
  //return !reply ? XCB_ATOM_STRING : reply->atom;
  return reply->atom;
}

//----------

// http://tronche.com/gui/x/xlib/appendix/b/

xcb_cursor_t kode_xcb_create_font_cursor(xcb_connection_t *conn, uint16_t glyph) {
  static xcb_font_t cursor_font;
  cursor_font = xcb_generate_id (conn);
  xcb_open_font(conn, cursor_font, strlen("cursor"), "cursor");
  xcb_cursor_t cursor = xcb_generate_id (conn);
  xcb_create_glyph_cursor(
    conn,
    cursor,         // cid
    cursor_font,    // source_font
    cursor_font,    // mask_font
    glyph,          // source_char
    glyph + 1,      // mask_char
    //0x3232,         // fore_red (uint16_t)
    //0x3232,         // fore_green (uint16_t)
    //0x3232,         // fore_blue (uint16_t)
    //0xeeee,         // back_red (uint16_t)
    //0xeeee,         // back_green (uint16_t)
    //0xeeec          // back_blue (uint16_t)
    0xffff,0xffff,0xffff,
    0x0000,0x0000,0x0000
  );
  xcb_close_font(conn, cursor_font);
  xcb_flush(conn);
  return cursor;
}

//----------

xcb_visualtype_t* kode_xcb_find_visual(xcb_connection_t* c, xcb_visualid_t visual) {
  xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(c));
  for (; screen_iter.rem; xcb_screen_next(&screen_iter)) {
    xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
    for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
      xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
      for (; visual_iter.rem; xcb_visualtype_next(&visual_iter)) {
        if (visual == visual_iter.data->visual_id) return visual_iter.data;
      }
    }
  }
  return nullptr;
}

//----------

/*
  https://lists.freedesktop.org/archives/xcb/2008-October/003990.html

  I noticed that there is no code for use with xcb_poly_text_{8,16}. I've
  hacked together a few functions that generate parameters for use with
  xcb_poly_text_8. Using a bit of cut'n'paste would create similar code
  for xcb_poly_text_16. What I'm still at loss is how I would go about
  handling malloc/realloc errors and what I also don't know is the encoding
  of the strings, my knowledge of X is simply too limited for that.
*/

//----------


//----------

void kode_xcb_add_string_text8(KODE_XcbPolyText8* pt, char const* s) {
  size_t len = strlen(s);
  // Each element can only contain up to 254 characters, so we have to
  // split the string.
  size_t chunks = (len + 253) / 254;
  // Each element starts with one byte length and one byte 'delta', followed
  // by the character payload, determine the according number of needed bytes.
  //size_t const required = chunks * 2 + len;
  // allocate storage
  //uint8_t* buffer = (uint8_t*)realloc(pt->data, pt->used + required);
  //if(!buffer) return;
  //pt->data = buffer;
  while(chunks) {
    size_t n = (len < 255) ? len : 254;
    pt->data[pt->used] = n;
    pt->data[pt->used + 1] = 0;
    memcpy( pt->data+2, s, n);
    pt->used += n+2;
    s += n;
    len -= n;
    --chunks;
  }
}

//----------

void kode_xcb_add_delta_text8(KODE_XcbPolyText8* pt, uint8_t delta) {
  uint8_t* buffer;
  buffer = (uint8_t*)realloc( pt->data, pt->used+2);
  if(!buffer) return;
  pt->data = buffer;
  pt->data[pt->used] = 0;
  pt->data[pt->used + 1] = delta;
  pt->used += 2;
}

//----------

void kode_xcb_add_font_text8(KODE_XcbPolyText8* pt, xcb_font_t font) {
  uint8_t* buffer;
  buffer = (uint8_t*)realloc( pt->data, pt->used+5);
  if(!buffer) return;
  pt->data = buffer;
  // set length field to magic value 255 to signal a font is following
  pt->data[pt->used] = 255;
  // write font XID with MSB first
  pt->data[pt->used] = font>>24;
  pt->data[pt->used] = font>>16;
  pt->data[pt->used] = font>>8;
  pt->data[pt->used] = font;
}

//----------------------------------------------------------------------

// cairo-xcb-connection-core.c

void kode_xcb_put_image(xcb_connection_t* connection, xcb_drawable_t dst, xcb_gcontext_t gc,
                        uint16_t width, uint16_t height, int16_t dst_x, int16_t dst_y,
                        uint8_t depth, uint32_t stride, void *data) {
  const uint32_t req_size = 18;
  uint32_t length = height * stride;
  uint32_t len = (req_size + length) >> 2;

  uint32_t maximum_request_length = xcb_get_maximum_request_length(connection);

  if (len < maximum_request_length) {
    xcb_put_image(
      connection,
      XCB_IMAGE_FORMAT_Z_PIXMAP,
      dst,
      gc,
      width,
      height,
      dst_x,
      dst_y,
      0,
      depth,
      length,
      (uint8_t*)data
    );
  } else {
    int rows = (maximum_request_length - req_size - 4) / stride;
    if (rows > 0) {
      do {
        if (rows > height) rows = height;
        length = rows * stride;
        xcb_put_image(
          connection,
          XCB_IMAGE_FORMAT_Z_PIXMAP,
          dst,
          gc,
          width,
          rows,
          dst_x,
          dst_y,
          0,
          depth,
          length,
          (uint8_t*)data
        );
        height -= rows;
        dst_y += rows;
        data = (char *) data + length;
      } while (height);
    } else {
      //ASSERT_NOT_REACHED;
    }
  }
}

//----------------------------------------------------------------------


//----------------------------------------------------------------------
#endif
