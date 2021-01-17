#ifndef kode_drawable_included
#define kode_drawable_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gfx/kode_bitmap.h"

//----------------------------------------------------------------------

//#ifdef KODE_CAIRO
//  #include "gui/cairo/kode_cairo.h"
//#endif

//#ifdef KODE_CTX
//  #include "gui/ctx/kode_ctx.h"
//#endif

#ifdef KODE_SDL
  #include "gui/sdl/kode_sdl.h"
#endif

#ifdef KODE_XCB
  #include "gui/xcb/kode_xcb.h"
#endif

//----------------------------------------------------------------------

class KODE_Drawable {

//------------------------------
public:
//------------------------------

  //virtual bool          isBitmap()    { return false; }
  virtual bool          isImage()     { return false; }
  virtual bool          isSurface()   { return false; }
  virtual bool          isWindow()    { return false; }
  virtual bool          isDrawable()  { return false; }

  virtual uint32_t      getWidth()    { return 0; }
  virtual uint32_t      getHeight()   { return 0; }
  virtual uint32_t      getDepth()    { return 0; }

  virtual KODE_Bitmap*  getBitmap()   { return KODE_NULL; }
  virtual uint32_t*     getBuffer()   { return KODE_NULL; }
  virtual uint32_t      getStride()   { return 0; }

  //#ifdef KODE_CAIRO
  //  virtual cairo_surface_t*  getCairoSurface() { return KODE_NULL; }
  //#endif

  //#ifdef KODE_CTX
  //  virtual Ctx*              getCtxContext()   { return KODE_NULL; }
  //#endif

  //#ifdef KODE_SDL
  //  virtual SDL_Window*       getSdlWindow()    { return KODE_NULL; }
  //  virtual SDL_Renderer*     getSdlRenderer()  { return KODE_NULL; }
  //  virtual SDL_Texture*      getSdlTexture()   { return KODE_NULL; }
  //#endif

  #ifdef KODE_XCB

    virtual xcb_drawable_t    getXcbDrawable()     { return XCB_NONE; }
    virtual xcb_image_t*      getXcbImage()        { return KODE_NULL; }
    virtual xcb_pixmap_t      getXcbPixmap()       { return XCB_NONE; }
    virtual xcb_window_t      getXcbWindow()       { return XCB_NONE; }

    virtual xcb_connection_t* getXcbConnection()   { return KODE_NULL; }
    virtual xcb_visualid_t    getXcbVisual()       { return XCB_NONE; }

  #endif

  // temp

  virtual void fill(uint32_t AColor) {}
  virtual void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) {}
  virtual void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource) {}
  virtual void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) {}


};

//----------------------------------------------------------------------
#endif
