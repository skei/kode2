#ifndef kode_sa_header_widget_included
#define kode_sa_header_widget_included
//----------------------------------------------------------------------

#include "gui/kode_image.h"
#include "gui/widgets/kode_background_widget.h"
#include "gui/widgets/kode_text_widget.h"

#include "../data/sa_logo_40_trans_black.h"

//----------------------------------------------------------------------

class KODE_SAHeaderWidget
: public KODE_BackgroundWidget {

private:

  KODE_Bitmap*  bitmap            = KODE_NULL;
  KODE_Image*   image             = KODE_NULL;
  char          text2_buffer[256] = {0};

public:

  KODE_SAHeaderWidget(KODE_Drawable* ATarget, const char* AName, const char* AVersion, const char*AFormat="")
  : KODE_BackgroundWidget( KODE_FRect(60)) {

    setFillBackground();
    setBackgroundColor( KODE_Color(0.50f) );
    setAlignment(KODE_WIDGET_ALIGN_FILL_TOP);

    bitmap = KODE_New KODE_Bitmap(sa_logo_40_trans_black,sa_logo_40_trans_black_size);

//    image = KODE_New KODE_Image(ATarget,sa_logo_40_trans_black,sa_logo_40_trans_black_size);
//    image->getBitmap()->premultAlpha();
//    KODE_ImageWidget* image1 = KODE_New KODE_ImageWidget(KODE_Rect( 10,10, 40,40 ), image);
//    appendWidget(image1);

    KODE_TextWidget* text1 = KODE_New KODE_TextWidget( KODE_FRect( 60,10, 100,20 ), AName );
    text1->setFillBackground(false);
    text1->setDrawBorder(false);
    appendChildWidget(text1);

    KODE_Strcpy(text2_buffer,AVersion);
    KODE_Strcat(text2_buffer," : ");
    KODE_Strcat(text2_buffer,AFormat);

    KODE_TextWidget* text2 = KODE_New KODE_TextWidget( KODE_FRect( 60,30, 100,20 ), text2_buffer );
    text2->setFillBackground(false);
    text2->setDrawBorder(false);
    appendChildWidget(text2);

  }

  virtual ~KODE_SAHeaderWidget() {
    if (image) KODE_Delete image;
    if (bitmap) KODE_Delete bitmap;
  }

};

//----------------------------------------------------------------------
#endif
