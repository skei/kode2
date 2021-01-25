#ifndef kode_xcb_painter_included
#define kode_xcb_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_painter.h"
#include "gui/xcb/kode_xcb.h"
#include "gui/xcb/kode_xcb_utils.h"

//----------------------------------------------------------------------



//----------------------------------------------------------------------

class KODE_XcbPainter
: public KODE_BasePainter {

//------------------------------
private:
//------------------------------

  xcb_gcontext_t    MGC           = XCB_NONE;
  xcb_connection_t* MConnection   = nullptr;
  xcb_drawable_t    MDrawable     = XCB_NONE;
  KODE_Drawable*    MTarget       = nullptr;

  int32_t           MFontAscent   = 0;
  int32_t           MFontDescent  = 0;
  int32_t           MFontWidth    = 0;
  int32_t           MFontHeight   = 0;
  int32_t           MFontOrigin   = 0;
  int32_t           MFontLeft     = 0;
  int32_t           MFontRight    = 0;

  //uint32_t          MDrawColor;//= KODE_LightGrey;
  //uint32_t          MFillColor;//= KODE_DarkGrey;
  //uint32_t          MTextColor;//= KODE_Black;

//  KODE_Rect         MClipRect     = KODE_Rect(0);
//  KODE_RectStack    MClipStack;

  float MXpos = 0.0f;
  float MYpos = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_XcbPainter(KODE_Drawable* ATarget)
  : KODE_BasePainter(ATarget) {

    if (ATarget->isDrawable()) {
      MTarget     = ATarget;
      MConnection = ATarget->getXcbConnection();
      MDrawable   = ATarget->getXcbDrawable();
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
//      MClipRect = KODE_Rect( 0,0,ATarget->getWidth(),ATarget->getHeight() );
    }

  }

  //----------

  // added flush, do we want this?

  virtual ~KODE_XcbPainter() {
    xcb_flush(MConnection); // ???
    xcb_free_gc(MConnection,MGC);
  }

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

  void  drawRect(KODE_FRect ARect, KODE_Color AColor) {
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

  void  fillRect(KODE_FRect ARect, KODE_Color AColor) {
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

  void drawText(float x, float y, const char* txt, KODE_Color color) final {
    set_color(color);
    uint8_t buffer[512];
    KODE_XcbPolyText8 pt;
    pt.data = buffer;
    pt.used = 0;
    kode_xcb_add_string_text8(&pt,txt);
    xcb_poly_text_8(MConnection,MDrawable,MGC,x,y,pt.used,pt.data);
  }

  //----------

  void  drawText(KODE_FRect ARect,const char* AText, KODE_Color AColor, uint32_t AAlignment) {
    measure_string(AText);
    float x,y,w;
    if (AAlignment & KODE_TEXT_ALIGN_TOP) y = ARect.y + MFontAscent;
    else if (AAlignment & KODE_TEXT_ALIGN_BOTTOM) y = ARect.y2() - MFontDescent;
    else y = ARect.y + (MFontAscent * 0.5f) + (ARect.h * 0.5f );
    w = MFontWidth;
    if (AAlignment & KODE_TEXT_ALIGN_LEFT) x = ARect.x;
    else if (AAlignment & KODE_TEXT_ALIGN_RIGHT) x = ARect.x2() - w;
    else x = ARect.x + (ARect.w * 0.5f) - ( w * 0.5f );
    drawText(x,y,AText,AColor);
  }

};

//----------------------------------------------------------------------
#endif

//------------------------------
//------------------------------

//  void clear() final {
//    // set color
//    uint32_t mask = XCB_GC_FOREGROUND;
//    uint32_t values[1];
//    values[0] = 0xff000000;
//    xcb_change_gc(MConnection,MGC,mask,values);
//    // fill rectangle
//    xcb_rectangle_t rectangles[] = {{
//      (int16_t)0,
//      (int16_t)0,
//      (uint16_t)MTarget->getWidth(),
//      (uint16_t)MTarget->getHeight(),
//    }};
//    xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
//    xcb_flush(MConnection);
//  }
//
//  //----------
//
//  void empty() final {
//  }
//
//  //----------
//
//  void dirty() final {
//  }
//
//  //----------
//
//  void dirty(float x1, float y1, float x2, float y2) final {
//  }
//
//  //----------
//
//  void flush() final {
//    xcb_flush(MConnection);
//  }
//
//  //----------
//
//  void finish() final {
//  }
//
//  //----------
//
//  void save() final {
//  }
//
//  //----------
//
//  void restore() final {
//  }
//
//  //----------
//
//  void rotate(float a) final {
//  }
//
//  //----------
//
//  void scale(float x, float y) final {
//  }
//
//  //----------
//
//  void translate(float x, float y) final {
//  }
//
//  //----------
//
//  void setFillRule(uint32_t rule) final {
//  }
//
//  //----------
//
//  void setGlobalAlpha(float alpha) final {
//  }
//
//  //----------
//
//  void setColor(uint32_t AColor) final {
//    uint32_t mask = XCB_GC_FOREGROUND;
//    uint32_t values[1];
//    values[0] = AColor;
//    xcb_change_gc(MConnection, MGC, mask, values);
//  }
//
//  //----------
//
//  void setLineWidth(float w) final {
//    uint32_t mask = XCB_GC_LINE_WIDTH;
//    uint32_t values[1];
//    values[0] = w;
//    xcb_change_gc(MConnection, MGC, mask, values);
//  }
//
//  //----------
//
//  void setLineDash(float* dashes, uint32_t numdashes, float offset) final {
//    //uint32_t mask = XCB_GC_DASH_LIST | XCB_GC_DASH_OFFSET;
//    //uint32_t values[2];
//    //values[0] =
//    //values[0] =
//    //xcb_change_gc(MConnection, MGC, mask, values
//  }
//
//  //----------
//
//  void setLineCap(uint32_t cap) final {
//    uint32_t mask = XCB_GC_CAP_STYLE;
//    uint32_t values[1];
//    values[0] = cap;
//    xcb_change_gc(MConnection, MGC, mask, values);
//  }
//
//  //----------
//
//  void setLineJoin(uint32_t join) final {
//    uint32_t mask = XCB_GC_JOIN_STYLE;
//    uint32_t values[1];
//    values[0] = join;
//    xcb_change_gc(MConnection, MGC, mask, values);
//  }
//
//  //----------
//
//  void setFont(const char* name) final {
//    xcb_font_t font = xcb_generate_id(MConnection);
//    xcb_open_font(
//      MConnection,
//      font,
//      KODE_Strlen(name),
//      name
//    );
//    uint32_t mask = XCB_GC_FONT;
//    uint32_t values[1];
//    values[0] = font;
//    xcb_change_gc(MConnection, MGC, mask, values);
//    xcb_close_font(MConnection,font);
//  }
//
//  //----------
//
//  void setFontSize(float size) final {
//  }
//
//  //----------
//
//  void setFontSlant(uint32_t slant) final {
//  }
//
//  //----------
//
//  void setFontWeight(uint32_t weight) final {
//  }
//
//  //----------
//
//  void loadFont(const char* name, const char* filepath) final {
//  }
//
//  //----------
//
//  void gradientAddStop(float pos, uint32_t color) final {
//  }
//
//  //----------
//
//  void gradientClearStops() final {
//  }
//
//  //----------
//
//  float getCurrentXPos() final {
//    return 0.0f;
//  }
//
//  //----------
//
//  float getCurrentYPos() final {
//    return 0.0f;
//  }
//
//  //----------
//
//  float getTextWidth(const char* str) final {
//    return 0.0f;
//    //_measureString(AText);
//    //return MFontWidth;
//  }
//
//  //----------
//
//  float getTextHeight(const char* str) final {
//    return 0.0f;
//    //_measureString(AText);
//    //return MFontHeight;
//  }
//
//  //----------
//
//  void newPath() final {
//  }
//
//  //----------
//
//  void closePath() final {
//  }
//
//  //----------
//
//  void fill() final {
//  }
//
//  //----------
//
//  void stroke() final {
//  }
//
//  //----------
//
//  void paint() final {
//  }
//
//  //----------
//
//  void clip() final {
//  }
//
//  //----------
//
//  void resetClip() final {
//    uint32_t mask = XCB_GC_CLIP_MASK;
//    uint32_t values[1];
//    values[0] = XCB_NONE;
//    xcb_change_gc(MConnection, MGC, mask, values);
//    xcb_flush(MConnection);
//  }
//
//  //----------
//
//  void linearGradient(float x0, float y0, float x1, float y1) final {
//  }
//
//  //----------
//
//  void radialGradient(float x0, float y0, float r0, float x1, float y1, float r1) final {
//  }
//
//  //----------
//
//  void moveTo(float x, float y) final {
//  }
//
//  //----------
//
//  void lineTo(float x, float y) final {
//  }
//
//  //----------
//
//  void curveTo(float cx0, float cy0, float cx1, float cy1, float x, float y) final {
//  }
//
//  //----------
//
//  void quadTo(float cx, float cy, float x, float y) final {
//  }
//
//  //----------
//
//  void arcTo(float x1, float y1, float x2, float y2, float radius) final {
//  }
//
//  //----------
//
//  void relMoveTo(float x, float y) final {
//  }
//
//  //----------
//
//  void relLineTo(float x, float y) final {
//  }
//
//  //----------
//
//  void relCurveTo(float cx0, float cy0, float cx1, float cy1, float x, float y) final {
//  }
//
//  //----------
//
//  void relQuadTo(float cx, float cy, float x, float y) final {
//  }
//
//  //----------
//
//  void relArcTo(float x1, float y1, float x2, float y2, float radius) final {
//  }
//
//  //----------
//
//  void setPixel(uint32_t x, uint32_t y, uint32_t color) final {
//  }
//
//  //----------
//
//  void rectangle(float x, float y, float w, float h) final {
//  }
//
//  //----------
//
//  void arc(float x, float y, float radius, float angle1, float angle2, int   direction) final {
//  }
//
//  //----------
//
//  void drawText(const char* txt) final {
//  }
//
//  //----------
//
//  void strokeText(const char* txt) final {
//  }
//
////------------------------------
//public:
////------------------------------
//
//  //void imagePath(const char *path, float x, float y) final {
//  //}
//
//  //----------
//
//  //void blit(void *data, int x, int y, int width, int height, int stride, uint32_t pixel_format) final {
//  //}
//
//  //----------
//
//  //int glyph(uint32_t unichar, int stroke) final final {
//  //}
//
////------------------------------
//public:
////------------------------------
//
//  void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource) final {
//    if (ASource->isImage()) {
//      xcb_image_put(
//        MConnection,            // xcb_connection_t*  conn,
//        MDrawable,              // xcb_drawable_t     draw,
//        MGC,                    // xcb_gcontext_t     gc,
//        ASource->getXcbImage(), // xcb_image_t*       image,
//        ADstX,                  // int16_t            x,
//        ADstY,                  // int16_t            y,
//        0                       // uint8_t            left_pad
//      );
//      xcb_flush(MConnection);
//    }
//    else if (ASource->isSurface()) {
//      //#ifdef KODE_CAIRO
//      //cairo_surface_flush(MCairoSurface);
//      //#endif
//      xcb_copy_area(
//        MConnection,                // Pointer to the xcb_connection_t structure
//        ASource->getXcbDrawable(),  // The Drawable we want to paste
//        MDrawable,                  // The Drawable on which we copy the previous Drawable
//        MGC,                        // A Graphic Context
//        0,                          // Top left x coordinate of the region we want to copy
//        0,                          // Top left y coordinate of the region we want to copy
//        ADstX,                      // Top left x coordinate of the region where we want to copy
//        ADstY,                      // Top left y coordinate of the region where we want to copy
//        ASource->getWidth(),        // Width                 of the region we want to copy
//        ASource->getHeight()        // Height of the region we want to copy
//      );
//      xcb_flush(MConnection);
//      //#ifdef KODE_CAIRO
//      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
//      //#endif
//    }
//    //else {
//    //  KODE_Trace("unknown ADrawable for blit()\n");
//    //}
//  }
//
//  //----------
//
//  void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) final {
//    if (ASource->isImage()) {
//
//      kode_xcb_put_image(
//        MConnection,
//        MDrawable,
//        MGC,
//        ASrcW,
//        ASrcH,
//        ADstX,
//        ADstY,
//        MTarget->getDepth(),  //ASource->getDepth(),
//        ASource->getStride(),
//        ASource->getBitmap()->getPixelPtr(ASrcX,ASrcY)//getBuffer()
//      );
//
//      xcb_flush(MConnection);
//      //#ifdef KODE_CAIRO
//      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
//      //#endif
//    }
//    else if (ASource->isSurface()) {
//      //#ifdef KODE_CAIRO
//      //cairo_surface_flush(MCairoSurface);
//      //#endif
//      xcb_copy_area(
//        MConnection,                // Pointer to the xcb_connection_t structure
//        ASource->getXcbDrawable(),  // The Drawable we want to paste
//        MDrawable,                    // The Drawable on which we copy the previous Drawable
//        MGC,                  // A Graphic Context
//        ASrcX,                      // Top left x coordinate of the region we want to copy
//        ASrcY,                      // Top left y coordinate of the region we want to copy
//        ADstX,                      // Top left x coordinate of the region where we want to copy
//        ADstY,                      // Top left y coordinate of the region where we want to copy
//        ASrcW,                      // Width                 of the region we want to copy
//        ASrcH                       // Height of the region we want to copy
//      );
//      xcb_flush(MConnection);
//      //#ifdef KODE_CAIRO
//      //cairo_surface_mark_dirty_rectangle(MCairoSurface,src_x,src_y,src_w,src_h);
//      //#endif
//    }
//    //else {
//    //  KODE_Trace("unknown ADrawable for blit()\n");
//    //}
//  }
//
//  //----------
//
//  void stretch(float ADstX, float ADstY, float ADstW, float ADstH, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) final {
//  }

//------------------------------
//------------------------------

//  //----------
//
//  // taken from http://lists.freedesktop.org/archives/xcb/2009-April/004611.html
//  // but https://bitbucket.org/ryanflannery/xtabs/src/4a36b745532d3a75f98ea115ceaabfac839fb8bc/xutil.c
//
//  /*
//  typedef struct xcb_char2b_t {
//    uint8_t byte1;
//    uint8_t byte2;
//  } xcb_char2b_t;
//  */
//
//  xcb_char2b_t* _buildChars(const char *str, size_t length) {
//    unsigned int i;
//    xcb_char2b_t* ret = (xcb_char2b_t *)KODE_Malloc(length * sizeof(xcb_char2b_t));
//    if (!ret) return nullptr;
//    for (i = 0; i < length; i++) {
//      ret[i].byte1 = 0;
//      ret[i].byte2 = str[i];
//    }
//    return ret;
//  }
//
//  //----------

//  void _measureString(const char *string) {
//    //xcb_char2b_t* xcb_str = buildChars(string, KODE_Strlen(string));
//    xcb_char2b_t xcb_str[256];
//    for (uint32_t i = 0; i < KODE_Strlen(string); i++) {
//      xcb_str[i].byte1 = 0;
//      xcb_str[i].byte2 = string[i];
//    }
//    xcb_query_text_extents_cookie_t cookie = xcb_query_text_extents(MConnection, MGC/*MFont*/, KODE_Strlen(string), xcb_str);
//    xcb_query_text_extents_reply_t* reply = xcb_query_text_extents_reply(MConnection, cookie, NULL);
//    MFontAscent   = reply->font_ascent;
//    MFontDescent  = reply->font_descent;
//    MFontWidth    = reply->overall_width;
//    MFontHeight   = reply->font_ascent + reply->font_descent;
//    MFontOrigin   = reply->font_ascent;
//    MFontLeft     = reply->overall_left;
//    MFontRight    = reply->overall_right;
//    //MFontOverallAscent = reply->overall_ascent;
//    //MFontOverallDescent = reply->overall_descent;
//    //KODE_Free(xcb_str);
//    KODE_Free(reply);
//  }

//------------------------------
//------------------------------

//  void drawLine(float x1, float y1, float x2, float y2, uint32_t color) final {
//    setColor(color);
//    xcb_point_t polyline[] =  {
//      (int16_t)x1, (int16_t)y1,
//      (int16_t)x2, (int16_t)y2,
//    };
//    xcb_poly_line(MConnection,XCB_COORD_MODE_ORIGIN,MDrawable,MGC,2,polyline);
//  }
//
//  //----------
//
//  //void drawRect(float x1, float y1, float x2, float y2, uint32_t color) final {
//  void drawRect(KODE_FRect rect, uint32_t color) final {
//    setColor(color);
//    xcb_rectangle_t rectangles[] = {{
//      (int16_t)rect.x,
//      (int16_t)rect.y,
//      (uint16_t)rect.w, // +1
//      (uint16_t)rect.h  // +1
//    }};
//    xcb_poly_rectangle(MConnection,MDrawable,MGC,1,rectangles);
//  }
//
//  //----------
//
//  /*
//    angle 1 = start angle, relative to 3 o'clock
//    angle 2 = 'distance' 0..1, counter-clockwise
//  */
//
//  //void drawArc(float x1, float y1, float x2, float y2, float a1, float a2, uint32_t color) final {
//  void drawArc(KODE_FRect rect, float a1, float a2, uint32_t color) final {
//    setColor(color);
//    // start angle = 12 o'clock
//    float A1 = -a1 + 0.25f;
//    // positive = clockwise, negative = counter-clockwise
//    float A2 = -a2;
//    //XDrawArc(MDisplay, MDrawable,MGC, AX1,AY1,(AX2-AX1+1),(AY2-AY1+1),(a1*(360.0f*64.0f)),(a2*(360.0f*64.0f)));
//    xcb_arc_t arcs[] = {
//      (int16_t)rect.x,
//      (int16_t)rect.y,
//      (uint16_t)rect.w,  // +1
//      (uint16_t)rect.h,  // +1
//      (int16_t)(A1 * 360.0f * 64.0f),
//      (int16_t)(A2 * 360.0f * 64.0f)
//    };
//    xcb_poly_arc(MConnection, MDrawable, MGC, 1, arcs );
//  }
//
//  //----------
//
//  void drawText(float x, float y, const char* txt, uint32_t color) final {
//    setColor(color);
//    //uint8_t* buffer = (uint8_t*)realloc(pt->data, pt->used + required);
//    //if(!buffer) return;
//    //pt->data = buffer;
//    uint8_t buffer[512];
//    KODE_XcbPolyText8 pt;
//    pt.data = buffer;//nullptr;//(uint8_t*)KMalloc(1024);
//    pt.used = 0;
//    kode_xcb_add_string_text8(&pt,txt);
//    //uint8_t* buffer = (uint8_t*)realloc(pt->data, pt->used + required);
//    //if(!buffer) return;
//    //pt->data = buffer;
//    xcb_poly_text_8(MConnection,MDrawable,MGC,x,y,pt.used,pt.data);
//    //if (pt.data) KODE_Free(pt.data);
//  }
//
//  //----------
//
//  void drawText(KODE_FRect rect, const char* txt, uint32_t color, uint32_t alignment) final {
//    _measureString(txt);
//    //int32_t x,y,w;
//    float x,y,w;
//    if (alignment & KODE_TEXT_ALIGN_TOP) y = rect.y + MFontAscent;//MFontStruct->ascent;
//    else if (alignment & KODE_TEXT_ALIGN_BOTTOM) y = rect.y2() - MFontDescent;//MFontStruct->descent;
//    else y = rect.y + (/*MFontStruct->ascent*/MFontAscent * 0.5f) + (rect.h * 0.5f );
//
//    w = MFontWidth;
//
//    if (alignment & KODE_TEXT_ALIGN_LEFT) x = rect.x;
//    else if (alignment & KODE_TEXT_ALIGN_RIGHT) x = rect.x2() - w;
//    else x = rect.x + (rect.w * 0.5f) - ( w * 0.5f );
//    drawText(x,y,txt,color);
//
//  }
//
//  //----------
//
//  //void fillRect(float x1, float y1, float x2, float y2, uint32_t color) final {
//  void fillRect(KODE_FRect ARect, uint32_t color) final {
//    setColor(color);
//    xcb_rectangle_t rectangles[] = {{
//      (int16_t)ARect.x,
//      (int16_t)ARect.y,
//      (uint16_t)ARect.w, // +1
//      (uint16_t)ARect.h  // +1
//    }};
//    xcb_poly_fill_rectangle(MConnection,MDrawable,MGC,1,rectangles);
//  }
//
//  //----------
//
//  // angle 1 = start angle, relative to 3 o'clock
//  // angle 2 = 'distance' 0..1, counter-clockwise
//
//  //void fillArc(float x1, float y1, float x2, float y2, float a1, float a2, uint32_t color) final {
//  void fillArc(KODE_FRect ARect, float a1, float a2, uint32_t color) final {
//    setColor(color);
//    //if (abs(AAngle2) >= 0.01) EPSILON
//    // start angle = 12 o'clock
//    float A1 = -a1 + 0.25;
//    // positive = clockwise, negative = counter-clockwise
//    float A2 = -a2;
//    xcb_arc_t arcs[] = {
//      (int16_t)ARect.x,
//      (int16_t)ARect.y,
//      (uint16_t)ARect.w, // +1
//      (uint16_t)ARect.h, // +1
//      (int16_t)(A1 * 360 * 64),
//      (int16_t)(A2 * 360 * 64)
//    };
//    xcb_poly_fill_arc(MConnection, MDrawable, MGC, 1, arcs );
//    //}
//  }
//
//  //----------
//
//  //void setClip(float x1, float y1, float x2, float y2) final {
//  void setClip(KODE_FRect ARect) final {
//    //resetClip();
//    xcb_rectangle_t rectangles[] = {{
//      (int16_t)ARect.x,
//      (int16_t)ARect.y,
//      (uint16_t)ARect.w, // +1
//      (uint16_t)ARect.h  // +1
//    }};
//    xcb_set_clip_rectangles(
//      MConnection,
//      XCB_CLIP_ORDERING_UNSORTED, // ordering,
//      MGC,
//      0, // clip_x_origin
//      0, // clip_y_origin
//      1, // rectangles_len
//      rectangles
//    );
//    xcb_flush(MConnection);
//  }

  //----------

