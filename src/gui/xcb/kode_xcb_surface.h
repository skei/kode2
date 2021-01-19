#ifndef kode_xcb_surface_included
#define kode_xcb_surface_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_surface.h"
#include "gui/xcb/kode_xcb.h"

class KODE_XcbSurface
: public KODE_BaseSurface
, public KODE_Drawable {

//------------------------------
private:
//------------------------------

  xcb_connection_t* MConnection   = nullptr;
  xcb_visualid_t    MVisual       = XCB_NONE;
  xcb_drawable_t    MDrawable     = XCB_NONE;
  xcb_pixmap_t      MPixmap       = XCB_NONE;
  int32_t           MWidth        = 0;
  int32_t           MHeight       = 0;
  int32_t           MDepth        = 0;
  bool              MIsWindow     = false;
  xcb_window_t      MWindow       = XCB_NONE;

//------------------------------
public:
//------------------------------

  KODE_XcbSurface(KODE_Drawable* AOwner)
  : KODE_BaseSurface(AOwner) {
    MConnection = AOwner->getXcbConnection();
    MDrawable   = AOwner->getXcbDrawable();
    MVisual     = AOwner->getXcbVisual();
    MWidth      = AOwner->getWidth();
    MHeight     = AOwner->getHeight();
    MDepth      = AOwner->getDepth();
    MWindow     = AOwner->getXcbWindow();
    MIsWindow   = true;
    xcb_flush(MConnection);
  }

  //----------

  KODE_XcbSurface(KODE_Drawable* AOwner, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=0)
  : KODE_BaseSurface(AOwner,AWidth,AHeight,ADepth) {
    MConnection = AOwner->getXcbConnection();
    MDrawable   = AOwner->getXcbDrawable();
    MVisual     = AOwner->getXcbVisual();
    MWidth      = AWidth;
    MHeight     = AHeight;
    if (ADepth  == 0) MDepth  = AOwner->getDepth();
    else MDepth  = ADepth;
    //if (AOwner->isWindow()) {
    //  MIsWindow = true;
    //  MWindow = AOwner->getWindow();
    //}
    //else {
      MPixmap = xcb_generate_id(MConnection);
      xcb_create_pixmap(
        MConnection,
        MDepth,
        MPixmap,
        MDrawable,
        AWidth,
        AHeight
      );
    //}
    xcb_flush(MConnection);
  }

  //----------

  virtual ~KODE_XcbSurface() {
    if (!MIsWindow) {
      xcb_free_pixmap(MConnection,MPixmap);
    }
  }

//------------------------------
public:
//------------------------------


//------------------------------
public: // paint target
//------------------------------

  bool              isSurface()         final { return true; }
  bool              isDrawable()        final { return true; }

  uint32_t          getWidth()          final { return MWidth; }
  uint32_t          getHeight()         final { return MHeight; }
  uint32_t          getDepth()          final { return MDepth; }

  xcb_pixmap_t      getXcbPixmap()      final { return MPixmap; }
  xcb_drawable_t    getXcbDrawable()    final { return MPixmap; } //Drawable;

  xcb_connection_t* getXcbConnection()  final { return MConnection; }
  xcb_visualid_t    getXcbVisual()      final { return MVisual; }


//------------------------------
//
//------------------------------

  void createXcbSurface(KODE_Drawable* ATarget) {
  }

//------------------------------
//
//------------------------------

//  void fill(uint32_t AColor) final {
//    fill(0,0,MWidth,MHeight,AColor);
//  }

  //----------

  // can't fill without gc?

//  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) final {
//    // set color
//    uint32_t mask = XCB_GC_FOREGROUND;
//    uint32_t values[1];
//    values[0] = AColor;
//    xcb_change_gc(MConnection,MScreenGC,mask,values);
//    // fill rectangle
//    xcb_rectangle_t rectangles[] = {{
//      (int16_t)AXpos,//0,
//      (int16_t)AYpos,//0,
//      (uint16_t)AWidth,//MWindowWidth,
//      (uint16_t)AHeight,//MWindowHeight
//    }};
//    xcb_poly_fill_rectangle(MConnection,MWindow,MScreenGC,1,rectangles);
//    xcb_flush(MConnection);
//  }

  //----------

//  void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource) final {
//    if (ASource->isImage()) {
//      xcb_image_put(
//        MConnection,            // xcb_connection_t *  conn,
//        MWindow,                // xcb_drawable_t      draw,
//        MScreenGC,              // xcb_gcontext_t      gc,
//        ASource->getXcbImage(), // xcb_image_t *       image,
//        ADstX,                  // int16_t             x,
//        ADstY,                  // int16_t             y,
//        0                       // uint8_t             left_pad
//      );
//      xcb_flush(MConnection);
//    }
//    else if (ASource->isSurface()) {
//      xcb_copy_area(
//        MConnection,                // Pointer to the xcb_connection_t structure
//        ASource->getXcbDrawable(),  // The Drawable we want to paste
//        MWindow,                    // The Drawable on which we copy the previous Drawable
//        MScreenGC,                  // A Graphic Context
//        0,                          // Top left x coordinate of the region we want to copy
//        0,                          // Top left y coordinate of the region we want to copy
//        ADstX,                      // Top left x coordinate of the region where we want to copy
//        ADstY,                      // Top left y coordinate of the region where we want to copy
//        ASource->getWidth(),        // Width                 of the region we want to copy
//        ASource->getHeight()        // Height of the region we want to copy
//      );
//      xcb_flush(MConnection);
//    }
//    //else {
//    //  KODE_Trace("unknown ADrawable for blit()\n");
//    //}
//  }


//  void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) final {
//    if (ASource->isImage()) {
//
//      kode_xcb_put_image(
//        MConnection,
//        MWindow,
//        MScreenGC,
//        ASrcW,
//        ASrcH,
//        ADstX,
//        ADstY,
//        MScreenDepth,//ASource->getDepth(),
//        ASource->getStride(),
//        ASource->getBuffer()
//      );
//
//      xcb_flush(MConnection);
//    }
//    else if (ASource->isSurface()) {
//      xcb_copy_area(
//        MConnection,                // Pointer to the xcb_connection_t structure
//        ASource->getXcbDrawable(),  // The Drawable we want to paste
//        MWindow,                    // The Drawable on which we copy the previous Drawable
//        MScreenGC,                  // A Graphic Context
//        ASrcX,                      // Top left x coordinate of the region we want to copy
//        ASrcY,                      // Top left y coordinate of the region we want to copy
//        ADstX,                      // Top left x coordinate of the region where we want to copy
//        ADstY,                      // Top left y coordinate of the region where we want to copy
//        ASrcW,                      // Width                 of the region we want to copy
//        ASrcH                       // Height of the region we want to copy
//      );
//      xcb_flush(MConnection);
//    }
//    //else {
//    //  KODE_Trace("unknown ADrawable for blit()\n");
//    //}
//  }

};

//----------------------------------------------------------------------
#endif
