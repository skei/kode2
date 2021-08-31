#if 0

#ifndef kode_knob3_widget_included
#define kode_knob3_widget_included
//----------------------------------------------------------------------

/*
      _
     / \ label
     \_/ value

*/

#include "base/kode.h"
#include "gui/kode_widget.h"
#include "gui/kode_widgets.h"
#include "plugin/kode_parameter.h"

#define TEXT_HEIGHT 16

class KODE_Knob3Widget
: public KODE_Widget {

  protected:

    KODE_KnobWidget*  wdg_knob        = nullptr;
    KODE_TextWidget*  wdg_label       = nullptr;
    KODE_TextWidget*  wdg_value       = nullptr;
    char              value_text[32]  = {0};

  //------------------------------
  //
  //------------------------------

  public:

    KODE_Knob3Widget(KODE_Rect ARect, float AValue=0.0f, bool ABipolar=false)
    : KODE_Widget(ARect) {
      MName = "KODE_Widget_Knob2";
      setValue(AValue);
      MHint = "knob3";

      //#define X MRect.x
      //#define Y MRect.y
      //#define X 0
      //#define Y 0
      //#define W MRect.w
      //#define H MRect.h
      //#define KS W

      sprintf(value_text,"%.3f",AValue);

      #define B 5

      float s   = MRect.h;
      float s05 = s * 0.5f;
      float ss = (MRect.w - B) - s;

      wdg_knob  = new KODE_KnobWidget( KODE_Rect( 0, 0,   s, s    ), AValue );
      wdg_label = new KODE_TextWidget( KODE_Rect( s, 0,   B + ss, s05 ), "text" );
      wdg_value = new KODE_TextWidget( KODE_Rect( s, s05, B + ss, s05 ), value_text );

      appendWidget(wdg_knob);
      appendWidget(wdg_label);
      appendWidget(wdg_value);

      #undef B

      //#undef X
      //#undef Y
      //#undef W
      //#undef H
      //#undef KS

      wdg_knob->setValue(AValue);
      wdg_knob->setFillBackground(true);
      wdg_knob->setBackgroundColor(KODE_Grey);
      wdg_knob->setBipolar(ABipolar);

      //wdg_label->setTextSize(10);
      wdg_label->setFillBackground(true);
      wdg_label->setBackgroundColor(KODE_Grey);
      wdg_label->setDrawBorder(false);
      wdg_label->setTextColor(KODE_Black);
      wdg_label->setTextAlignment(KODE_TEXT_ALIGN_LEFT);

      //wdg_value->setTextSize(10);
      wdg_value->setFillBackground(true);
      wdg_value->setBackgroundColor(KODE_Grey);
      wdg_value->setDrawBorder(false);
      wdg_value->setTextColor(KODE_Black);
      wdg_value->setTextAlignment(KODE_TEXT_ALIGN_LEFT);
      wdg_value->setValue(AValue);

    }

    //----------

    virtual ~KODE_Knob3Widget() {
    }

  //------------------------------
  //
  //------------------------------

  public:

    virtual KODE_KnobWidget* getKnobWidget()  { return wdg_knob; }
    virtual KODE_TextWidget* getLabelWidget() { return wdg_label; }
    virtual KODE_TextWidget* getValueWidget() { return wdg_value; }

  //------------------------------
  //
  //------------------------------

  public:

    void on_connect(KODE_Parameter* AParameter) override {
      wdg_knob->setParameter(AParameter);
      if (AParameter) {
        wdg_label->setText( AParameter->getName() );
        float value = 0.0f;
        //const char* txt = AParameter->getDisplayText(value);
        //KODE_FloatToString(value_text,AValue);
        sprintf(value_text,"%.3f",value);
        wdg_value->setText(value_text);
      }
    }

    //----------

    void setValue(float AValue) override {
      KODE_Widget::setValue(AValue);
      //KODE_Assert(wdg_knob != KODE_NULL);
      if (wdg_knob) {
        wdg_knob->setValue(AValue);
      }
      if (wdg_value) {

        //if (MParameter) {
        //  KODE_Parameter* par = (KODE_Parameter*)MParameter;
        KODE_Parameter* par = (KODE_Parameter*)getParameter();
        if (par) {

          //KODE_String txt = par->getDisplayText(AValue);
          //wdg_value->setText(txt);
          par->getDisplayText(AValue,value_text);
          wdg_value->setText(value_text);
        }
        else {
          //KODE_FloatToString(value_text,AValue);
          sprintf(value_text,"%.3f",AValue);
          wdg_value->setText(value_text);
        }
      }
    }

    //----------

    void do_update(KODE_Widget* ASender) override {
      if (MParent) MParent->do_update(this);
      if (ASender==wdg_knob) {
        setValue( ASender->getValue() );

        //if (MParameter) {
        //  KODE_Parameter* par = (KODE_Parameter*)MParameter;
        KODE_Parameter* par = (KODE_Parameter*)getParameter();
        if (par) {

          par->getDisplayText(getValue(),value_text);
          wdg_value->setText(value_text);
        }
        do_redraw(wdg_value,wdg_value->getRect());
      }
      else {
        if (MParent) MParent->do_update(this);
      }
    }

};

#undef TEXT_HEIGHT

//----------------------------------------------------------------------
#endif

#endif // 0
