#ifndef kode_saheader_widget_included
#define kode_saheader_widget_included
//----------------------------------------------------------------------

#include "gui//kode_image.h"
#include "gui/widgets/kode_panel_widget.h"

//----------

class KODE_SAHeaderWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

//  KODE_Bitmap*  MBitmap   = KODE_NULL;
//  //KODE_Image*   MImage    = KODE_NULL;
//  //KODE_Surface* MSurface  = KODE_NULL;
//  KODE_ImageWidget* MImageWidget  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_SAHeaderWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
//    MBitmap = KODE_New KODE_Bitmap(sa_logo_40_trans_black,sa_logo_40_trans_black_size);
    MLayout.arrangement = KODE_WIDGET_ALIGN_TOP;
  }

  virtual ~KODE_SAHeaderWidget() {
  }

//------------------------------
public:
//------------------------------

  void load_png() {
    MImage = KODE_New KODE_Image(
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_IPainter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    fillBackground(APainter);
    drawBorder(APainter);
  }

//------------------------------
public:
//------------------------------

//  void do_widget_update(KODE_Widget* ASender) final {
//    if (MParent) MParent->do_widget_update(ASender);
//  }
//
//  void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) final {
//    if (MParent) MParent->do_widget_redraw(ASender,ARect,AMode);
//  }
//
//  void do_widget_grabMouse(KODE_Widget* ASender) final {
//    if (MParent) MParent->do_widget_grabMouse(ASender);
//  }
//
//  void do_widget_grabKeyboard(KODE_Widget* ASender) final {
//    if (MParent) MParent->do_widget_grabKeyboard(ASender);
//  }
//
//  void do_widget_grabModal(KODE_Widget* ASender) final {
//    if (MParent) MParent->do_widget_grabModal(ASender);
//  }
//
//  void do_widget_setMouseCursor(KODE_Widget* ASender, uint32_t ACursor) final {
//    if (MParent) MParent->do_widget_setMouseCursor(ASender,ACursor);
//  }
//
//  void do_widget_setMousePos(KODE_Widget* ASender, float AXpos, float AYpos) final {
//    if (MParent) MParent->do_widget_setMousePos(ASender,AXpos,AYpos);
//  }
//
//  void do_widget_setHint(KODE_Widget* ASender, const char* AHint) final {
//    if (MParent) MParent->do_widget_setHint(ASender,AHint);
//  }
//
//  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) final {
//    if (MParent) MParent->do_widget_notify(AWidget,AValue);
//  }

//------------------------------

};

//----------------------------------------------------------------------
#endif

