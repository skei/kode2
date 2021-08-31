#ifndef kode_drawable_included
#define kode_drawable_included
//----------------------------------------------------------------------

#ifdef KODE_USE_XCB
#include "gui/xcb/kode_xcb.h"
#endif

#ifdef KODE_USE_CAIRO
#include "gui/cairo/kode_cairo.h"
#endif

//----------------------------------------------------------------------

struct KODE_Drawable {

  virtual bool              isDrawable()          { return false; }
  virtual bool              isImage()             { return false; }
  virtual bool              isSurface()           { return false; }
  virtual bool              isWindow()            { return false; }
  virtual bool              isCairo()             { return false; }

  virtual uint32_t          getWidth()            { return 0; }
  virtual uint32_t          getHeight()           { return 0; }
  virtual uint32_t          getDepth()            { return 0; }
  virtual KODE_Bitmap*      getBitmap()           { return KODE_NULL; }

  #ifdef KODE_USE_XCB
  virtual xcb_connection_t* getXcbConnection()    { return KODE_NULL; }
  virtual xcb_visualid_t    getXcbVisual()        { return XCB_NONE; }
  virtual xcb_drawable_t    getXcbDrawable()      { return XCB_NONE; }
  virtual xcb_image_t*      getXcbImage()         { return KODE_NULL; }
  virtual xcb_pixmap_t      getXcbPixmap()        { return XCB_NONE; }
  virtual xcb_window_t      getXcbWindow()        { return XCB_NONE; }
  #endif

  #ifdef KODE_USE_CAIRO
  virtual cairo_surface_t*  createCairoSurface()  { return KODE_NULL; }
  #endif

  //#ifdef KODE_USE_CTX
  //virtual Ctx*              getCtxContext()     { return KODE_NULL; }
  //#endif

  //#ifdef KODE_USE_SDL
  //virtual SDL_Window*       getSdlWindow()      { return KODE_NULL; }
  //irtual SDL_Renderer*     getSdlRenderer()    { return KODE_NULL; }
  //virtual SDL_Texture*      getSdlTexture()     { return KODE_NULL; }
  //#endif

};

//----------------------------------------------------------------------
#endif
