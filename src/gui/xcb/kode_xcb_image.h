#ifndef kode_xcb_image_included
#define kode_xcb_image_included
//----------------------------------------------------------------------

/*

  https://lists.freedesktop.org/archives/xcb/2006-October/002182.html

  As it stands, xcb_image_put only works for sending full ZPixmap
  images (x_offset==y_offset==0, width==image->width) to a server
  having the same xcb_setup parameters as the client.

*/

//----------------------------------------------------------------------

#include "base/kode.h"
#include "gfx/kode_bitmap.h"
#include "gui/kode_drawable.h"
#include "gui/base/kode_base_image.h"
#include "gui/xcb/kode_xcb.h"

class KODE_XcbImage
: public KODE_BaseImage
, public KODE_Drawable {

//------------------------------
private:
//------------------------------

  xcb_image_t*      MImage            = KODE_NULL;
  bool              MCreatedXcbImage  = false;
  bool              MAllocated        = false;

  uint32_t          MWidth            = 0;
  uint32_t          MHeight           = 0;
  uint32_t          MDepth            = 0;
  uint32_t*         MBuffer           = KODE_NULL;
  uint32_t          MBufferSize       = 0;
  KODE_Bitmap*      MBitmap           = KODE_NULL;

  KODE_Drawable*    MTarget           = KODE_NULL;
  xcb_connection_t* MTargetConnection = KODE_NULL;
  xcb_visualid_t    MTargetVisual     = XCB_NONE;
  xcb_drawable_t    MTargetDrawable   = XCB_NONE;


//------------------------------
public:
//------------------------------

  //todo: lazy creation, in case we only want to use the bitmap..

  KODE_XcbImage(KODE_Drawable* ATarget, uint32_t AWidth, uint32_t AHeight, uint32_t ADepth=32)
  : KODE_BaseImage(ATarget,AWidth,AHeight,ADepth) {
    MTarget           = ATarget;
    MTargetConnection = MTarget->getXcbConnection();
    MTargetVisual     = MTarget->getXcbVisual();
    MTargetDrawable   = MTarget->getXcbDrawable();

    MWidth            = AWidth;
    MHeight           = AHeight;
    MDepth            = ADepth;
    MBitmap           = KODE_New KODE_Bitmap(AWidth,AHeight);
    MBuffer           = MBitmap->getBuffer();
    MBufferSize       = MBitmap->getBufferSize();

    MAllocated        = true;

    //_create_xcb_image(ATarget);

  }

  //----------

  virtual ~KODE_XcbImage() {
    if (MCreatedXcbImage) destroyXcbImage();
    if (MAllocated && MBitmap) KODE_Delete MBitmap;
  }

//------------------------------
private:
//------------------------------

  /*
    //http://tronche.com/gui/x/xlib/utilities/XCreateImage.html
    does not allocate space for the image itself
    initializes the structure byte-order, bit-order, and bitmap-unit values from the display

    //http://www.sbin.org/doc/Xlib/chapt_06.html
    XCreateImage()
    Allocates memory for an XImage structure and sets various members.
    Note that it uses the server's data format, which is often not appropriate.
    The byte- and bit-order fields should usually be changed directly to the client-native format.
    However, then the call _XInitImageFuncPtrs(image) should be issued to reset the mapping
    to the appropriate versions of the functions for manipulating the image.
    This call is supposed to be private to Xlib and, therefore, should be watched for changes
    in later releases, but this is currently the accepted method.

  */

  void createXcbImage(KODE_Drawable* ATarget) {

    //MTarget           = ATarget;
    //MTargetConnection = ATarget->getConnection();
    //MTargetVisual     = ATarget->getVisual();
    //MTargetDrawable   = ATarget->getDrawable();

    uint32_t* buffer = MBitmap->getBuffer();
    uint32_t  bufferSize = MBitmap->getBufferSize();

    MImage = xcb_image_create(
      MWidth,//MBitmap->getWidth(),            // width      width in pixels.
      MHeight,//MBitmap->getHeight(),           // height     height in pixels.
      XCB_IMAGE_FORMAT_Z_PIXMAP,      // format
      32,                             // xpad       scanline pad (8,16,32)
      24,                             // depth      (1,4,8,16,24 zpixmap),    (1 xybitmap), (anything xypixmap)
      32,                             // bpp        (1,4,8,16,24,32 zpixmap,  (1 xybitmap), (anything xypixmap)
      32,                             // unit       unit of image representation, in bits (8,16,32)
      XCB_IMAGE_ORDER_LSB_FIRST,      // byte_order
      XCB_IMAGE_ORDER_LSB_FIRST,      // bit_order
      buffer,           // base       The base address of malloced image data
      bufferSize,       // bytes      The size in bytes of the storage pointed to by base.
                                      //            If base == 0 and bytes == ~0 and data == 0, no storage will be auto-allocated.
      (uint8_t*)buffer  // data       The image data. If data is null and bytes != ~0, then an attempt will be made
                                      //            to fill in data; from base if it is non-null (and bytes is large enough), else
                                      //            by mallocing sufficient storage and filling in base.
    );

    xcb_flush(MTargetConnection);
    MCreatedXcbImage = true;

  }

  //----------

  /*
    //http://www.sbin.org/doc/Xlib/chapt_06.html
    XDestroyImage()
    Frees the data field in an image structure if the image structure was
    allocated in the application. If the image was created using
    XCreateImage(), XGetImage(), or XGetSubImage(), XDestroyImage() frees
    both the data and the image structure. Note that if the image data is
    stored in static memory in the application, it cannot be freed.
    To free an image created with XCreateImage() that has statically
    allocated data, you must set NULL into the data field before calling
    XDestroyImage().
  */

  void destroyXcbImage() {
    //MImage->data = nullptr; // crash
    MImage->base = nullptr;
    xcb_image_destroy(MImage);
  }

  //----------

  //void checkImage() {
  //  if (MCreatedXcbImage == false) {
  //    createXcbImage(MTarget);
  //  }
  //}

  xcb_image_t* _get_image() {
    if (MCreatedXcbImage == false) {
      createXcbImage(MTarget);
    }
    return MImage;
  }

//------------------------------
public:
//------------------------------

  //KODE_Bitmap*  getBitmap() { return MBitmap; }
  //uint32_t*     getBuffer() { return MBitmap ? MBitmap->getBuffer() : KODE_NULL; }
  //uint32_t      getStride() { return MBitmap ? MBitmap->getStride() : 0; }

//------------------------------
public: // paint source,target
//------------------------------

  bool              isImage()       final { return true; }

  uint32_t          getWidth()      final { return MWidth; }
  uint32_t          getHeight()     final { return MHeight; }
  uint32_t          getDepth()      final { return MDepth; }

  uint32_t          getStride()     final { return MBitmap->getStride(); }
  KODE_Bitmap*      getBitmap()     final { return MBitmap; }
  uint32_t*         getBuffer()     final { return MBitmap->getBuffer(); }

  //xcb_image_t*      getXcbImage()   final { checkImage(); return MImage; }
  xcb_image_t*      getXcbImage()   final { return _get_image(); }

  //xcb_drawable_t    getXcbDrawable()    final { checkImage(); return MTargetDrawable; }
  //xcb_connection_t* getXcbConnection()  final { checkImage(); return MTargetConnection; }
  //xcb_visualid_t    getXcbVisual()      final { checkImage(); return MTargetVisual; }

//------------------------------
//
//------------------------------

  void fill(uint32_t AColor) final {
    MBitmap->fill(AColor);
  }

  void fill(int32_t AXpos, int32_t AYpos, int32_t AWidth, int32_t AHeight, uint32_t AColor) final {
    MBitmap->fillArea(AXpos,AYpos,AXpos+AWidth-1,AYpos+AHeight-1,AColor);
  }

  void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource) final {
    if (ASource->isImage()) {
      KODE_Bitmap* bitmap = ASource->getBitmap();
      MBitmap->copyBitmap(ADstX,ADstY,bitmap);
    }
  }

  void blit(int32_t ADstX, int32_t ADstY, KODE_Drawable* ASource, int32_t ASrcX, int32_t ASrcY, int32_t ASrcW, int32_t ASrcH) final {
    if (ASource->isImage()) {
      KODE_Bitmap* bitmap = ASource->getBitmap();
      KODE_Bitmap* sub = bitmap->createSubBitmap(ASrcX,ASrcY,ASrcX,ASrcH);
      MBitmap->copyBitmap(ADstX,ADstY,sub);
      KODE_Delete sub;
    }
  }

//------------------------------
//
//------------------------------

};

//----------------------------------------------------------------------
#endif
