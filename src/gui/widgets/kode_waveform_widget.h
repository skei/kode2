#ifndef kode_waveform_widget_included
#define kode_waveform_widget_included
//----------------------------------------------------------------------

#include "gui/kode_widget.h"

//----------------------------------------------------------------------

struct KODE_WaveformMarker {
  //int32_t     pos;
  float       pos;
  KODE_Color  color;
};

struct KODE_WaveformArea {
  //int32_t   pos;
  //int32_t   size;
  float       pos;
  float       size;
  KODE_Color  color;
};

//typedef KArray<KWaveformMarker> KWaveformMarkers;
//typedef KArray<KWaveformArea> KWaveformAreas;

//----------------------------------------------------------------------

class KODE_WaveformWidget
: public KODE_Widget {

//------------------------------
private:
//------------------------------

    float*                MBuffer         = KODE_NULL;
    int32_t               MBufferSize     = 0;
    int32_t               MNumChannels    = 0;
    uint32_t              MStereoMode     = 1; // left

//------------------------------
protected:
//------------------------------

    KODE_Color            MBackColor      = KODE_COLOR_DARK_GRAY;
    KODE_Color            MWaveColor      = KODE_COLOR_LIGHT_GRAY;
    KODE_Color            MGridColor      = KODE_COLOR_GRAY;
    KODE_Color            MZeroColor      = KODE_COLOR_GRAY;
    bool                  MDrawBackground = true;
    int32_t               MNumMarkers     = 0;
    int32_t               MNumAreas       = 0;
    KODE_WaveformMarker*  MMarkers        = KODE_NULL;
    KODE_WaveformArea*    MAreas          = KODE_NULL;
    int32_t               MNumGrid        = 0;
    //bool                  MMono           = false;

//------------------------------
public:
//------------------------------

  KODE_WaveformWidget(KODE_FRect ARect=KODE_FRect())
  : KODE_Widget(ARect) {
    setName("KODE_WaveformWidget");
    setHint("waveform");
  }

  //----------

  virtual ~KODE_WaveformWidget() {
    if (MMarkers) free(MMarkers);
    if (MAreas) free(MAreas);
  }

//------------------------------
public:
//------------------------------

  void setBuffer(float* ABuffer) {
    MBuffer = ABuffer;
  }

  void setBufferSize(int32_t ASize) {
    MBufferSize = ASize;
  }

  void setBackColor(KODE_Color AColor) {
    MBackColor = AColor;
  }

  void setWaveColor(KODE_Color AColor) {
    MWaveColor = AColor;
  }

  void drawBackground(bool ADraw=true) {
    MDrawBackground = ADraw;
  }

  void setNumMarkers(int32_t ANum) {
    MNumMarkers = ANum;
    //SetLength(MMarkers,ANum);
    if (MMarkers) free(MMarkers);
    MMarkers = (KODE_WaveformMarker*)malloc(ANum * sizeof(KODE_WaveformMarker));
  }

  void setNumAreas(int32_t ANum) {
    MNumAreas = ANum;
    //SetLength(MAreas,ANum);
    if (MAreas) free(MAreas);
    MAreas = (KODE_WaveformArea*)malloc(ANum * sizeof(KODE_WaveformArea));
  }

  //void setMarker(int32_t AIndex, int32_t APos, KODE_Color AColor) {
  void setMarker(int32_t AIndex, float APos, KODE_Color AColor) {
    MMarkers[AIndex].pos = APos;
    MMarkers[AIndex].color = AColor;
  }

  //void setMarkerPos(int32_t AIndex, int32_t APos) {
  void setMarkerPos(int32_t AIndex, float APos) {
    MMarkers[AIndex].pos = APos;
  }

  void setMarkerColor(int32_t AIndex, KODE_Color AColor) {
    MMarkers[AIndex].color = AColor;
  }

  //void setArea(int32_t AIndex, int32_t APos, int32_t ASize, KODE_Color AColor) {
  void setArea(int32_t AIndex, float APos, float ASize, KODE_Color AColor) {
    MAreas[AIndex].pos = APos;
    MAreas[AIndex].size = ASize;
    MAreas[AIndex].color = AColor;
  }

  //void setAreaPos(int32_t AIndex, int32_t APos) {
  void setAreaPos(int32_t AIndex, float APos) {
    MAreas[AIndex].pos = APos;
  }

  //void setAreaPosSize(int32_t AIndex, int32_t APos, int32_t ASize) {
  void setAreaPosSize(int32_t AIndex, float APos, float ASize) {
    MAreas[AIndex].pos = APos;
    MAreas[AIndex].size = ASize;
  }

  //void setAreaSize(int32_t AIndex, int32_t ASize) {
  void setAreaSize(int32_t AIndex, float ASize) {
    MAreas[AIndex].size = ASize;
  }

  void setAreaColor(int32_t AIndex, KODE_Color AColor) {
    MAreas[AIndex].color = AColor;
  }

  void setNumGrid(int32_t ANum) {
    MNumGrid = ANum;
  }

  //void setMono(bool AMono=true) {
  //  MMono = AMono;
  //}

//------------------------------
public:
//------------------------------

  void on_widget_paint(KODE_Painter* APainter, KODE_FRect ARect, uint32_t AMode) override {
    KODE_FRect mrect = getRect();
    //KAssert(MBufferSize>0);

    if (mrect.w > 0 ) {

      float h2 = (float)mrect.h * 0.5;
      int32_t midy = mrect.y + (mrect.h * 0.5f);

      /* background */

      if (MDrawBackground) {
        APainter->fillRectangle(mrect,MBackColor);
      }

      /* areas */

      for (int32_t i=0; i<MNumAreas; i++) {
        if (MAreas[i].size > 0.0f) {
          float x = MAreas[i].pos;// / (float)MBufferSize;
          float w = MAreas[i].size;// / (float)MBufferSize;
          int32_t ix = mrect.x + ( x * mrect.w );
          int32_t iw = ix + ( w * mrect.w );
          APainter->fillRectangle(KODE_FRect(ix,mrect.y,iw,mrect.y2()), MAreas[i].color);
        }
      }

      /* waveform */

      if (MBufferSize > 0) {
        if (MBuffer) {
          float xadd = (float)MBufferSize / (float)mrect.w;
          float x = 0;
          //APainter->setDrawColor(MWaveColor);
          //APainter->drawLine( mrect.x, midy, mrect.x2(), midy );
          for (int32_t i=0; i<mrect.w; i++) {
            int32_t index = x;

            //float s;// = 0;
            //if (MMono) s = MBuffer[index];
            //else s = ( MBuffer[ index*2 ] + MBuffer[ index*2 + 1 ] ) * 0.5;

            float s = 0.0f;

            switch (MStereoMode) {
              case 0: s =  MBuffer[index * 2];                                    break; // mono
              case 1: s = (MBuffer[index * 2] + MBuffer[(index * 2) + 1]) * 0.5f; break; // stereo mixdown
              case 2: s =  MBuffer[index * 2];                                    break; // stereo left
              case 3: s =  MBuffer[(index * 2) + 1] ;                             break; // stereo right
            }

            // todo: if (MDrawDb) s = KVolumeToDb(s);
            s *=  h2;
            int32_t ix = mrect.x + i;
            int32_t iy = s;
            APainter->drawLine( ix, midy, ix, midy-iy, MWaveColor,1 );
            x += xadd;
          }
        }

      } // MBufferSize > 0

      /* markers */

      for (int32_t i=0; i<MNumMarkers; i++) {
        float x = MMarkers[i].pos;// / (float)MBufferSize;
        if ((x >= 0.0f) && (x <= 1.0f)) {
          int32_t ix = mrect.x + ( (int)( x * mrect.w ) );
          //APainter->setDrawColor(MMarkers[i].color);
          APainter->drawLine( ix, mrect.y, ix, mrect.y2(), MMarkers[i].color );
        }
      }

      /* grid */

      if (MNumGrid > 1) {
        float xadd = (float)mrect.w / (float)MNumGrid;
        float x = mrect.x + xadd;
        //APainter->setDrawColor(MGridColor);
        for (int32_t i=1; i<MNumGrid; i++) {
          int32_t ix = x;
          APainter->drawLine( ix, mrect.y, ix, mrect.y2(), MGridColor );
          x += xadd;
        }
      }

    } // w>0

    // zero line
    int32_t midy = mrect.y + (mrect.h * 0.5f);
    APainter->drawLine( mrect.x, midy,mrect.x2(),midy, MZeroColor );

    KODE_Widget::on_widget_paint(APainter,ARect,0);
  }

};

//----------------------------------------------------------------------
#endif

