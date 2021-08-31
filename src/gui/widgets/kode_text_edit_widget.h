#if 0

#ifndef kode_widget_textedit_included
#define kode_widget_textedit_included
//----------------------------------------------------------------------

#include "base/kode_string_utils.h"
#include "gui/kode_widget.h"

/*
enum EKeyChar {
  kkc_enter     = 13,
  kkc_esc       = 27,
  kkc_home      = 36,
  kkc_end       = 35,
  kkc_left      = 37,
  kkc_right     = 39,
  kkc_delete    = 46,
  kkc_backspace = 8
};
*/

//----------

class KODE_TextEditWidget
: public KODE_Widget {

  private:

    char      MText[256];
    KODE_Color  MTextColor;
    KODE_Color  MBackColor;
    KODE_Color  MCaretColor;
    bool      MEditing;
    int32_t     MCaretPos;

  public:

    KODE_TextEditWidget(KODE_Rect ARect, const char* AText)
    : KODE_Widget(ARect) {
      MName = "KWidget_TextEdit";
      MHint = "textedit";
      strncpy(MText,AText,255);
      MTextColor  = KODE_Black;
      MBackColor  = KODE_LightGrey;
      MCaretColor = KODE_BrightRed;
      MEditing    = false;
      MCaretPos   = 0;
      MCursor = KODE_CURSOR_IBEAM;
    }

    virtual ~KODE_TextEditWidget() {
    }

  private:

    void start_edit() {
      MEditing = true;
      MCaretPos = strlen(MText);
      //do_update(this);
      do_redraw(this,MRect);
      do_setModal(this);
      do_wantKeyEvents(this);
    }

    void stop_edit() {
      MEditing = false;
      do_redraw(this,MRect);
      do_setModal(KODE_NULL);
      do_wantKeyEvents(KODE_NULL);
    }

  public:

    void on_paint(KODE_Painter* APainter, KODE_Rect ARect) override {
      KODE_Widget::on_paint(APainter,ARect);
      APainter->setFillColor(MBackColor);
      APainter->fillRectangle(MRect);
      if (MEditing) {
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-2,MRect.y2(),MText,KODE_TEXT_ALIGN_LEFT);

        char c = MText[MCaretPos];
        MText[MCaretPos] = 0;
        int32_t txtwidth = APainter->getTextWidth(MText);
        MText[MCaretPos] = c;
        int32_t x = MRect.x + 2 + txtwidth;

        APainter->setDrawColor(MCaretColor);
        APainter->drawLine(x,MRect.y,x,MRect.y2());

      } // editing
      else {
        APainter->setTextColor(MTextColor);
        APainter->drawText(MRect.x+2,MRect.y,MRect.x2()-4,MRect.y2(),MText,KODE_TEXT_ALIGN_LEFT);
      } // not editing
    }

    ////----------------------------------------
    //
    //{
    //  assume APos is valid
    //}
    //
    //// pos = 2
    //// len = 6
    //// 01234567
    //// ..::::0
    //
    //procedure insertChar(ABuffer:PChar; APos:LongInt; AChar:Char);
    //var
    //  i,len : longint;
    //begin
    //  if not Assigned(ABuffer) then exit;
    //  len := KStrlen(ABuffer);
    //  for i := (len+1) downto (APos+1) do ABuffer[i] := ABuffer[i-1];
    //  ABuffer[Apos] := AChar;
    //end;
    //
    ////----------
    //
    //procedure deleteChar(ABuffer:PChar; APos:LongInt);
    //var
    //  i,len : longint;
    //begin
    //  if not Assigned(ABuffer) then exit;
    //  len := KStrlen(ABuffer);
    //  for i := Apos to len do ABuffer[i] := ABuffer[i+1];
    //end;

    //----------------------------------------

    // AKey = key code, not ascii..

    void on_keyPress(uint8_t AChar, uint32_t AKey, uint32_t AState) override {

      //KODE_Trace("AChar %i AKey %i AState %i\n",(int)AChar,AKey,AState);

    //var
    //  c : char;
    //  len : longint;
    ////  changed : boolean;

      int32_t len;
      char  c;

//      KODE_Widget::on_keyPress(AChar,AKey,AState);

      //STrace("key: %i, skift: %i\n",AKey,AState);

      #define kkc_enter     65293 // 13
      #define kkc_esc       65307 // 27
      #define kkc_home      65360 // 1
      #define kkc_end       65367 // 2
      #define kkc_left      65361 // 3
      #define kkc_right     65363 // 4
      #define kkc_delete    65535 // 5
      #define kkc_backspace 65288 // 6

      //changed := false;
      switch(AKey) {
        case kkc_enter:
          MEditing = false;
          do_wantKeyEvents(KODE_NULL);
          do_update(this);
          do_redraw(this,MRect);
          //changed := true;
          //KTrace(['"',MText,'"',KODE_CR])
          break;
        case kkc_esc:
          MEditing = false;
          do_wantKeyEvents(KODE_NULL);
          do_update(this);
          do_redraw(this,MRect);
          //changed = true;
          break;
        case kkc_home:
          MCaretPos = 0;
          //changed = true;
            do_update(this);
            do_redraw(this,MRect);
          break;
        case kkc_end:
          len = strlen(MText);
          MCaretPos = len;
          //changed = true;
            do_update(this);
            do_redraw(this,MRect);
          break;
        case kkc_left:
          MCaretPos -= 1;
          if (MCaretPos < 0) MCaretPos = 0;
          //changed = true;
            do_update(this);
            do_redraw(this,MRect);
          break;
        case kkc_right:
          len = strlen(MText);
          MCaretPos += 1;
          if (MCaretPos > len) MCaretPos = len;
          //changed = true;
            do_update(this);
            do_redraw(this,MRect);
          break;
        case kkc_delete:
          len = strlen(MText);
          if ((uint32_t)MCaretPos < strlen(MText)) {
            KODE_DeleteChar(MText,MCaretPos);
            do_update(this);
            do_redraw(this,MRect);
          }
          break;
        case kkc_backspace:
          if (MCaretPos > 0) {
            MCaretPos -= 1;
            KODE_DeleteChar(MText,MCaretPos);
            do_update(this);
            do_redraw(this,MRect);
          }
          break;
        default:
          if ((AKey >= 32) && (AKey <= 127)) {
            c = AKey & 0xff;
            KODE_InsertChar(MText,MCaretPos,c);
            MCaretPos += 1;
            //if FCaretPos > len then FCaretPos = len;
            do_update(this);
            do_redraw(this,MRect);
            //changed = true;
          }
          break;
      } // switch
      //if changed then
      //begin
      //  do_update(this);
      //  do_redraw(this,MRect);
      //end;
    }

    //virtual
    //void on_keyUp(int32 AKey, int32 AState) {
    //}

    void on_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState) override {
      KODE_Widget::on_mouseClick(AXpos,AYpos,AButton,AState);
      switch(AButton) {
        case KODE_BUTTON_LEFT:
          if (MRect.contains(AXpos,AYpos)) {
            start_edit();
          }
          else {
            stop_edit();
          }
          break;
        case KODE_BUTTON_RIGHT:
          stop_edit();
          break;
      }
    }

};

//----------------------------------------------------------------------
#endif

#endif // 0
