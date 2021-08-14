#ifndef kode_saheader_widget_included
#define kode_saheader_widget_included
//----------------------------------------------------------------------

#include "gui//kode_image.h"
#include "gui/widgets/kode_panel_widget.h"

#include "../data/img/sa_logo_40_trans_black.h"

//----------

class KODE_SAHeaderWidget
: public KODE_PanelWidget {

//------------------------------
private:
//------------------------------

  const char* MName = "";
  const char* MVersion = "";
  KODE_Color  MNameColor = KODE_COLOR_BLACK;
  KODE_Color  MVersionColor = KODE_COLOR_DARK_GRAY;

  KODE_Bitmap*  MBitmap   = KODE_NULL;
  KODE_Surface* MSurface  = KODE_NULL;
  KODE_Painter* MPainter  = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_SAHeaderWidget(KODE_FRect ARect, const char* AName, const char* AVersion, KODE_Drawable* ATarget)
  : KODE_PanelWidget(ARect) {
    MFillBackground = false;
    MDrawBorder = false;
    MBackgroundColor = KODE_Color(0.5f);
    MName = AName;
    MVersion = AVersion;
    MLayout.alignment = KODE_WIDGET_ALIGN_TOP;
    MBitmap = KODE_New KODE_Bitmap(sa_logo_40_trans_black,sa_logo_40_trans_black_size);
    MBitmap->premultAlpha( MBackgroundColor );
    MSurface = KODE_New KODE_Surface(ATarget,MBitmap->getWidth(),MBitmap->getHeight());
    KODE_Painter* painter = KODE_New KODE_Painter(MSurface);
    painter->uploadBitmap(0,0,MBitmap);
  }

  virtual ~KODE_SAHeaderWidget() {
    KODE_Delete MPainter;
    KODE_Delete MSurface;
    KODE_Delete MBitmap;
  }

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) final {
    fillBackground(APainter);
    APainter->drawBitmap( MRect.x,      MRect.y,      MSurface );
    APainter->drawText(   MRect.x + 50, MRect.y + 17, MName, MNameColor );
    APainter->drawText(   MRect.x + 50, MRect.y + 31, MVersion, MVersionColor );
    drawBorder(APainter);
  }

//------------------------------
public:
//------------------------------

};

//----------------------------------------------------------------------
#endif

