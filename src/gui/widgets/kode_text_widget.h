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
  bool        MDrawParamText  = true;
  const char* MText           = "";
  KODE_Color  MTextColor      = KODE_COLOR_DARK_GRAY;
  uint32_t    MTextAlignment  = KODE_TEXT_ALIGN_LEFT;
  KODE_FRect  MTextOffset     = KODE_FRect(2,2,2,2);
  float       MTextSize       = 10;

//------------------------------
public:
//------------------------------

  KODE_TextWidget(KODE_FRect ARect/*=KODE_FRect()*/, const char* AText="")
  : KODE_PanelWidget(ARect) {
    setName("KODE_TextWidget");
    setHint("text");
    MText = AText;
    MDrawBorder = false;
    MFillBackground = false;

  }

  virtual ~KODE_TextWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void setDrawText(bool ADraw)                { MDrawText = ADraw; }
  virtual void setDrawParamText(bool ADraw)           { MDrawParamText = ADraw; }
  virtual void setTextAlignment(uint32_t AAlignment)  { MTextAlignment = AAlignment; }
  virtual void setTextColor(KODE_Color AColor)        { MTextColor = AColor; }
  virtual void setTextOffset(KODE_FRect AOffset)      { MTextOffset = AOffset; }
  virtual void setTextOffset(float x)                 { MTextOffset = KODE_FRect(x,x,x,x); }
  virtual void setTextOffset(float x, float y)        { MTextOffset = KODE_FRect(x,y); }
  virtual void setText(const char* AText)             { MText = AText; }
  virtual void setTextSize(float ASize)               { MTextSize = ASize; }

  virtual const char* getText()                       { return MText; }

  //----------

  virtual void drawText(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) {
    KODE_FRect rect = getRect();
    rect.shrink(MTextOffset);
    if (MDrawText) {
      const char* txt = MText;
      if (MDrawParamText) {
        KODE_Parameter* param = getParameter();
        if (param) {
          //APainter->drawText(rect,param->getName(),MTextAlignment,MTextColor);
          txt = param->getName();
        }
      }
      //else {
      //  //APainter->drawText(rect,MText,MTextAlignment,MTextColor);
      //  txt = MText;
      //}
      APainter->drawText(rect,txt,MTextAlignment,MTextColor);
    }
  }

//------------------------------
public:
//------------------------------

  void attachWindow(KODE_BaseWindow* AWindow) override {
    if (AWindow) {
      if (flags.autoSize) {
        KODE_BasePainter* painter = AWindow->getPainter();
        if (painter) {
          float w = painter->getTextWidth(MText) + 2;
          float h = painter->getTextHeight(MText);// + 1;
          setInitialSize(w,h);
        }
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
