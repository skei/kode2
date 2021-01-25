
/*

Effect engine for a simple reverb plugin

Copyright (c) 2015 Gordon JC Pearce <gordonjcp@gjcp.net>

Permission to use, copy, modify, and/or distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright notice
and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF
THIS SOFTWARE.

*/

#define KODE_NO_GUI

// quite metallic

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

// cat /proc/sys/kernel/random/uuid
// 8f1c1ca5-43ff-4cf5-be20-72118b3e21b6
uint8_t myPluginId[16]  = {0x8f,0x1c,0x1c,0xa5,0x43,0xff,0x4c,0xf5,0xbe,0x20,0x72,0x11,0x8b,0x3e,0x21,0xb6};

/*
  note that buffers need to be a power of 2
  if we scale the tap sizes for higher sample rates,
  this will need to be larger
*/

#define COMB_SIZE 4096
#define COMB_MASK (COMB_SIZE-1)
#define NUM_COMBS 4
#define NUM_APS   3

//----------------------------------------------------------------------

//----------------------------------------------------------------------

/*
  Colour
    is a "tilt" EQ similar to that used on old Quad amps, rolling off the
    treble when turned down and bass when turned up
  Size
    adjusts the size of the "room", and to an extent its shape.
  Decay
    adjusts the feedback trim through the comb filters.
*/

class KODE_SchroederReverb {

  private: // controls

    float   MDecay;
    float   MSize;
    float   MColor;
    float   MLpo;

  private: // internal plugin variables

    float     MComb[NUM_COMBS][COMB_SIZE];  // buffers for comb filters
    uint32_t  MComb_pos;                    // position within comb filter
    float     MAp[NUM_APS][COMB_SIZE];      // lazy, reuse comb size
    uint32_t  MAp_pos;                      // position within allpass filter

    /*
      these values are based on the CCRMA article on Schroeder's original reverb
      they have been scaled to correspond to 44.1kHz Fs instead of 25kHz
    */

    uint32_t  MTap[NUM_COMBS]      = {2975, 2824, 3621, 3970};
    float     MTap_gain[NUM_COMBS] = {0.964, 1.0, 0.939, 0.913};
    uint32_t  MAp_tap[NUM_APS]     = {612, 199, 113};

  public:

    KODE_SchroederReverb() {
    }

    void clear(void) {
      KODE_Memset(MComb,0,sizeof(MComb));
      KODE_Memset(MAp,0,sizeof(MAp));
      MComb_pos = 0;
      MAp_pos   = 0;
    }

    void setDecay(float AValue) { MDecay = AValue; }
    void setSize(float AValue)  { MSize = AValue; }
    void setColor(float AValue) { MColor = AValue; }

    void process(float* in, float* out, uint32_t sample_count) {
      unsigned long pos;
      unsigned long comb_pos = MComb_pos;
      unsigned long ap_pos = MAp_pos;
      int c;
      float* const input = in;
      float* const output = out;
      float in_s, in_s1, temp;
      float gl, gh;
      float tilt = MColor;
      float decay = MDecay;
      if (tilt > 0) {
        gl = -5 * tilt;
        gh = tilt;
      } else {
        gl = -tilt;
        gh = 5 * tilt;
      }
      gl = exp(gl / 8.66) - 1;
      gh = exp(gh / 8.66) - 1;
      //printf("%f %f      ", gl, gh);
      float n = 1 / (5340 + 132300.0);
      float a0 = 2 * 5340 * n;
      float b1 = (132300 - 5340) * n;
      MTap[0] = (int)(2975 * MSize);
      MTap[1] = (int)(2824 * (MSize/2));
      MTap[2] = (int)(3621 * MSize);
      MTap[3] = (int)(3970 * (MSize/1.5));
      MAp_tap[2] = (int)(400 * MSize);
      /* loop around the buffer */
      for (pos = 0; pos < sample_count; pos++) {
        /* loop around the comb filters */
        temp = 0;
        in_s = input[pos]/3;
        MLpo = a0 * in_s + b1 * MLpo;
        in_s1 = in_s + gl * MLpo + gh *(in_s - MLpo);
        for (c = 0; c<NUM_COMBS; c++) {
          MComb[c][(comb_pos + MTap[c]) & COMB_MASK] = in_s1 + (decay * MTap_gain[c]) * MComb[c][comb_pos];
          temp += MComb[c][comb_pos];
        }
        /* loop around the allpass filters */
        #if 1
          for (c = 0; c<NUM_APS; c++) {
            MAp[c][(ap_pos + MAp_tap[c]) & COMB_MASK] = temp + (decay * -0.3535) * MAp[c][ap_pos];
            temp = (decay * 0.3535 * temp) + MAp[c][ap_pos];
          }
        #endif
        output[pos] = temp * 0.35;
        comb_pos++;
        comb_pos &= COMB_MASK;  /* increment and wrap buffer */
        ap_pos++;
        ap_pos &= COMB_MASK;  /* increment and wrap buffer */
        MComb_pos = comb_pos;
        MAp_pos = ap_pos;
      }
    }

};

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName       = "sa.ports.schroeder_reverb";
    MAuthor     = "skei.audio";
    MLongId = myPluginId;

    appendParameter( KODE_New KODE_FloatParameter( "decay", 0.5f,  0.0f, 1.0f ));
    appendParameter( KODE_New KODE_FloatParameter( "size",  0.55f, 0.1f, 1.0f ));
    appendParameter( KODE_New KODE_FloatParameter( "color", 0.0f, -6.0f, 6.0f ));

  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  KODE_SchroederReverb  l_reverb;
  KODE_SchroederReverb  r_reverb;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

public:

  void on_activate() final {
    l_reverb.clear();
    r_reverb.clear();
  }

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    switch (AIndex) {
      case 0:
        l_reverb.setDecay(AValue);
        r_reverb.setDecay(AValue);
        break;
      case 1:
        l_reverb.setSize(AValue);
        r_reverb.setSize(AValue);
        break;
      case 2:
        l_reverb.setColor(AValue);
        r_reverb.setColor(AValue);
        break;
    }
  }

  void on_processBlock(KODE_ProcessContext* AContext) final {
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];

    l_reverb.process(in0,out0,len);
    r_reverb.process(in1,out1,len);

  }

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
