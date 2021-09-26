#ifndef kode_filter_ms20_included
#define kode_filter_ms20_included
//----------------------------------------------------------------------

// gpl
// see /plugins/gpl/fx_filter_ms20.h

class KODE_Ms20Filter {

//------------------------------
private:
//------------------------------

  float     z1 = 0;
  float     z2 = 0;

//------------------------------
private:
//------------------------------

  uint32_t  p_type  = 0;      // 0=lowpass, 1=highpass
  float     p_freq  = 1.0f;
  float     p_reso  = 0.0f;   // 0.0  - 1.1
  float     p_clip  = 0.5f;   // 0.01 - 4.0
  float     p_pre   = 1.0;    // 0.0  - 2.0
  float     p_post  = 1.0;    // 0.0  - 2.0

//------------------------------
public:
//------------------------------

  void setType(uint32_t a) { p_type = a; }
  void setFreq(float a) { p_freq = a; }
  void setReso(float a) { p_reso = a; }
  void setClip(float a) { p_clip = a; }
  void setPre(float a) { p_pre = a; }
  void setPost(float a) { p_post = a; }

//------------------------------
public:
//------------------------------

  KODE_Ms20Filter() {
    z1 = 0;
    z2 = 0;
  }

  //----------

  // arbitrarily made up, asymmetric to introduce even harmonics

  float shape_gain(float x) {
    return 1 / sqrt(1.0 + 0.66 * x + x * x);
  }

  //----------

  float shape(float clip_level, float x) {
    return x * shape_gain(x / clip_level);
  }

  //----------

  float tick(float freq, float reso, float clip_level, float lp_input, float hp_input) {
    float k = fmin(1.0, freq * KODE_PI2);
    float z2_prime = z2 + hp_input;
    float z1_prime = z1 + shape(clip_level, 2 * z2_prime * reso);
    z1 += (lp_input - z1_prime) * k;
    z1_prime += (lp_input - z1_prime) * k;
    z2 += (z1_prime - z2_prime) * k;
    z2_prime += (z1_prime - z2_prime) * k;
    return z2_prime;
  }

  //----------

  float tick_lp(float freq, float reso, float clip_level, float input) {
    return tick(freq, reso, clip_level, input, 0.0);
  }

  //----------

  float tick_hp(float freq, float reso, float clip_level, float input) {
    return tick(freq, reso, clip_level, 0.0, input);
  }

  //----------

  void process(float** AInputs, float** AOutputs, uint32_t ANumSamples) {
    float* input0 = AInputs[0];
    float* input1 = AInputs[1];
    float* output0 = AOutputs[0];
    float* output1 = AOutputs[1];
    for (uint32_t i=0; i<ANumSamples; i++) {
      float spl0 = *input0++;
      float spl1 = *input1++;
      switch (p_type) {
        case 0: // lowpass
          spl0 = tick_lp(p_freq*0.5,p_reso,p_clip,spl0*p_pre) * p_post;
          break;
        case 1: // highpass
          spl0 = tick_hp(p_freq*0.5,p_reso,p_clip,spl0*p_pre) * p_post;
          break;
      }
      spl1 = spl0;
      *output0++ = spl0;
      *output1++ = spl1;
    }
  }


};

//----------------------------------------------------------------------
#endif

