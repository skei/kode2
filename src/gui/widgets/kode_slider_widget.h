#ifndef kode_value_widget_included
#define kode_value_widget_included
//----------------------------------------------------------------------

#include "base/kode.h"
#include "base/types/kode_rect.h"
#include "gui/kode_painter.h"
#include "gui/widgets/kode_panel_widget.h"
#include "plugin/kode_parameter.h"

//----------------------------------------------------------------------

class KODE_SliderWidget
//: public KODE_Widget {
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------


//------------------------------
protected:
//------------------------------

  KODE_FRect  MTextOffset           = KODE_FRect(5,0,5,0);

  bool        MDrawNameText         = true;
  uint32_t    MNameTextColor        = 0xffa0a0a0;

  bool        MDrawValueText        = true;
  uint32_t    MValueTextColor       = 0xffffffff;

  bool        MDrawLabelText        = true;
  uint32_t    MLabelTextColor       = 0xffa0a0a0;

  bool        MDrawValueBar         = true;
  uint32_t    MValueBarColor        = 0xff404040;
  uint32_t    MHoverValueBarColor   = 0xff383838;
  uint32_t    MInterValueBarColor   = 0xff382828;

  float       MDragSensitivity      = 0.005f;
  float       MPrevMouseX           = 0.0f;
  float       MPrevMouseY           = 0.0f;

//------------------------------
public:
//------------------------------

  KODE_SliderWidget(KODE_FRect ARect)
  : KODE_PanelWidget(ARect) {
    MName = "KODE_SliderWidget";
  }

  //----------

  virtual ~KODE_SliderWidget() {
  };

//------------------------------
public:
//------------------------------

  KODE_FRect getTextOffset()                  { return MTextOffset; };
  void setTextOffset(KODE_FRect AOffset)      { MTextOffset = AOffset; };

  void setDrawNameText(bool ADraw=true)       { MDrawNameText = ADraw; }
  void setNameTextColor(uint32_t AColor)      { MNameTextColor = AColor; }

  void setDrawValueText(bool ADraw=true)      { MDrawValueText = ADraw; }
  void setValueTextColor(uint32_t AColor)     { MValueTextColor = AColor; }

  void setDrawLabelText(bool ADraw=true)      { MDrawLabelText = ADraw; }
  void setLabelTextColor(uint32_t AColor)     { MLabelTextColor = AColor; }

  void setDrawValueBar(bool ADraw=true)       { MDrawValueBar = ADraw; }
  void setValueBarColor(uint32_t AColor)      { MValueBarColor = AColor; }
  void setHoverValueBarColor(uint32_t AColor) { MHoverValueBarColor = AColor; }
  void setInterValueBarColor(uint32_t AColor) { MInterValueBarColor = AColor; }


//------------------------------
protected:
//------------------------------

  virtual void drawValueBar(KODE_Painter* APainter) {
    if (MDrawValueBar) {
      uint32_t color = MValueBarColor;
      if (MIsClicked) color = MInterValueBarColor;
      else if (MIsHovering)  color = MHoverValueBarColor;
      KODE_FRect r = MRect;
      r.w *= MValue;
      APainter->fillRect(r,color);
    }
  }

  //----------

  virtual void drawNameText(KODE_Painter* APainter) {
    KODE_FRect r = MRect;
    r.shrink(MTextOffset);
    if (MDrawNameText) {
      const char* name  = getName();
      KODE_Parameter* parameter = (KODE_Parameter*)getParameter();
      if (parameter) {
        name  = parameter->getName();
      }
      APainter->drawText(r,name,MNameTextColor,KODE_TEXT_ALIGN_LEFT);
    }
  }

  //----------

  virtual void drawValueText(KODE_Painter* APainter) {
    KODE_FRect r = MRect;
    r.shrink(MTextOffset);
    const char* label = "";
    KODE_Parameter* parameter = (KODE_Parameter*)getParameter();
    if (parameter) {
      label = parameter->getLabel();
    }
    if (MDrawLabelText & (label[0] != 0)) {
      float labelwidth = APainter->getTextWidth("_");
      labelwidth += APainter->getTextWidth(label);
      APainter->drawText(r,label,MLabelTextColor,KODE_TEXT_ALIGN_RIGHT);
      r.w -= labelwidth;
    }
    if (MDrawValueText) {
      char buffer[64];
      if (parameter) {
        parameter->getDisplayText(MValue,buffer);
      }
      else {
        KODE_FloatToString(buffer,MValue);
      }
      APainter->drawText(r,buffer,MValueTextColor,KODE_TEXT_ALIGN_RIGHT);
    }
  }

//------------------------------
public: // widget
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode=0) final {
    //_painter = APainter;
    fillBackground(APainter);
    drawValueBar(APainter);
    drawNameText(APainter);
    drawValueText(APainter);
    drawBorder(APainter);
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
    MPrevMouseY = AYpos;
    MPrevMouseX = AXpos;
    MPrevMouseY = AYpos;
    do_widget_redraw(this,MRect,0);
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) final {
    if (MRect.contains(AXpos,AYpos)) MIsHovering = true;
    do_widget_redraw(this,MRect,0);
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState) final {
    float diff = MPrevMouseY - AYpos;
    diff *= MDragSensitivity;
    float value = KODE_Clamp((MValue + diff),0.0f,1.0f);
    if (value != MValue) { // KODE_AlmostEqual
      MValue = value;
      do_widget_update(this);
      do_widget_redraw(this,MRect,0);
    }
    MPrevMouseX = AXpos;
    MPrevMouseY = AYpos;
  }

  //----------

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom) final {
    do_widget_redraw(this,MRect,0);
  }

  //----------

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo) final {
    do_widget_redraw(this,MRect,0);
  }

};

//----------------------------------------------------------------------
#endif
