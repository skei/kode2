#ifndef kode_ctx_painter_included
#define kode_ctx_painter_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_painter.h"
#include "gui/ctx/kode_ctx.h"

//----------------------------------------------------------------------

class KODE_CtxPainter
: public KODE_BasePainter {

//------------------------------
private:
//------------------------------

  KODE_Bitmap*  MBitmap = KODE_NULL;
  Ctx*          MCtx    = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_CtxPainter(KODE_Drawable* ATarget)
  : KODE_BasePainter(ATarget) {
    if (ATarget->isImage()) {
      MBitmap = ATarget->getBitmap();
      uint32_t* buffer  = MBitmap->getBuffer();
      uint32_t  width   = MBitmap->getWidth();
      uint32_t  height  = MBitmap->getHeight();
      uint32_t  stride  = MBitmap->getStride();
      MCtx = ctx_new_for_framebuffer(buffer,width,height,stride,CTX_FORMAT_BGRA8);
    }
    //else if (ATarget->isSurface()) {
    //  KODE_Print("ERROR KODE_CtxPainter doesn't support KODE_Surface targets\n");
    //  exit(1);
    //}
    //else if (ATarget->isWindow()) {
    //  KODE_Print("ERROR KODE_CtxPainter doesn't support KODE_Window targets\n");
    //  exit(1);
    //}
  }

  //----------

  virtual ~KODE_CtxPainter() {
    ctx_free(MCtx);
  }

//------------------------------
public:
//------------------------------

  void clear() final {
    ctx_clear(MCtx);
  }

  void empty() final {
    ctx_empty(MCtx);
  }

  void flush() final {
    ctx_flush(MCtx);
  }

  void save() final {
    ctx_save(MCtx);
  }

  void restore() final {
    ctx_restore(MCtx);
  }

  void rotate(float a) final {
    ctx_rotate(MCtx,a);
  }

  void scale(float x, float y) final {
    ctx_scale(MCtx,x,y);
  }

  void translate(float x, float y) final {
    ctx_translate(MCtx,x,y);
  }

  //typedef enum {
  //  CTX_FILL_RULE_EVEN_ODD,
  //  CTX_FILL_RULE_WINDING
  //} CtxFillRule;

  void setFillRule(uint32_t rule) final {
    ctx_set_fill_rule(MCtx,(CtxFillRule)rule);
  }

  void setGlobalAlpha(float alpha) final {
    ctx_set_global_alpha(MCtx,alpha);
  }

  void setColor(uint32_t AColor) final {
    uint8_t a = (AColor & 0xff000000) >> 24;
    uint8_t r = (AColor & 0x00ff0000) >> 16;
    uint8_t g = (AColor & 0x0000ff00) >> 8;
    uint8_t b = (AColor & 0x000000ff);
    ctx_set_rgba_u8(MCtx,r,g,b,a);
  }

  //#define CTX_LINE_WIDTH_HAIRLINE     -1000.0
  //#define CTX_LINE_WIDTH_ALIASED      -1.0
  //#define CTX_LINE_WIDTH_FAST         -1.0  // aliased 1px wide line

  void setLineWidth(float w) final {
    ctx_set_line_width(MCtx,w);
  }

  void setLineDash(float* dashes, uint32_t numdashes, float offset) final {
  }

  //typedef enum {
  //  CTX_CAP_NONE,
  //  CTX_CAP_ROUND,
  //  CTX_CAP_SQUARE
  //} CtxLineCap;

  void setLineCap(uint32_t cap) final {
    ctx_set_line_cap(MCtx,(CtxLineCap)cap);
  }

  //typedef enum {
  //  CTX_JOIN_BEVEL,
  //  CTX_JOIN_ROUND,
  //  CTX_JOIN_MITER
  //} CtxLineJoin;

  void setLineJoin(uint32_t join) final {
    ctx_set_line_join(MCtx,(CtxLineJoin)join);
  }

  void setFont(const char* name) final {
    ctx_set_font(MCtx,name);
  }

  void setFontSize(float size) final {
    ctx_set_font_size(MCtx,size);
  }

  void loadFont(const char* name, const char* filepath) final {
    ctx_load_font_ttf_file(MCtx,name,filepath);
  }

  void gradientAddStop(float pos, uint32_t color) final {
    uint8_t a = (color & 0xff000000) >> 24;
    uint8_t r = (color & 0x00ff0000) >> 16;
    uint8_t g = (color & 0x0000ff00) >> 8;
    uint8_t b = (color & 0x000000ff);
    ctx_gradient_add_stop_u8(MCtx,pos,r,g,b,a);
  }

  void gradientClearStops() final {
    ctx_gradient_clear_stops(MCtx);
  }

  float getCurrentXPos() final {
    return ctx_x(MCtx);
  }

  float getCurrentYPos() final {
    return ctx_y(MCtx);
  }

  float getTextWidth(const char* str) final {
    return ctx_text_width(MCtx,str);
  }

  float getTextHeight(const char* str) final {
    return 0.0f;
  }

  void newPath() final {
    ctx_new_path(MCtx);
  }

  void closePath() final {
    ctx_close_path(MCtx);
  }

  void fill() final {
    ctx_fill(MCtx);
  }

  void stroke() final {
    ctx_stroke(MCtx);
  }

  void paint() final {
    ctx_paint(MCtx);
  }

  void clip() final {
    ctx_clip(MCtx);
  }

  void linearGradient(float x0, float y0, float x1, float y1) final {
    ctx_linear_gradient(MCtx,x0,y0,x1,y1);
  }

  void radialGradient(float x0, float y0, float r0, float x1, float y1, float r1) final {
    ctx_radial_gradient(MCtx,x0,y0,r0,x1,y1,r1);
  }

  void moveTo(float x, float y) final {
    ctx_move_to(MCtx,x,y);
  }

  void lineTo(float x, float y) final {
    ctx_line_to(MCtx,x,y);
  }

  void curveTo(float cx0, float cy0, float cx1, float cy1, float x, float y) final {
    ctx_curve_to(MCtx,cx0,cy0,cx1,cy1,x,y);
  }

  void quadTo(float cx, float cy, float x, float y) final {
    ctx_quad_to(MCtx,cx,cy,x,y);
  }

  void relMoveTo(float x, float y) final {
    ctx_rel_move_to(MCtx,x,y);
  }

  void relLineTo(float x, float y) {
    ctx_rel_line_to(MCtx,x,y);
  }

  void relCurveTo(float cx0, float cy0, float cx1, float cy1, float x, float y) {
    ctx_rel_curve_to(MCtx,cx0,cy0,cx1,cy1,x,y);
  }

  void relQuadTo(float cx, float cy, float x, float y) {
    ctx_rel_quad_to(MCtx,cx,cy,x,y);
  }

  void relArcTo(float x1, float y1, float x2, float y2, float radius) final {
    ctx_rel_arc_to(MCtx,x1,y1,x2,y2,radius);
  }

  void arcTo(float x1, float y1, float x2, float y2, float radius) final {
    ctx_arc_to(MCtx,x1,y1,x2,y2,radius);
  }

  void setPixel(uint32_t x, uint32_t y, uint32_t color) final {
    uint8_t a = (color & 0xff000000) >> 24;
    uint8_t r = (color & 0x00ff0000) >> 16;
    uint8_t g = (color & 0x0000ff00) >> 8;
    uint8_t b = (color & 0x000000ff);
    ctx_set_pixel_u8(MCtx,x,y,r,g,b,a);
  }

  void rectangle(float x, float y, float w, float h) final {
    ctx_rectangle(MCtx,x,y,w,h);
  }

  void arc(float x, float y, float radius, float angle1, float angle2, int   direction) final {
    ctx_arc(MCtx,x,y,radius,angle1,angle2,direction);
  }

  void drawText(const char* txt) final {
    ctx_text(MCtx,txt);
  }

  void strokeText(const char* txt) final {
    ctx_text_stroke(MCtx,txt);
  }

  void  blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource) final {
  }

  void  blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) final {
  }

  void stretch(float ADstX, float ADstY, float ADstW, float ADstH, KODE_Drawable* ASource, float ASrcX, float ASrcY, float ASrcW, float ASrcH) final {
  }

};

//----------------------------------------------------------------------
#endif



  //void blit(void *data, int x, int y, int width, int height, int stride, uint32_t pixel_format) {
  //  ctx_blit(MCtx,data,x,y,width,height,stride,(CtxPixelFormat)pixel_format);
  //}

  //void imagePath(const char *path, float x, float y) {
  //  ctx_image_path(MCtx,path,x,y);
  //}

  //  int glyph(uint32_t unichar, int stroke) final {
  //    //uint32_t ctx_utf8_to_unichar(const char *utf8);
  //    //int ctx_unichar_to_utf8 (uint32_t  ch, uint8_t *dest)
  //    return ctx_glyph(MCtx,unichar,stroke);
  //  }

