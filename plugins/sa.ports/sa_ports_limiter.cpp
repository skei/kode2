
#define KODE_NO_GUI

#include "base/utils/kode_math.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

// cat /proc/sys/kernel/random/uuid
// a3243aba-be82-463d-bacd-4846adb59899
uint8_t myPluginId[16]  = {0xa3,0x24,0x3a,0xba,0xbe,0x82,0x46,0x3d,0xba,0xcd,0x48,0x46,0xad,0xb5,0x98,0x99};

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName       = "sa.ports.limiter";
    MAuthor     = "skei.audio";
    MLongId = myPluginId;

    appendParameter( KODE_New KODE_FloatParameter( "Threshold (dB)", 0, -30,0,1 ));
    appendParameter( KODE_New KODE_FloatParameter( "Ceiling (dB)",   0, -30,0,1 ));


  }

  //----------

  virtual ~myDescriptor() {
    //KODE_TRACE;
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  float     C1 = 0.0f;
  float     C2 = 0.0f;
  float     C3 = 0.0f;
  uint32_t  len = 0;
  uint32_t  bufl = 0;
  uint32_t  bufr = 0;
  uint32_t  pos = 0;
  float     thresh = 0.0f;
  float     make_gain = 0.0f;
  float     ceiling = 0.0f;
  float     BUFFER[1024*1024] = {0};
  float     slider1 = 0.0f;
  float     slider2 = 0.0f;

  float out1 = 0.0f;
  float out2 = 0.0f;
  float out3 = 0.0f;

  bool need_recalc = true;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

private:

  void recalc(float srate) {
    C1    = exp(-1/(25/1000*srate));
    C2    = exp(-1/(25/1000*srate));
    C3    = exp(-1/(5/1000*srate));
    len   = (srate * 10) / 1000; //10/1000*srate;
    bufl  = 0;
    bufr  = len;
    pos   = 0;
  }

public:

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
      switch(AIndex) {
        case 0: slider1 = AValue; break;
        case 1: slider2 = AValue; break;
      }
      thresh    = powf(10,(slider1/20));
      make_gain = powf(10,((-slider1+slider2)/20));
      ceiling   = powf(10,(slider2/20));
  }

  void on_processBlock(KODE_ProcessContext* AContext) final {

    if (need_recalc) {
      need_recalc = false;
      recalc(AContext->samplerate);
    }

    uint32_t len = AContext->numsamples;
    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {

      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;

      float spl0 = *input0++;
      float spl1 = *input1++;
      float in = KODE_Max(fabs(spl0),fabs(spl1));
      // smooth via a rc low pass
      out1 = (in   > out1) ? in   : in   + C1 * (out1-in);
      out2 = (out1 > out2) ? out1 : out1 + C2 * (out2-out1);
      out3 = out2 + C3 * (out3-out2);
      float out = out3;
      BUFFER[bufl+pos] = spl0;
      BUFFER[bufr+pos] = spl1;
      pos = (pos+1) % len;
      float gain = ((out > thresh) ? thresh/out : 1) * make_gain;
      spl0 = BUFFER[bufl+pos] * gain;
      spl0 = KODE_Max(KODE_Min(spl0,ceiling),-ceiling);
      spl1 = BUFFER[bufr+pos] * gain;
      spl1 = KODE_Max(KODE_Min(spl1,ceiling),-ceiling);
      *output0++ = spl0;
      *output1++ = spl1;


    }
  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)






#if 0

/* Copyright (c) 2012, Michael Gruhn
 * All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
 * THIS SOFTWARE.
 */

desc:Limiter
slider1:0<-30,0,1>Threshold (dB)
slider2:0<-30,0,1>Ceiling (dB)

@init
/* rc smoothing filters */
C1 = exp(-1/(25/1000*srate));
C2 = exp(-1/(25/1000*srate));
C3 = exp(-1/(5/1000*srate));

len = 10/1000*srate;
bufl = 0;
bufr = len;
pos = 0;

pdc_delay = len;
pdc_bot_ch = 0;
pdc_top_ch = 2;

@slider
thresh = 10^(slider1/20);
make_gain = 10^((-slider1+slider2)/20);
ceiling = 10^(slider2/20);

@sample
in = max(abs(spl0),abs(spl1));

/* smooth via a rc low pass */
out1 = in  >out1 ? in   : in   + C1 * (out1-in);
out2 = out1>out2 ? out1 : out1 + C2 * (out2-out1);
out3 = out2 + C3 * (out3-out2);

out = out3;

bufl[pos] = spl0;
bufr[pos] = spl1;

pos = (pos+1)%len;

gain = (out > thresh ? thresh/out : 1)*make_gain;

spl0 = bufl[pos] * gain;
spl0 = max(min(spl0,ceiling),-ceiling);
spl1 = bufr[pos] * gain;
spl1 = max(min(spl1,ceiling),-ceiling);


#endif // 0
