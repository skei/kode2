#ifndef kode_text_widget_included
#define kode_text_widget_included
//----------------------------------------------------------------------

#include "gui/widgets/kode_panel_widget.h"

//----------------------------------------------------------------------

class KODE_TextWidget
: public KODE_PanelWidget {

//------------------------------
protected:
//------------------------------

  bool        MDrawText       = true;
  const char* MText           = "";
  KODE_Color  MTextColor      = KODE_Color(1.0f);
  uint32_t    MTextAlignment  = KODE_TEXT_ALIGN_CENTER;
  KODE_FRect  MTextOffset     = KODE_FRect(2);

//------------------------------
public:
//------------------------------

  KODE_TextWidget(KODE_FRect ARect, const char* AText="")
  : KODE_PanelWidget(ARect) {
    setName("KODE_TextWidget");
    setHint("text");
    MText = AText;
    MDrawBorder = false;
    MFillBackground = false;

    //KODE_Widget* owner = do_widget_get_owner(this);
    //if (owner) {
    //  KODE_PRINT;
    //}

  }

  virtual ~KODE_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawText(bool ADraw) { MDrawText = ADraw; }
  virtual void setTextAlignment(uint32_t AAlignment) { MTextAlignment = AAlignment; }
  virtual void setTextColor(KODE_Color AColor) { MTextColor = AColor; }

  virtual void setText(const char* AText) { MText = AText; }
  virtual const char* getText() { return MText; }

  //----------

  virtual void drawText(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) {
    KODE_FRect rect = getRect();
    rect.shrink(MTextOffset);
    if (MDrawText) {

      KODE_Parameter* param = getParameter();
      if (param) {
        APainter->drawText(rect,param->getName(),MTextAlignment,MTextColor);
      }
      else {
        APainter->drawText(rect,MText,MTextAlignment,MTextColor);
      }

    }
  }

//------------------------------
public:
//------------------------------

  void attachWindow(KODE_BaseWindow* AWindow) override {
    if (AWindow) {
      KODE_BasePainter* painter = AWindow->getPainter();
      if (painter) {
        float w = painter->getTextWidth(MText) + 2;
        float h = painter->getTextHeight(MText);// + 1;
        setInitialSize(w,h);
      }
    }
  }


//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    drawText(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

//------------------------------

};

//----------------------------------------------------------------------
#endif
