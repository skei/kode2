#ifndef kode_audio_utils_included
#define kode_audio_utils_included
//----------------------------------------------------------------------

#include "base/kode_math.h"

//----------------------------------------------------------------------
// interface
//----------------------------------------------------------------------

//------------------------------
// mono
//------------------------------

void KODE_CopyMonoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void KODE_ClearMonoBuffer(float** ADst, uint32_t ALength);
void KODE_AddMonoBuffer(float** ADst, float** AAdd, uint32_t ALength);
void KODE_ScaleMonoBuffer(float** ADst, float AScale, uint32_t ALength);

void KODE_CopyMonoBuffer(float* ADst, float* ASrc, uint32_t ALength);
void KODE_ClearMonoBuffer(float* ADst, uint32_t ALength);
void KODE_AddMonoBuffer(float* ADst, float* AAdd, uint32_t ALength);
void KODE_ScaleMonoBuffer(float* ADst, float AScale, uint32_t ALength);

// KODE_PLUGIN_PROCESS_TICKSIZE

//void KODE_CopyMonoBuffer(float** ADst, float** ASrc);
//7void KODE_ClearMonoBuffer(float** ADst);
//void KODE_AddMonoBuffer(float** ADst, float** AAdd);
//void KODE_ScaleMonoBuffer(float** ADst, float AScale);

//void KODE_CopyMonoBuffer(float* ADst, float* ASrc);
//void KODE_ClearMonoBuffer(float* ADst);
//void KODE_AddMonoBuffer(float* ADst, float* AAdd);
//void KODE_ScaleMonoBuffer(float* ADst, float AScale);

//------------------------------
// stereo
//------------------------------

void KODE_CopyStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void KODE_ClearStereoBuffer(float** ADst, uint32_t ALength);
void KODE_AddStereoBuffer(float** ADst, float** AAdd, uint32_t ALength);
void KODE_ScaleStereoBuffer(float** ADst, float AScale, uint32_t ALength);
void KODE_ScaleStereoBuffer(float** ADst, float ALeft, float ARight, uint32_t ALength);
void KODE_ClampStereoBuffer(float** ADst, float AMin, float AMax, uint32_t ALength);

// KODE_PLUGIN_PROCESS_TICKSIZE

//void KODE_CopyStereoBuffer(float** ADst, float** ASrc);
//void KODE_ClearStereoBuffer(float** ADst);
//void KODE_AddStereoBuffer(float** ADst, float** AAdd);
//void KODE_ScaleStereoBuffer(float** ADst, float AScale);
//void KODE_ScaleStereoBuffer(float** ADst, float ALeft, float ARight);

//------------------------------
// mono -> stereo
//------------------------------

void KODE_CopyMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void KODE_AddMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength);
void KODE_CopyMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength);
void KODE_AddMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength);

// KODE_PLUGIN_PROCESS_TICKSIZE

//void KODE_CopyMonoToStereoBuffer(float** ADst, float** ASrc);
//void KODE_AddMonoToStereoBuffer(float** ADst, float** ASrc);
//void KODE_CopyMonoToStereoBuffer(float** ADst, float* ASrc);
//void KODE_AddMonoToStereoBuffer(float** ADst, float* ASrc);

//----------------------------------------------------------------------
// implementation
//----------------------------------------------------------------------

//------------------------------
// mono
//------------------------------

void KODE_CopyMonoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  KODE_Memcpy(ADst[0],ASrc[0],ALength*sizeof(float));
}

//----------

void KODE_ClearMonoBuffer(float** ADst, uint32_t ALength) {
  KODE_Memset(ADst[0],0,ALength*sizeof(float));
}

//----------

void KODE_AddMonoBuffer(float** ADst, float** AAdd, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void KODE_ScaleMonoBuffer(float** ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst[0];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------

void KODE_CopyMonoBuffer(float* ADst, float* ASrc, uint32_t ALength) {
  KODE_Memcpy(ADst,ASrc,ALength*sizeof(float));
}

//----------

void KODE_ClearMonoBuffer(float* ADst, uint32_t ALength) {
  KODE_Memset(ADst,0,ALength*sizeof(float));
}

//----------

void KODE_AddMonoBuffer(float* ADst, float* AAdd, uint32_t ALength) {
  float* dst0 = ADst;
  float* add0 = AAdd;
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void KODE_ScaleMonoBuffer(float* ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst;
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------
// mono, ticksize
//------------------------------

/*

void KODE_CopyMonoBuffer(float** ADst, float** ASrc) {
  KODE_Memcpy(ADst[0],ASrc[0],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void KODE_ClearMonoBuffer(float** ADst) {
  KODE_Memset(ADst[0],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void KODE_AddMonoBuffer(float** ADst, float** AAdd) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void KODE_ScaleMonoBuffer(float** ADst, float AScale) {
  float* dst0 = ADst[0];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
  }
}

//------------------------------

void KODE_CopyMonoBuffer(float* ADst, float* ASrc) {
  KODE_Memcpy(ADst,ASrc,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void KODE_ClearMonoBuffer(float* ADst) {
  KODE_Memset(ADst,0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void KODE_AddMonoBuffer(float* ADst, float* AAdd) {
  float* dst0 = ADst;
  float* add0 = AAdd;
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
  }
}

//----------

void KODE_ScaleMonoBuffer(float* ADst, float AScale) {
  float* dst0 = ADst;
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
  }
}

*/

//------------------------------
// stereo
//------------------------------

void KODE_CopyStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  KODE_Memcpy(ADst[0],ASrc[0],ALength*sizeof(float));
  KODE_Memcpy(ADst[1],ASrc[1],ALength*sizeof(float));
}

//----------

void KODE_ClearStereoBuffer(float** ADst, uint32_t ALength) {
  KODE_Memset(ADst[0],0,ALength*sizeof(float));
  KODE_Memset(ADst[1],0,ALength*sizeof(float));
}

//----------

void KODE_AddStereoBuffer(float** ADst, float** AAdd, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  float* dst1 = ADst[1];
  float* add1 = AAdd[1];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ += *add0++;
    *dst1++ += *add1++;
  }
}

//----------

void KODE_ScaleStereoBuffer(float** ADst, float AScale, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

void KODE_ScaleStereoBuffer(float** ADst, float ALeft, float ARight, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    *dst0++ *= ALeft;
    *dst1++ *= ARight;
  }
}

//----------

void KODE_ClampStereoBuffer(float** ADst, float AMin, float AMax, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<ALength; i++) {
    float spl0 = *dst0;
    float spl1 = *dst1;
    spl0 = KODE_Clamp(spl0,AMin,AMax);
    spl1 = KODE_Clamp(spl1,AMin,AMax);
    *dst0++ = spl0;
    *dst1++ = spl1;
  }
}

//------------------------------
// stereo, ticksize
//------------------------------

/*

void KODE_CopyStereoBuffer(float** ADst, float** ASrc) {
  KODE_Memcpy(ADst[0],ASrc[0],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
  KODE_Memcpy(ADst[1],ASrc[1],KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void KODE_ClearStereoBuffer(float** ADst) {
  KODE_Memset(ADst[0],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
  KODE_Memset(ADst[1],0,KPLUGIN_PROCESS_TICKSIZE*sizeof(float));
}

//----------

void KODE_AddStereoBuffer(float** ADst, float** AAdd) {
  float* dst0 = ADst[0];
  float* add0 = AAdd[0];
  float* dst1 = ADst[1];
  float* add1 = AAdd[1];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ += *add0++;
    *dst1++ += *add1++;
  }
}

//----------

void KODE_ScaleStereoBuffer(float** ADst, float AScale) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= AScale;
    *dst1++ *= AScale;
  }
}

//----------

void KODE_ScaleStereoBuffer(float** ADst, float ALeft, float ARight) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    *dst0++ *= ALeft;
    *dst1++ *= ARight;
  }
}

*/

//------------------------------
// mono -> stereo
//------------------------------

void KODE_CopyMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc[0];
  for (uint32_t i=0; i<ALength; i++) {
    float spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void KODE_AddMonoToStereoBuffer(float** ADst, float** ASrc, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc[0];
  for(uint32_t i=0; i<ALength; i++) {
    float spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------

void KODE_CopyMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc;
  for (uint32_t i=0; i<ALength; i++) {
    float spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void KODE_AddMonoToStereoBuffer(float** ADst, float* ASrc, uint32_t ALength) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc;
  for(uint32_t i=0; i<ALength; i++) {
    float spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------
// mono -> stereo, ticksize
//------------------------------

/*

void KODE_CopyMonoToStereoBuffer(float** ADst, float** ASrc) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc[0];
  for (uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    float spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void KODE_AddMonoToStereoBuffer(float** ADst, float** ASrc) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc[0];
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    float spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

//------------------------------

void KODE_CopyMonoToStereoBuffer(float** ADst, float* ASrc) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc;
  for (uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    float spl0 = *src0++;
    *dst0++ = spl0;
    *dst1++ = spl0;
  }
}

//----------

void KODE_AddMonoToStereoBuffer(float** ADst, float* ASrc) {
  float* dst0 = ADst[0];
  float* dst1 = ADst[1];
  float* src0 = ASrc;
  for(uint32_t i=0; i<KPLUGIN_PROCESS_TICKSIZE; i++) {
    float spl0 = *src0++;
    *dst0++ += spl0;
    *dst1++ += spl0;
  }
}

*/

//----------------------------------------------------------------------
#endif
