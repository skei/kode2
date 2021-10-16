#ifndef kode_dual_slider_widget_included
#define kode_dual_slider_widget_included
//----------------------------------------------------------------------

// TODO:

#include "gui/widgets/kode_slider_widget.h"

//----------------------------------------------------------------------

class KODE_DualSliderWidget
: public KODE_SliderWidget {

//------------------------------
protected:
//------------------------------

  float       MValue2               = 0.0f;
  char        MValue1Text[32]       = {0};
  KODE_Color  MValue1TextColor      = KODE_Color(0.0f);
  uint32_t    MValue1TextAlignment  = KODE_TEXT_ALIGN_LEFT;
  KODE_FRect  MValue1TextOffset     = KODE_FRect(2,2,2,2);

  char        MValue2Text[32]       = {0};
  KODE_Color  MValue2TextColor      = KODE_Color(0.0f);
  uint32_t    MValue2TextAlignment  = KODE_TEXT_ALIGN_RIGHT;
  KODE_FRect  MValue2TextOffset     = KODE_FRect(2,2,2,2);

  KODE_Color  MEdgeColor            = KODE_COLOR_WHITE;
  uint32_t    MEdgeWidth            = 3;
  int32_t     MHoverEdge            = -1;
  bool        MIsDragging1          = false;
  bool        MIsDragging2          = false;
  int32_t     MEdgeDistance         = 3;

//------------------------------
public:
//------------------------------

  KODE_DualSliderWidget(KODE_FRect ARect)
  : KODE_SliderWidget(ARect) {
    setName("KODE_DualSliderWidget");
    setHint("dual_slider");
    flags.autoCursor      = false;
    flags.autoMouseHide   = true;
    //flags.autoMouseRedraw = true;

  }

  virtual ~KODE_DualSliderWidget() {
  }

//------------------------------
public:
//------------------------------

  virtual void  setValue2(float AValue) { MValue2 = AValue; }

  virtual void setBackgroundColor(KODE_Color AColor)  { MBackgroundColor = AColor; }
  //virtual void setBarColor(KODE_Color AColor)         { MBarColor = AColor; }
  virtual void setEdgeColor(KODE_Color AColor)        { MEdgeColor = AColor; }

  //----------

  virtual float getValue2(void) { return MValue2; }

//------------------------------
private:
//------------------------------

  virtual void findHoverEdge(float AXpos, bool ASetCursor=true) {
    KODE_FRect mrect = getRect();
    float v = (AXpos - mrect.x) / mrect.w;
    float d1 = v - getValue();
    float d2 = v - getValue2();
    int32_t d = 0;
    int32_t prev = MHoverEdge;
    MHoverEdge = -1;
    //const char* hint = "";
    if (fabs(d1) <= fabs(d2)) {
      d = d1 * mrect.w;
      if (d <= -MEdgeDistance) {
        //hint = "PgDn";
      }
      else if (d >=  MEdgeDistance) {
        //hint = "left thumb";
      }
      else {
        MHoverEdge = 0;
        //hint = "left";
      }
    }
    else {
      d = d2 * mrect.w;
      if (d <= -MEdgeDistance) {
        //hint = "right thumb";
      }
      else if (d >=  MEdgeDistance) {
        //hint = "PgUp";
      }
      else {
        MHoverEdge = 1;
        //hint = "right";
      }
    }

    if (MHoverEdge != prev) {
      //do_widget_setHint(this,hint);
      do_widget_redraw(this,mrect,0);
      if (ASetCursor) {
        if (MHoverEdge >= 0) {
          do_widget_setMouseCursor(this,KODE_CURSOR_ARROWLEFTRIGHT);
        }
        else {
          do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
        }
      }
    }
  }

  //----------

  void drawValues(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) {
    if (MDrawValueText) {
      float value1 = getValue();
      float value2 = getValue2();
      const char* label = "";
      KODE_Parameter* param = getParameter();
      if (param) {
        label = param->getLabel();
        param->getDisplayString(value1,MValue1Text);
        param->getDisplayString(value2,MValue2Text);
      }
      else {
        label = MLabel;
        KODE_FloatToString(MValue1Text,value1);
        KODE_FloatToString(MValue2Text,value2);
      }
      KODE_FRect value1_rect = getRect();
      KODE_FRect value2_rect = getRect();
      KODE_FRect label_rect = getRect();
      value1_rect.shrink(MValue1TextOffset);
      value2_rect.shrink(MValue2TextOffset);
      //label_rect.shrink(MValueTextOffset);
      //if (MDrawLabel) {
      //  float width = APainter->getTextWidth(label);
      //  label_rect.x = value_rect.x2() - width;
      //  label_rect.w = width;
      //  value_rect.w -= (width + KODE_VALUE_WIDGET_LABEL_SPACE);
      //}
      if (MDrawValueText) {
//        APainter->setFontSize(14);
        APainter->drawText(value1_rect,MValue1Text,MValue1TextAlignment,MValue1TextColor);
        APainter->drawText(value2_rect,MValue2Text,MValue2TextAlignment,MValue2TextColor);
      }
      if (MDrawLabel) {
        APainter->drawText(label_rect,label,KODE_TEXT_ALIGN_CENTER,MLabelColor);
      }
    }
  }

  //----------

  virtual void drawValueBar2(KODE_BasePainter* APainter, KODE_FRect ARect, uint32_t AMode) {
    KODE_FRect mrect = getRect();
    float x1 = mrect.x + (mrect.w * getValue());
    float x2 = mrect.x + (mrect.w * getValue2()) - 1;
    float w  = (x2 - x1 + 1);
    //if (w > 0) {
      if (w < 3) {
        APainter->fillRectangle( KODE_FRect(x1-1,mrect.y,2,mrect.h), MValueBarColor );
      }
      else {
        APainter->fillRectangle( KODE_FRect(x1,mrect.y,w,mrect.h), MValueBarColor );
      }
    //}
    if (MHoverEdge == 0) {
      APainter->fillRectangle( KODE_FRect(x1,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
    }
    else if (MHoverEdge == 1) {
      APainter->fillRectangle( KODE_FRect(x2-MEdgeWidth,mrect.y,MEdgeWidth,mrect.h), MEdgeColor );
    }
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    fillBackground(APainter,ARect,AMode);
    //paintChildren(APainter,getRect(),AMode);
    drawValueBar2(APainter,ARect,AMode);
//    drawText(APainter,ARect,AMode);
    drawValues(APainter,ARect,AMode);
    drawBorder(APainter,ARect,AMode);
  }

  //----------

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_SliderWidget::on_widget_mouseClick(AXpos,AYpos,AButton,AState,ATimeStamp);
    if (AButton == KODE_BUTTON_LEFT) {
      if (MHoverEdge==0) MIsDragging1 = true;
      else if (MHoverEdge==1) MIsDragging2 = true;
      //MIsInteractive = true;
      //if (hasFlag(KODE_WIDGET_HIDECURSOR)) do_setCursor(this,KODE_CURSOR_HIDE);
//      do_widget_redraw(this,getRect(),0);
    }
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_SliderWidget::on_widget_mouseRelease(AXpos,AYpos,AButton,AState,ATimeStamp);
    if (AButton == KODE_BUTTON_LEFT) {
      MIsDragging1 = false;
      MIsDragging2 = false;
      if (getRect().contains(AXpos,AYpos)) findHoverEdge(AXpos);
      else {
        MHoverEdge = -1;
        do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
      }
      //MIsInteractive = false;
      //if (hasFlag(KODE_WIDGET_HIDECURSOR)) do_setCursor(this,KODE_CURSOR_SHOW);
//      do_widget_redraw(this,getRect(),0);
    }
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {

    //float preValue = getValue();
    //float preValue2 = getValue2();

    //KODE_SliderWidget::on_widget_mouseMove(AXpos,AYpos,AState,ATimeStamp);

    //float v = calcValueFromMouse(AXpos,AYpos,AState);
    //KODE_Print("v %f\n",v);
    float value1 = getValue();
    float value2 = getValue2();
    if (MIsDragging1) {
      value1 = calcValueFromMouse(AXpos,AYpos,AState);//v;
      if (value1 > value2) {
        setValue(value2);
        setValue2(value1);
        //KODE_Swap(MIsDragging1,MIsDragging2);
        {
          bool temp = MIsDragging1;
          MIsDragging1 = MIsDragging2;
          MIsDragging2 = temp;
        }
        MHoverEdge = 1;
      }
      else {
        setValue(value1);
        setValue2(value2);
      }
      do_widget_update(this);
      do_widget_redraw(this,getRect(),0);
    }
    else if (MIsDragging2) {
      value2 = calcValueFromMouse(AXpos,AYpos,AState);//v;
      if (value1 > value2) {
        setValue(value2);
        setValue2(value1);
        //KODE_Swap(MIsDragging1,MIsDragging2);
        {
          bool temp = MIsDragging1;
          MIsDragging1 = MIsDragging2;
          MIsDragging2 = temp;
        }
        MHoverEdge = 0;
      }
      else {
        setValue(value1);
        setValue2(value2);
      }
      do_widget_update(this);
      do_widget_redraw(this,getRect(),0);
    }
    //if (!(MIsDragging1 || MIsDragging2)) {
    else {
      findHoverEdge(AXpos,false);
    }
  }

  //----------

  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) final {
    findHoverEdge(AXpos,false);
    //if (flags.autoHint) do_widget_setHint(this,getHint());
    //KODE_SliderWidget::on_widget_enter(AXpos,AYpos,AFrom,ATimeStamp);
  }

  //----------

  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) final {
    if (MHoverEdge >= 0) {
      MHoverEdge = -1;
      do_widget_redraw(this,getRect(),0);
      do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
    }
    do_widget_setMouseCursor(this,KODE_CURSOR_DEFAULT);
    //if (flags.autoHint) do_widget_setHint(this,"");
    //KODE_SliderWidget::on_widget_leave(AXpos,AYpos,ATo,ATimeStamp);
  }

};

//----------------------------------------------------------------------
#endif
