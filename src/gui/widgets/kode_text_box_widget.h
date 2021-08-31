#if 0

#ifndef kode_widget_text_box_included
#define kode_widget_text_box_included
//----------------------------------------------------------------------

#if 0

/*
  maybe this one needs some locking?
  if i press midi keys in a synth plugin, and draws text output to
  this window from the process thread (a big no no! but..)..
  during drawing, some of the members, like the string list might
  be updated, and we can mismatches, etc..

  we can do:
  * queue?
  * locks
*/

#include "base/kode_lock.h"
#include "base/kode_string.h"
#include "gui/kode_widget.h"
#include "gui/widgets/kode_color_widget.h"
#include "gui/widgets/kode_scroll_bar_widget.h"

typedef KODE_Array<KODE_String*> KODE_StringArray;

//----------------------------------------------------------------------

class KODE_TextBoxWidget
//: public KWidget {
: public KODE_ColorWidget {

  private:

    KODE_ScrollBarWidget*  MScrollBar;
    KODE_StringArray        MStrings;
    uint32_t                MMaxStrings;
    uint32_t                MStringHeight;

    //KLock               MLock;

  //------------------------------
  //
  //------------------------------

  public:

    KODE_TextBoxWidget(KODE_Rect ARect, uint32_t AMaxStrings)
    //: KWidget(ARect,AAlignment) {
    : KODE_ColorWidget(ARect,KODE_Grey) {
      MName         = "KWidget_TextArea";
      MHint         = "textarea";
      setPadding(2);
      //MStrings.clear();
      MMaxStrings   = AMaxStrings;
      MStringHeight = 16;
      MScrollBar    = new KODE_ScrollBarWidget( KODE_Rect(10), true );
      MScrollBar->setValue(0);
      MScrollBar->setThumbSize(1);
      appendWidget( MScrollBar );
    }

    //----------

    virtual ~KODE_TextBoxWidget() {
      #ifndef KODE_NO_AUTODELETE
      deleteStrings();
      #endif
    }

  //------------------------------
  //
  //------------------------------

  public:

    void appendText(const char* AText) {
      //MLock.lock();
      if (MStrings.size() >= MMaxStrings ) {
        delete MStrings[0];
        //MStrings.remove(0);
        MStrings.removeHead();
      }
      MStrings.append( new KODE_String(AText) );
      //MLock.unlock();
    }

    //----------

    // cppcheck: va_end missing

    void appendString(const char* format, ...) {
      char buffer[256];
      //KStrcpy(buffer,MPrefix);
      va_list args;
      va_start(args,format);
      vsprintf(buffer,format,args);
      appendText(buffer);
      //va_end(args);
    }

    //----------

    void deleteStrings(void) {
      for (uint32 i=0; i<MStrings.size(); i++) {
        //MLock.lock();
        delete MStrings[i];
        MStrings[i] = KODE_NULL;
        //MLock.unlock();
      }
    }

  //------------------------------
  //
  //------------------------------

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect/*, uint32 AMode=0*/) override {
      //MLock.lock();
      //KWidget::on_widgetPaint(AWidget,APainter,ARect,AMode);
      KWidget_Color::on_widget_paint(AWidget,APainter,ARect/*,AMode*/);
      int32 num_strings = MStrings.size();
      if (num_strings>0) {
        float total_height = MStrings.size() * MStringHeight;
        if (total_height > MRect.h) {
          MScrollBar->thumbsize( MRect.h / total_height );
          float num_visible = (float)MRect.h / (float)MStringHeight;
          float offset = (float)( total_height - MRect.h ) * MScrollBar->getValue();
          float first_visible = offset / MStringHeight;
          float first_string = KTrunc(first_visible);
          float first_offset = (first_visible - first_string) * MStringHeight;
          //KDTrace("num_visible %.2f offset %.2f first_visible %.2f first_string %.2f first_offset %.2f\n",num_visible,offset,first_visible,first_string,first_offset);
          int32 first = first_string;
          int32 num   = ceil( num_visible );
          int32 y = -first_offset;
          APainter->setTextColor(KBlack);
          for (int32 i=0; i<num; i++) {
            APainter->drawText(2,2+y+i*MStringHeight,MStrings[first+i]->ptr());
          }
        }
        else {
          MScrollBar->thumbsize(1);
          APainter->setTextColor(KBlack);
          for (int32 i=0; i<num_strings; i++) {
            APainter->drawText(2,2+i*MStringHeight,MStrings[i]->ptr());
          }
        }
      }
      //MLock.unlock();
    }

  //------------------------------
  //
  //------------------------------

  public:

  //virtual
  void do_update(KODE_Widget* AWidget) {
    //KWidget::do_widgetUpdate(AWidget);
    KWidget_Color::do_widget_update(AWidget);
    if (AWidget==MScrollBar) {
      do_widget_redraw(this,MRect,krs_opaque);
    }
  }

};

#endif // 0

//----------------------------------------------------------------------
#endif

#endif // 0
