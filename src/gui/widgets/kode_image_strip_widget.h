#ifndef kode_template_widget_includedbutton
#define kode_template_widget_included
//----------------------------------------------------------------------

//#include "gui/kode_tilemap.h"
#include "gui/widgets/kode_drag_value_widget.h"

class KODE_ImageStripWidget
: public KODE_DragValueWidget {

//------------------------------
public:
//------------------------------

  KODE_ImageStripWidget(KODE_FRect ARect)
  : KODE_DragValueWidget(ARect) {
    setName("KODE_ImageStripWidget");
    setHint("imagestrip");
  }

  //----------

  virtual ~KODE_ImageStripWidget() {
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    uint32_t num = MTileXcount * MTileYcount;
    if (num > 0) {
      float idx = (float)getValue() * (float)num;
      uint32_t index = KODE_MinI( num-1, floorf(idx) );
      KODE_FRect rect = getTileRect(index);
      APainter->drawBitmap(getRect().x,getRect().y,MWidgetSurface,rect);
    }
  }

};

//----------------------------------------------------------------------
#endif
