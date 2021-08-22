#ifndef kode_cached_widget_included
#define kode_cached_widget_included
//----------------------------------------------------------------------

// unfinished, untested..

#include "gui/kode_surface.h"
#include "gui/kode_widget.h"

//----------------------------------------------------------------------

class KODE_CachedWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  bool          MSurfaceDirty = true;
  KODE_Surface* MSurface      = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_CachedWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_CachedWidget");
  }

  virtual ~KODE_CachedWidget() {
  }

//------------------------------
public:
//------------------------------

  void setDirty(bool ADirty=true) {
    MSurfaceDirty = ADirty;
  }

  void createSurface(uint32_t AWidth, uint32_t AHeight) {
  }

  void deleteSurface() {
  }

  void resizeSurface(uint32_t AWidth, uint32_t AHeight) {
  }

  void updateSurface(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) {
  }

//------------------------------
public:
//------------------------------

  void on_widget_setSize(float AWidth, float AHeight) final {
    resizeSurface(AWidth,AHeight);
    setDirty();
  }

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    if (MSurfaceDirty) {
      updateSurface(APainter,ARect,AMode);
      MSurfaceDirty = false;
    }
    APainter->drawBitmap(ARect.x,ARect.y,MSurface,ARect);
  }

//------------------------------
public:
//------------------------------

  void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) final {
    MSurfaceDirty = true;
    //if (MParent) MParent->do_widget_redraw(ASender,ARect,AMode);
    KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
  }

};

//----------------------------------------------------------------------
#endif
