#ifndef kode_grid_widget_included
#define kode_grid_widget_included
//----------------------------------------------------------------------

/*

  todo:
  - selected x1,y1,x2,y2
    invert if necesary
    KODE_List<cells>
  - scroll?
  - cut, copy, paste
  - move/drag


*/

#include "base/utils/kode_math.h"
#include "gui/kode_widget.h"

class KODE_GridWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

  bool        MIsDragging           = false;
  bool        MIsMoving             = false;
  int32_t     MClickedX             = -1;
  int32_t     MClickedY             = -1;
  int32_t     MSelectedX            = -1;
  int32_t     MSelectedY            = -1;
  int32_t     MSelectedXcount       = 0;
  int32_t     MSelectedYcount       = 0;

//------------------------------
protected:
//------------------------------

  int32_t     MNumColumns           = 0;
  int32_t     MNumRows              = 0;          // rows

  bool        MSelectCell           = true;
  bool        MSelectMultipleCells  = true;

  bool        MFillBackground       = false;
  bool        MDrawCells            = true;
  bool        MDrawHorizontalLines  = true;
  bool        MDrawVerticalLines    = true;

  KODE_Color  MBackgroundColor      = KODE_COLOR_GRAY;
  KODE_Color  MGridColor            = KODE_COLOR_DARK_GRAY;

//------------------------------
public:
//------------------------------

  KODE_GridWidget(KODE_FRect ARect)
  : KODE_Widget(ARect) {
    setName("KODE_GridWidget");
    setName("KODE_GridWidget");
    setNumCells(8,8);
  }

  //----------

  virtual ~KODE_GridWidget() {
  }

//------------------------------
public:
//------------------------------

  int32_t     getNumColumns()                           { return MNumColumns; }
  int32_t     getnumRows()                              { return MNumRows; }

  void        setNumColumns(int32_t AWidth)             { MNumColumns = AWidth; }
  void        setNumRows(int32_t AHeight)               { MNumRows = AHeight; }
  void        setNumCells(int32_t AX, int32_t AY)       { MNumColumns = AX; MNumRows = AY; }

  void        setSelectCell(bool ASelect=true)          { MSelectCell = ASelect; }
  void        setSelectMultipleCells(bool ASelect=true) { MSelectMultipleCells = ASelect; }

  void        setDrawCells(bool ADraw=true)             { MDrawCells = ADraw; }
  void        setDrawHorizontalLines(bool ADraw=true)   { MDrawHorizontalLines = ADraw; }
  void        setDrawVerticalLines(bool ADraw=true)     { MDrawVerticalLines = ADraw; }
  void        setBackgroundColor(KODE_Color AColor)     { MBackgroundColor = AColor; }
  void        setGridColor(KODE_Color AColor)           { MGridColor = AColor; }

//------------------------------
public:
//------------------------------

  virtual void on_initCell(KODE_FRect ARect, int32_t AX, int32_t AY) {}
  virtual void on_clickCell(int32_t AX, int32_t AY, int32_t AB) {}
  virtual void on_paintCell(KODE_Painter* APainter, KODE_FRect ARect, int32_t AX, int32_t AY) {}

//------------------------------
public:
//------------------------------

  virtual void initCells(void) {
    KODE_FRect mrect = getRect();
    if ((MNumColumns > 0) && (MNumRows > 0)) {
      float xcell = ( mrect.w / (float)MNumColumns );
      float ycell = ( mrect.h / (float)MNumRows );
      for (int32_t yy=0; yy<MNumRows; yy++) {
        for (int32_t xx=0; xx<MNumColumns; xx++) {
          KODE_FRect R = KODE_FRect(
            mrect.x+floorf(xx*xcell),
            mrect.y+floorf(yy*ycell),
                    floorf(   xcell),
                    floorf(   ycell)
          );
          on_initCell(/*this,*/R,xx,yy);
        }
      }
    }
  }

  //----------

  /*
  virtual void redrawCell(int32_t x, int32_t y) {
    float xcell = ( (float)MRect.w / (float)MNumColumns );
    float ycell = ( (float)MRect.h / (float)MNumRows );
    MSelectedX = x;
    MSelectedY = y;
    //KODE_Trace("%.3f %.3f %.3f %.3f\n",x*xcell,y*ycell,xcell,ycell);
    do_redraw(this,KODE_Rect(x*xcell,y*ycell,xcell,ycell));
  }
  */

//------------------------------
public:
//------------------------------

  /*
    TODO
    - only draw visible cells..
    - cells intersecting with update-rect
  */

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    KODE_FRect mrect = getRect();
    if ((MNumColumns > 0) && (MNumRows > 0)) {
      float xcell = ( mrect.w / (float)MNumColumns );
      float ycell = ( mrect.h / (float)MNumRows );
      // background
      if (MFillBackground) APainter->fillRect(getRect(),MBackgroundColor);
      // cells
      if (MDrawCells) {
        for (int32_t xx=0; xx<MNumColumns; xx++) {
          for (int32_t yy=0; yy<MNumRows; yy++) {
            KODE_FRect R = KODE_FRect(
              mrect.x+floorf(xx*xcell),
              mrect.y+floorf(yy*ycell),
              floorf(xcell),
              floorf(ycell)
            );
            on_paintCell(APainter,R,xx,yy);
          }
        }
      }
      // grid-lines
      float x = mrect.x + xcell - 1;
      float y = mrect.y + ycell - 1;
      if (MDrawVerticalLines) {
        if (MNumColumns > 1) {
          for (int32_t i=0; i<MNumColumns-1; i++) {
            APainter->drawLine( floorf(x), mrect.y, floorf(x), mrect.y2(), MGridColor );
            x += xcell;
          }
        }
      }
      if (MDrawHorizontalLines) {
        if (MNumRows > 1) {
          for (int32_t i=0; i<MNumRows-1; i++) {
            APainter->drawLine( mrect.x, floorf(y), mrect.x2(), floorf(y), MGridColor );
            y += ycell;
          }
        }
      }
      // selected cell(s)
      if ((MSelectedX >= 0) && (MSelectedY >= 0)) {
        x = xcell * MSelectedX;
        y = ycell * MSelectedY;
        float xn = xcell * MSelectedXcount;
        float yn = ycell * MSelectedYcount;
        float ww = xn - 1;
        float hh = yn - 1;
        x  += mrect.x;
        y  += mrect.y;
        APainter->drawRect( KODE_FRect(x,y,ww,hh), KODE_COLOR_WHITE );
      }
    }
  }

  //----------

  /*
    todo:
    - has selection -> move
  */

  void on_widget_mouseClick(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_FRect mrect = getRect();
    float xcell = ( mrect.w / (float)MNumColumns );
    float ycell = ( mrect.h / (float)MNumRows );
    int32_t x = floorf( (float)(AXpos - mrect.x) / xcell );
    int32_t y = floorf( (float)(AYpos - mrect.y) / ycell );
    MClickedX = x;
    MClickedY = y;
    if (MSelectCell) {
      MSelectedX = x;
      MSelectedY = y;
      MSelectedXcount = 1;
      MSelectedYcount = 1;
      if (MSelectMultipleCells) {
        MIsDragging = true;
      }
      on_clickCell(x,y,AButton);
      do_widget_redraw(this,mrect,0);
    }
  }

  //----------

  void on_widget_mouseRelease(float AXpos, float AYpos, uint32_t AButton, uint32_t AState, uint32_t ATimeStamp=0) final {
    MIsDragging = false;
  }

  //----------

  void on_widget_mouseMove(float AXpos, float AYpos, uint32_t AState, uint32_t ATimeStamp=0) final {
    KODE_FRect mrect = getRect();
    float xcell = ( mrect.w / (float)MNumColumns );
    float ycell = ( mrect.h / (float)MNumRows );
    float x = floorf( (float)(AXpos - mrect.x) / xcell );
    float y = floorf( (float)(AYpos - mrect.y) / ycell );
    x = KODE_Clamp(x,0,MNumColumns-1);
    y = KODE_Clamp(y,0,MNumRows-1);
    if (MIsDragging) {
      //if (MIsMoving) {
      //}
      //else {
        if (x < MClickedX) {
          MSelectedX = x;
          MSelectedXcount = MClickedX - x + 1;
        }
        else {
          MSelectedX = MClickedX;
          MSelectedXcount = x - MClickedX + 1;
        }
        if (y < MClickedY) {
          MSelectedY = y;
          MSelectedYcount = MClickedY - y + 1;
        }
        else {
          MSelectedY = MClickedY;
          MSelectedYcount = y - MClickedY + 1;
        }
      //}
      do_widget_redraw(this,mrect,0);
    }
  }

};

//----------------------------------------------------------------------
#endif
