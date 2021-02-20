#ifndef kode_xcb_painter_included
#define kode_xcb_painter_included
//----------------------------------------------------------------------

#include "gui/kode_gui_base.h"
#include "gui/xcb/kode_xcb_utils.h"

#ifdef KODE_USE_CAIRO
  #include "gui/cairo/kode_cairo.h"
#endif

class KODE_XcbPainter
: public KODE_IPainter {

//------------------------------
private:
//------------------------------

  KODE_Drawable*    MTarget       = KODE_NULL;
  xcb_connection_t* MConnection   = KODE_NULL;
  xcb_drawable_t    MDrawable     = XCB_NONE;
  uint32_t          MWidth        = 0;
  uint32_t          MHeight       = 0;

  xcb_gcontext_t    MGC           = XCB_NONE;
  int32_t           MFontAscent   = 0;
  int32_t           MFontDescent  = 0;
  int32_t           MFontWidth    = 0;
  int32_t           MFontHeight   = 0;
  int32_t           MFontOrigin   = 0;
  int32_t           MFontLeft     = 0;
  int32_t           MFontRight    = 0;
  float             MXpos         = 0.0f;
  float             MYpos         = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_XcbPainter(KODE_Drawable* ATarget)
  : KODE_IPainter(ATarget) {
    if (ATarget->isDrawable()) {
      MTarget     = ATarget;
      MConnection = ATarget->getXcbConnection();
      MDrawable   = ATarget->getXcbDrawable();
      MWidth      = ATarget->getWidth();
      MHeight     = ATarget->getHeight();
      MGC         = xcb_generate_id(MConnection);
      uint32_t mask =
        //XCB_GC_FUNCTION
        //XCB_GC_PLANE_MASK
        //XCB_GC_FOREGROUND
        //XCB_GC_BACKGROUND
        //XCB_GC_LINE_WIDTH
        //XCB_GC_LINE_STYLE
        //XCB_GC_CAP_STYLE
        //XCB_GC_JOIN_STYLE
        //XCB_GC_FILL_STYLE
        //XCB_GC_FILL_RULE
        //XCB_GC_TILE
        //XCB_GC_STIPPLE
        //XCB_GC_TILE_STIPPLE_ORIGIN_X
        //XCB_GC_TILE_STIPPLE_ORIGIN_Y
        //XCB_GC_FONT |
        //XCB_GC_SUBWINDOW_MODE
        XCB_GC_GRAPHICS_EXPOSURES;
        //XCB_GC_CLIP_ORIGIN_X
        //XCB_GC_CLIP_ORIGIN_Y
        //XCB_GC_CLIP_MASK
        //XCB_GC_DASH_OFFSET
        //XCB_GC_DASH_LIST
        //XCB_GC_ARC_MODE
      uint32_t values[] = {
        0
      };
      xcb_create_gc(MConnection,MGC,MDrawable,mask,values);
      xcb_flush(MConnection);
    }
  }

  //----------

  virtual ~KODE_XcbPainter() {
    //xcb_flush(MConnection);
    xcb_free_gc(MConnection,MGC);
  }

//------------------------------
public:
//------------------------------

  #ifdef KODE_USE_CAIRO
  cairo_surface_t* void createCairoSurface() {
    cairo_surface_t* surface = MCairoSurface = cairo_xcb_surface_create(
      MConnection,
      MDrawable,
      kode_xcb_find_visual(MConnection,MVisual),
      MWidth,
      MHeight
    );
    return surface;
  }
  #endif


//------------------------------
private:
//------------------------------

  void set_color(KODE_Color AColor) {
    uint8_t r = AColor.r * 255.0f;
    uint8_t g = AColor.g * 255.0f;
    uint8_t b = AColor.b * 255.0f;
    uint8_t a = AColor.a * 255.0f;
    uint32_t color = (a << 24) + (r << 16) + (g << 8) + b;
    uint32_t mask = XCB_GC_FOREGROUND;
    uint32_t values[1];
    values[0] = color;
    xcb_change_gc(MConnection, MGC, mask, values);
  }

  //----------

  void measure_string(const char *string) {
    xcb_char2b_t xcb_str[256];
    for (uint32_t i = 0; i < KODE_Strlen(string); i++) {
      xcb_str[i].byte1 = 0;
      xcb_str[i].byte2 = string[i];
    }
    xcb_query_text_extents_cookie_t cookie = xcb_query_text_extents(MConnection, MGC, KODE_Strlen(string), xcb_str);
    xcb_query_text_extents_reply_t* reply = xcb_query_text_extents_reply(MConnection, cookie, NULL);
    MFontAscent   = reply->font_ascent;
    MFontDescent  = reply->font_descent;
    MFontWidth    = reply->overall_width;
    MFontHeight   = reply->font_ascent + reply->font_descent;
    MFontOrigin   = reply->font_ascent;
    MFontLeft     = reply->overall_left;
    MFontRight    = reply->overall_right;
    //MFontOverallAscent = reply->overall_ascent;
    //MFontOverallDescent = reply->overall_descent;
    //KODE_Free(xcb_str);
    KODE_Free(reply);
  }

//------------------------------
public:
//------------------------------

  void resize(uint32_t AWidth, uint32_t AHeight) final {
    MWidth = AWidth;
    MHeight = AHeight;
    // cairo: cairo_xcb_surface_set_size:

  }

  //----------

  void setClip(KODE_FRect ARect) final {
    //resetClip();
    xcb_rectangle_t rectangles[] = {{
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w, // +1
      (uint16_t)ARect.h  // +1
    }};
    xcb_set_clip_rectangles(
      MConnection,
      XCB_CLIP_ORDERING_UNSORTED, // ordering,
      MGC,
      0, // clip_x_origin
      0, // clip_y_origin
      1, // rectangles_len
      rectangles
    );
    xcb_flush(MConnection);
  }

  //----------

  void resetClip() final {
    uint32_t mask = XCB_GC_CLIP_MASK;
    uint32_t values[1];
    values[0] = XCB_NONE;
    xcb_change_gc(MConnection, MGC, mask, values);
    xcb_flush(MConnection);
  }

//------------------------------
public:
//------------------------------

  float getTextWidth(const char* AText) final {
    measure_string(AText);
    return MFontWidth;
  }

  //----------

  float getTextHeight(const char* AText) final {
    measure_string(AText);
    return MFontHeight;
  }

//------------------------------
public:
//------------------------------

  void drawLine(float AXpos1, float AYpos1, float AXpos2, float AYpos2, KODE_Color AColor) final {
    set_color(AColor);
    xcb_point_t polyline[] =  {
      (int16_t)AXpos1, (int16_t)AYpos1,
      (int16_t)AXpos2, (int16_t)AYpos2,
    };
    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,2,polyline);
  }

  //----------

  void drawRect(KODE_FRect ARect, KODE_Color AColor) final {
    set_color(AColor);
    xcb_rectangle_t rectangles[] = {{
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w,
      (uint16_t)ARect.h
    }};
    xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }

  //----------

  void fillRect(KODE_FRect ARect, KODE_Color AColor) final {
    set_color(AColor);
    xcb_rectangle_t rectangles[] = {{
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w,
      (uint16_t)ARect.h
    }};
    xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
  }

  //----------

  /*
    angle 1 = start angle, relative to 3 o'clock
    angle 2 = 'distance' 0..1, counter-clockwise
  */

  void drawPie(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor) final {
    set_color(AColor);
    // start angle = 12 o'clock
    float a1 = -AAngle1 + 0.25f;
    // positive = clockwise, negative = counter-clockwise
    float a2 = -AAngle2;
    //XDrawArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(a1*(360.0f*64.0f)),(a2*(360.0f*64.0f)));
    xcb_arc_t arcs[] = {
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w,  // +1
      (uint16_t)ARect.h,  // +1
      (int16_t)(a1 * 360.0f * 64.0f),
      (int16_t)(a2 * 360.0f * 64.0f)
    };
    xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  //----------

  // angle 1 = start angle, relative to 3 o'clock
  // angle 2 = 'distance' 0..1, counter-clockwise

  void fillPie(KODE_FRect ARect, float AAngle1, float AAngle2, KODE_Color AColor) final {
    set_color(AColor);
    //if (abs(AAngle2) >= 0.01) EPSILON
    // start angle = 12 o'clock
    float a1 = -AAngle1 + 0.25;
    // positive = clockwise, negative = counter-clockwise
    float a2 = -AAngle2;
    xcb_arc_t arcs[] = {
      (int16_t)ARect.x,
      (int16_t)ARect.y,
      (uint16_t)ARect.w, // +1
      (uint16_t)ARect.h, // +1
      (int16_t)(a1 * 360 * 64),
      (int16_t)(a2 * 360 * 64)
    };
    xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
  }

  //----------

  void drawText(float AXpos, float AYpos, const char* AText, KODE_Color AColor) final {
    set_color(AColor);
    uint8_t buffer[512];
    KODE_XcbPolyText8 pt;
    pt.data = buffer;
    pt.used = 0;
    kode_xcb_add_string_text8(&pt,AText);
    xcb_poly_text_8(MConnection,MDrawable,MGC,AXpos,AYpos,pt.used,pt.data);
  }

  //----------

  void drawText(KODE_FRect ARect, const char* AText, uint32_t AAlignment, KODE_Color AColor) final {
    measure_string(AText);
    float x,y,w;
    if (AAlignment & KODE_TEXT_ALIGN_TOP) y = ARect.y    + MFontAscent;
    else if (AAlignment & KODE_TEXT_ALIGN_BOTTOM) y = ARect.y2() - MFontDescent;
    else y = ARect.y + (MFontAscent * 0.5f) + (ARect.h * 0.5f);
    w = MFontWidth;
    if (AAlignment & KODE_TEXT_ALIGN_LEFT) x = ARect.x;
    else if (AAlignment & KODE_TEXT_ALIGN_RIGHT) x = ARect.x2() - w;
    else x = ARect.x + (ARect.w * 0.5f) - (w * 0.5f);
    drawText(x,y,AText,AColor);
  }

  //----------

  void uploadBitmap(float AXpos, float AYpos, KODE_Bitmap* ABitmap) final {
    uint32_t width      = ABitmap->getWidth();
    uint32_t height     = ABitmap->getHeight();
    uint32_t buffersize = ABitmap->getBufferSize();
    uint32_t* buffer    = ABitmap->getBuffer();
    xcb_image_t* image = xcb_image_create(
      width,                          // width      width in pixels.
      height,                         // height     height in pixels.
      XCB_IMAGE_FORMAT_Z_PIXMAP,      // format
      32,                             // xpad       scanline pad (8,16,32)
      24,                             // depth      (1,4,8,16,24 zpixmap),    (1 xybitmap), (anything xypixmap)
      32,                             // bpp        (1,4,8,16,24,32 zpixmap,  (1 xybitmap), (anything xypixmap)
      32,                             // unit       unit of image representation, in bits (8,16,32)
      XCB_IMAGE_ORDER_LSB_FIRST,      // byte_order
      XCB_IMAGE_ORDER_LSB_FIRST,      // bit_order
      buffer,                         // base       The base address of malloced image data
      buffersize,                     // bytes      The size in bytes of the storage pointed to by base.
                                      //            If base == 0 and bytes == ~0 and data == 0, no storage will be auto-allocated.
      (uint8_t*)buffer                // data       The image data. If data is null and bytes != ~0, then an attempt will be made
                                      //            to fill in data; from base if it is non-null (and bytes is large enough), else
                                      //            by mallocing sufficient storage and filling in base.
    );
    //xcb_flush(MTargetConnection);
    xcb_image_put(
      MConnection,            // xcb_connection_t*  conn,
      MDrawable,              // xcb_drawable_t     draw,
      MGC,                    // xcb_gcontext_t     gc,
      image,                  // xcb_image_t*       image,
      AXpos,                  // int16_t            x,
      AYpos,                  // int16_t            y,
      0                       // uint8_t            left_pad
    );
    //xcb_flush(MConnection);
    image->base = KODE_NULL;
    xcb_image_destroy(image);
    xcb_flush(MConnection);
  }

  //----------

  void drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource) final {
    if (ASource->isImage()) {
      xcb_image_put(
        MConnection,            // xcb_connection_t*  conn,
        MDrawable,              // xcb_drawable_t     draw,
        MGC,                    // xcb_gcontext_t     gc,
        ASource->getXcbImage(), // xcb_image_t*       image,
        AXpos,                  // int16_t            x,
        AYpos,                  // int16_t            y,
        0                       // uint8_t            left_pad
      );
      xcb_flush(MConnection);
    }
    else if (ASource->isSurface()) {
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_flush(MCairoSurface);
      //#endif
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->getXcbDrawable(),  // The Drawable we want to paste
        MDrawable,                  // The Drawable on which we copy the previous Drawable
        MGC,                        // A Graphic Context
        0,                          // Top left x coordinate of the region we want to copy
        0,                          // Top left y coordinate of the region we want to copy
        AXpos,                      // Top left x coordinate of the region where we want to copy
        AYpos,                      // Top left y coordinate of the region where we want to copy
        ASource->getWidth(),        // Width                 of the region we want to copy
        ASource->getHeight()        // Height of the region we want to copy
      );
      xcb_flush(MConnection);
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    //else {
    //  KODE_Trace("unknown ADrawable for blit()\n");
    //}
  }

  //----------

  void drawBitmap(float AXpos, float AYpos, KODE_Drawable* ASource, KODE_FRect ASrc) final {
    if (ASource->isImage()) {
      KODE_Bitmap* bitmap = ASource->getBitmap();
      kode_xcb_put_image(
        MConnection,
        MDrawable,
        MGC,
        ASrc.w,
        ASrc.h,
        AXpos,
        AYpos,
        MTarget->getDepth(),  //ASource->getDepth(),
        bitmap->getStride(),
        bitmap->getPixelPtr(ASrc.x,ASrc.y)  //getBuffer()
      );
      xcb_flush(MConnection);
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    else if (ASource->isSurface()) {
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_flush(MCairoSurface);
      //#endif
      xcb_copy_area(
        MConnection,                // Pointer to the xcb_connection_t structure
        ASource->getXcbDrawable(),  // The Drawable we want to paste
        MDrawable,                    // The Drawable on which we copy the previous Drawable
        MGC,                  // A Graphic Context
        ASrc.x,                      // Top left x coordinate of the region we want to copy
        ASrc.y,                      // Top left y coordinate of the region we want to copy
        AXpos,                      // Top left x coordinate of the region where we want to copy
        AYpos,                      // Top left y coordinate of the region where we want to copy
        ASrc.w,                      // Width                 of the region we want to copy
        ASrc.h                       // Height of the region we want to copy
      );
      xcb_flush(MConnection);
      //#ifdef KODE_USE_CAIRO
      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
      //#endif
    }
    //else {
    //  KODE_Trace("unknown ADrawable for blit()\n");
    //}
  }

  //----------

  void drawBitmap(KODE_FRect ADst, KODE_Drawable* ASource, KODE_FRect ASrc) final {
  }

  //----------

};

//----------------------------------------------------------------------
#endif
