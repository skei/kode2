#ifndef kode_value_widget_included
#define kode_value_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/kode_widget.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

class KODE_ValueWidget
: public KODE_Widget {

//------------------------------
protected:
//------------------------------

  bool        MFillBackground     = true;
  bool        MDrawNameText       = true;
  bool        MDrawValueText      = true;
  bool        MDrawLabelText      = true;
  uint32_t    MBackgroundColor    = 0xff555555;
  uint32_t    MNameTextColor      = 0xffaaaaaa;
  uint32_t    MValueTextColor     = 0xffffffff;
  uint32_t    MLabelTextColor     = 0xffaaaaaa;

  KODE_FRect  MInnerBorder        = KODE_FRect(5,0,5,0);

  //uint32_t    MHoverTextColor       = 0xff000000;
  //uint32_t    MInteractiveTextColor = 0xff000000;
  //char        MDisplayText[64]      = {0};

//------------------------------
public:
//------------------------------

  KODE_ValueWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    //KODE_Print("x %.1f y %.1f w %.1f h %.1f\n",ARect.x,ARect.y,ARect.w,ARect.h);
  }

  virtual ~KODE_ValueWidget() {
    //KODE_Print("\n");
  };

//------------------------------
public:
//------------------------------

//------------------------------
public: // widget
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) final {

    KODE_FRect rect = MRect;
    rect.shrink(MInnerBorder);

    const char* name  = getName();
    float       value = getValue();
    const char* label = "%";

    KODE_Parameter* param = (KODE_Parameter*)getParameter();
    if (param) {
      name  = param->getName();
      value = param->from01( value );
      label = param->getLabel();
    }

    if (MFillBackground) {
      uint32_t color = MBackgroundColor;
      if (MState.isHovering)  color = 0xff664444;
      APainter->fillRect(MRect,color);
    }

    if (MDrawNameText) {
      //KODE_Strcat(MDisplayText,name);
      //KODE_Strcat(MDisplayText," ");
      APainter->drawText(rect,name,MNameTextColor,KODE_TEXT_ALIGN_LEFT);
    }

    if (MDrawLabelText & (label[0] != 0)) {
      //KODE_Strcpy(buffer," ");
      //KODE_Strcat(buffer,label);
      float labelwidth = APainter->getTextWidth("-");
      labelwidth += APainter->getTextWidth(label);
      APainter->drawText(rect,label,MLabelTextColor,KODE_TEXT_ALIGN_RIGHT);
      rect.w -= labelwidth;
    }

    if (MDrawValueText) {
      char buffer[64];
      KODE_FloatToString(buffer,value);
      APainter->drawText(rect,buffer,MValueTextColor,KODE_TEXT_ALIGN_RIGHT);
    //}
    }

  }

  //void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
  //}

  //void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
  //}

  //void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState) final {
  //}

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom) final {
    do_widget_redraw(this,MRect,0);
  }

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo) final {
    do_widget_redraw(this,MRect,0);
  }

//------------------------------
public: // "widget listener"
//------------------------------

  //void do_widget_update(KODE_Widget* ASender) final {
  //  //KODE_Print("w %p\n",ASender);
  //  KODE_Widget::do_widget_update(ASender);
  //}

  //void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode=0) final {
  //  //KODE_Print("w %p x %.1f y %.1f w %.1f h %.1f (m %i)\n",ASender,ARect.x,ARect.y,ARect.w,ARect.h,AMode);
  //  KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
  //}

};

//----------------------------------------------------------------------
#endif
