#ifndef kode_keyboard_widget_included
#define kode_keyboard_widget_included
//----------------------------------------------------------------------

/*
  mind-storm:
    black or white, black = 1 rect, white = 2 rect
    only 2 numbers per rectangle is unique
    use rectangles for hit testing (mouse click)
    precalc them in setup(scale,dist,y1,y2) ?
    array with 12 elememnts, one for each note in octave
    1 find octave
    octave width = mScale*3*7
    so, a key could be
    - int black/white
    -  num1,num2
    -  num1,num2
*/

//
//  |  |4| | |  |
//  |  |_| |_|  |
//  |1  |2  |3  |
//  |___|___|___|
//

#include "gui/kode_widget.h"

// ofs,type

//static
const int32_t kode_keyboard_internal_note_data[12*2] = {
  0,0,
  2,3,
  1,1,
  2,3,
  1,2,
  3,0,
  2,3,
  1,1,
  2,3,
  1,1,
  2,3,
  1,2
};

//----------------------------------------------------------------------

class KODE_KeyboardWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  bool        MActiveMap[12];
  int32_t     MNoteRects1[12*4];
  int32_t     MNoteRects2[12*4];

//------------------------------
protected:
//------------------------------

  int32_t     MDist;
  float       MRatio;
  KODE_Color  MBlackColor;
  KODE_Color  MWhiteColor;
  KODE_Color  MBlackActiveColor;
  KODE_Color  MWhiteActiveColor;

//------------------------------
public:
//------------------------------

  KODE_KeyboardWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_KeyboardWidget");
    setHint("keyboard");
    setCursor(KODE_CURSOR_FINGER);
    MDist             = 2;
    MRatio            = 0.6;
    MWhiteColor       = KODE_COLOR_LIGHT_GRAY;
    MWhiteActiveColor = KODE_COLOR_WHITE;
    MBlackColor       = KODE_COLOR_DARK_GRAY;
    MBlackActiveColor = KODE_COLOR_BLACK;
    for (int32_t i=0; i<12; i++) MActiveMap[i] = false;
    //recalc();
    setCursor(KODE_CURSOR_FINGER);
  }

  virtual ~KODE_KeyboardWidget() {
  }

//------------------------------
public:
//------------------------------

  public:

    void setColors(KODE_Color AWhite, KODE_Color ABlack, KODE_Color AWhiteActive, KODE_Color ABlackActive) {
      MWhiteColor       = AWhite;
      MWhiteActiveColor = AWhiteActive;
      MBlackColor       = ABlack;
      MBlackActiveColor = ABlackActive;
    }

    void setDist(int32_t ADist) {
      MDist = ADist + 1;
    }

    void setRatio(float ARatio) {
      MRatio = ARatio;
    }

    void activate(int32_t num) {
      MActiveMap[num] = true;
    }

    void deactivate(int32_t num) {
      MActiveMap[num] = false;
    }

    //----------

    void recalc(void) {
    /*var
      width : longint;
      step1,step2,step3 : single;
      x1 : single;
      y1,y2 : longint;
      note,n2,n4,iofs : longint;
      ofs,x1s1,x1s2,x1s3:single;
      typ:longint;*/
      //KODE_Trace("MRect.w %i\n",(int)MRect.w);
      KODE_FRect mrect = getRect();
      int32_t width = mrect.w;
      float step1 = (float)width / 21.0;
      float step2 = step1 * 2;
      float step3 = step1 * 3;
      float x1    = 0;
      int32_t y1    = floorf((float)mrect.h * MRatio);
      int32_t y2    = mrect.h - 1;
      for (int32_t note=0; note<12; note++) {
        int32_t n2    = note+note;
        int32_t n4    = n2+n2;
        int32_t iofs  = kode_keyboard_internal_note_data[note*2];
        float ofs   = (float)iofs * step1;
        x1         += ofs;
        float x1s1  = x1 + step1;
        float x1s2  = x1 + step2;
        float x1s3  = x1 + step3;
        int32_t typ   = kode_keyboard_internal_note_data[(note*2)+1];
        switch(typ) {
          case 0: // C
            MNoteRects1[n4+0] = floorf(x1);            MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 1: // D
            MNoteRects1[n4+0] = floorf(x1s1);          MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 2: // E
            MNoteRects1[n4+0] = floorf(x1s1);          MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s3) - MDist;  MNoteRects1[n4+3] = y1;
            MNoteRects2[n4+0] = floorf(x1);            MNoteRects2[n4+1] = y1;
            MNoteRects2[n4+2] = floorf(x1s3) - MDist;  MNoteRects2[n4+3] = y2;
            break;
          case 3: // black
            MNoteRects1[n4+0] = floorf(x1);            MNoteRects1[n4+1] = 0;
            MNoteRects1[n4+2] = floorf(x1s2) - MDist;  MNoteRects1[n4+3] = y1 - MDist;
            break;
        } // caseKWidget_Keyboard
      } // for note
    }

    //----------

    void fill_rect(KODE_Painter* APainter, float x, float y, int32_t n, uint32_t which, KODE_Color color) {
      KODE_FRect r;
      switch (which) {
        case 1:
          r.x =     x + MNoteRects1[n  ];
          r.y =     y + MNoteRects1[n+1];
          r.setX2(  x + MNoteRects1[n+2] );
          r.setY2(  y + MNoteRects1[n+3] );
          break;
        case 2:
          r.x =     x + MNoteRects2[n  ];
          r.y =     y + MNoteRects2[n+1];
          r.setX2(  x + MNoteRects2[n+2] );
          r.setY2(  y + MNoteRects2[n+3] );
          break;
      }
      APainter->fillRectangle( r, color );
    }


//------------------------------
public:
//------------------------------

//  void on_widget_setPos(float AXpos, float AYpos) final {
//  }

//  void on_widget_setSize(float AWidth, float AHeight) final {
//    //  KODE_Widget::setSize(w,h);
//    //  recalc();
//  }

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
      KODE_FRect mrect = getRect();
      recalc();
      //KODE_TRACE;
      for (int32_t note=0; note<12; note++) {
        int32_t n2 = note*2;
        int32_t n4 = note*4;
        bool active = MActiveMap[note];
        int32_t typ = kode_keyboard_internal_note_data[n2+1];
        if (typ == 3) {
          if (active) {
//            //APainter->setFillColor(MBlackActiveColor);
//            APainter->fillRectangle( KODE_FRect( mrect.x + MNoteRects1[n4],  mrect.y + MNoteRects1[n4+1],
//                                            mrect.x + MNoteRects1[n4+2],mrect.y + MNoteRects1[n4+3] ), MBlackActiveColor);
            fill_rect(APainter,mrect.x,mrect.y,n4,1,MBlackActiveColor);
          }
          else {
//            //APainter->setFillColor(MBlackColor);
//            APainter->fillRectangle( KODE_FRect( mrect.x + MNoteRects1[n4],  mrect.y + MNoteRects1[n4+1],
//                                            mrect.x + MNoteRects1[n4+2],mrect.y + MNoteRects1[n4+3] ), MBlackColor);
            fill_rect(APainter,mrect.x,mrect.y,n4,1,MBlackColor);
          }
        }
        else {

          if (active) {
//            //APainter->setFillColor(MWhiteActiveColor);
//            APainter->fillRectangle( KODE_FRect( mrect.x + MNoteRects1[n4  ], mrect.y + MNoteRects1[n4+1],
//                                            mrect.x + MNoteRects1[n4+2], mrect.y + MNoteRects1[n4+3] ), MWhiteActiveColor);
//
//            APainter->fillRectangle( KODE_FRect( mrect.x + MNoteRects2[n4  ], mrect.y + MNoteRects2[n4+1],
//                                            mrect.x + MNoteRects2[n4+2], mrect.y + MNoteRects2[n4+3] ), MWhiteActiveColor);
            fill_rect(APainter,mrect.x,mrect.y,n4,1,MWhiteActiveColor);
            fill_rect(APainter,mrect.x,mrect.y,n4,2,MWhiteActiveColor);
          }
          else {
//            //APainter->setFillColor(MWhiteColor);
//            APainter->fillRectangle( KODE_FRect( mrect.x + MNoteRects1[n4  ], mrect.y + MNoteRects1[n4+1],
//                                            mrect.x + MNoteRects1[n4+2], mrect.y + MNoteRects1[n4+3] ), MWhiteColor);
//
//            APainter->fillRectangle( KODE_FRect( mrect.x + MNoteRects2[n4  ], mrect.y + MNoteRects2[n4+1],
//                                            mrect.x + MNoteRects2[n4+2], mrect.y + MNoteRects2[n4+3] ), MWhiteColor);
            fill_rect(APainter,mrect.x,mrect.y,n4,1,MWhiteColor);
            fill_rect(APainter,mrect.x,mrect.y,n4,2,MWhiteColor);
          }

        } //type=0,1,2
      } //for note;
  }

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
      KODE_FRect mrect = getRect();
    //var
    //  hit,note,n2,n4,typ : longint;
    //  rec : KRect;
      if (AButton == KODE_BUTTON_LEFT) {
        KODE_FRect rec;
        int32_t hit = -1;
        for (int32_t note=0; note<12; note++) {
          int32_t n4 = note*4;
          rec.x =  mrect.x + MNoteRects1[n4  ];
          rec.y =  mrect.y + MNoteRects1[n4+1];
          rec.w = (mrect.x + MNoteRects1[n4+2]) - rec.x;
          rec.h = (mrect.y + MNoteRects1[n4+3]) - rec.y;
          if (rec.contains(AXpos,AYpos)) {
            hit = note;
            if (MActiveMap[note]) deactivate(note);
            else activate(note);
            do_widget_update(this);
            do_widget_redraw(this,mrect,0/*,krs_opaque*/);
          } // contain
          if (hit < 0) { // check lower part?
            int32_t n2 = note*2;
            int32_t typ = kode_keyboard_internal_note_data[n2+1];
            if (typ < 3) { //0,1,2 = white keys
              rec.x =  mrect.x + MNoteRects2[n4  ];
              rec.y =  mrect.y + MNoteRects2[n4+1];
              rec.w = (mrect.x + MNoteRects2[n4+2]) - rec.x;
              rec.h = (mrect.y + MNoteRects2[n4+3]) - rec.y;
              if (rec.contains(AXpos,AYpos)) {
                if (MActiveMap[note]) deactivate(note);
                else activate(note);
                do_widget_update(this);
                do_widget_redraw(this,mrect,0/*,krs_opaque*/);
              } //contains
            } //type 0,1,2
          } //hit
        } // for note
      }
      //axWidget::doMouseDown(aXpos,aYpos,aButton);
  }

//  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_keyPress(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_keyRelease(uint32_t AKey, char AChar, uint32_t AState, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_enter(float AXpos, float AYpos, KODE_Widget* AFrom, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_leave(float AXpos, float AYpos, KODE_Widget* ATo, uint32_t ATimeStamp=0) final {
//  }
//
//  void on_widget_connect(KODE_Parameter* AParameter, uint32_t ASubIndex) final {
//  }

//------------------------------
public:
//------------------------------

//  void do_widget_update(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_update(ASender);
//  }
//
//  void do_widget_redraw(KODE_Widget* ASender, KODE_FRect ARect, uint32_t AMode) final {
//    KODE_Widget::do_widget_redraw(ASender,ARect,AMode);
//  }
//
//  void do_widget_moved(KODE_Widget* ASender, float ADeltaX=0.0f, float ADeltaY=0.0f) final {
//    KODE_Widget::do_widget_moved(ASender,ADeltaX,ADeltaY);
//  }
//
//  void do_widget_resized(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_resized(ASender);
//  }
//
//  void do_widget_grabMouseCursor(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabMouseCursor(ASender);
//  }
//
//  void do_widget_grabKeyboard(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabKeyboard(ASender);
//  }
//
//  void do_widget_grabModal(KODE_Widget* ASender) final {
//    KODE_Widget::do_widget_grabModal(ASender);
//  }
//
//  void do_widget_setMouseCursor(KODE_Widget* ASender, int32_t ACursor) final {
//    KODE_Widget::do_widget_setMouseCursor(ASender,ACursor);
//  }
//
//  void do_widget_setMouseCursorPos(KODE_Widget* ASender, float AXpos, float AYpos) final {
//    KODE_Widget::do_widget_setMouseCursorPos(ASender,AXpos,AYpos);
//  }
//
//  void do_widget_setHint(KODE_Widget* ASender, const char* AHint) final {
//    KODE_Widget::do_widget_setHint(ASender,AHint);
//  }
//
//  void do_widget_notify(KODE_Widget* AWidget, uint32_t AValue=0) final {
//    KODE_Widget::do_widget_notify(AWidget,AValue);
//  }

};

//----------------------------------------------------------------------
#endif
