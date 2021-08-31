
#define KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

/*
  based on:
    ladspa-vocoder by Achim Settelmeier (settel-linux@sirlab.de)
    https://www.t2-project.org/packages/ladspa-vocoder.html
    (gpl3)
    see bottom of this file for more info
*/

/*
  vocoder.c
  Version 0.3
  LADSPA Unique ID: 1441
  Version 0.3
  Added support for changing bands in real time 2003-12-09
  Version 0.2
  Adapted to LADSPA by Josh Green <jgreen@users.sourceforge.net>
  15.6.2001 (for the LinuxTag 2001!)
  Original program can be found at:
  http://www.sirlab.de/linux/
  Author: Achim Settelmeier <settel-linux@sirlab.de>
*/

//#define AMPLIFIER 16.0
#define MAX_BANDS     16
#define PRE_CARRIER   1
#define PRE_FORMANT   1
#define POST_CARRIER  2
#define POST_FORMANT  2

//#undef CLAMP
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

struct _bandpass {
  float c, f, att;
  float freq;
  float low1, low2;
  float mid1, mid2;
  float high1, high2;
  float y;
};

struct _bands_out {
  float decay;
  float oldval;
  float level;		/* 0.0 - 1.0 level of this output band */
};

const float _decay_table[] = {
  1/100.0,
  1/100.0, 1/100.0, 1/100.0,
  1/125.0, 1/125.0, 1/125.0,
  1/166.0, 1/166.0, 1/166.0,
  1/200.0, 1/200.0, 1/200.0,
  1/250.0, 1/250.0, 1/250.0
};

const char* _band_names[] = {
  "band 1", "band 2", "band 3", "band 4", "band 5", "band 6", "band 7", "band 8",
  "band 9", "band 10","band 11","band 12","band 13","band 14","band 15","band 16",
  "band 17","band 18","band 19","band 20","band 21","band 22","band 23","band 24",
  "band 25","band 26","band 27","band 28","band 29","band 30","band 31","band 32"
};

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    //KODE_TRACE;

    MName       = "sap_vocoder";
    MAuthor     = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    //appendParameter( new KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( new KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );

      appendParameter( new KODE_FloatParameter( "main vol", 1,  0, 2 ));
      appendParameter( new KODE_IntParameter( "num bands", 4, 1, MAX_BANDS ));
      for (int32_t i=0; i<MAX_BANDS; i++) {
        appendParameter( new KODE_Parameter( _band_names[i]/*"band"*/, 1 ));
      }

  }

  //----------

  virtual ~myDescriptor() {
    //KODE_TRACE;
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  //float MLeft   = 0.0f;
  //float MRight  = 0.0f;

  int32_t     num_bands;		              // current number of bands
  float       mainvol;		                // main volume
  _bandpass   bands_formant[MAX_BANDS];   // one structure per band
  _bandpass   bands_carrier[MAX_BANDS];   // one structure per band
  _bands_out  bands_out[MAX_BANDS];       // one structure per band
  int32_t     par_BandCount;	            // Band count control
  float       par_BandLevels[MAX_BANDS];  // level controls for each band
  float       par_vol;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
      num_bands = -1;
      mainvol   = 1.0f;
      memset(bands_formant,0,sizeof(bands_formant));
      memset(bands_carrier,0,sizeof(bands_carrier));
      memset(bands_out,0,sizeof(bands_out));
  }

  virtual ~myInstance() {
    //KODE_TRACE;
  }

  private:

    void do_bandpasses(_bandpass* bands, float sample) {
      for (int32_t i=0; i < num_bands; i++) {
        bands[i].high1 = sample - bands[i].f * bands[i].mid1 - bands[i].low1;
        bands[i].mid1 += bands[i].high1 * bands[i].c;
        bands[i].low1 += bands[i].mid1;
        bands[i].high2 = bands[i].low1 - bands[i].f * bands[i].mid2 - bands[i].low2;
        bands[i].mid2 += bands[i].high2 * bands[i].c;
        bands[i].low2 += bands[i].mid2;
        bands[i].y = bands[i].high2 * bands[i].att;
      }
    }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch(AIndex) {
      case 0:
        //par_vol = AValue * AValue;
        par_vol = AValue * AValue;
        break;
      case 1:
        par_BandCount = AValue;
        break;
      default:
        AIndex -= 2;
        if (AIndex < MAX_BANDS) {
          par_BandLevels[AIndex] = AValue;
        }
        break;
    }
  }

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    uint32_t len = AContext->numsamples;
    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];

      mainvol = par_vol;

      // prepare

      int32_t nb = par_BandCount;
      if ((nb < 1) || (nb > MAX_BANDS)) nb = MAX_BANDS;
      // initialize bandpass information if num_bands control has changed, or on first run
      if (nb != num_bands) {
        num_bands = nb;
        //STrace("num_bands %i\n",num_bands);
        for (int32_t i=0; i<nb; i++) {
          memset(&bands_formant[i], 0, sizeof(_bandpass));
          // stretch existing bands

          /*
            skei:
            where does all these constants come from?
          */

          float a = 16.0 * i/(float)nb;
          if (a < 4.0) bands_formant[i].freq = 150 + 420 * a / 4.0;
          else bands_formant[i].freq = 600 * pow (1.23, a - 4.0);

          float c = bands_formant[i].freq * KODE_PI2 / AContext->samplerate;
          bands_formant[i].c = c * c;
          bands_formant[i].f = 0.4/c;
          bands_formant[i].att = 1/(6.0 + ((exp (bands_formant[i].freq / AContext->samplerate) - 1) * 10));
          memcpy(&bands_carrier[i], &bands_formant[i], sizeof(_bandpass));
          bands_out[i].decay = _decay_table[(int32_t)a];
          bands_out[i].level = CLAMP(par_BandLevels[i], 0.0, 1.0);
        }
      }
      else {
        for (int32_t i=0; i<nb; i++)
          bands_out[i].level = CLAMP(par_BandLevels[i], 0.0, 1.0);
      }

      // process


    for (uint32_t i=0; i<len; i++) {
      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;

        float spl0 = *input0++;
        float spl1 = *input1++;

        spl0 *= PRE_CARRIER;
        spl1 *= PRE_FORMANT;

        do_bandpasses(bands_carrier,spl0);
        do_bandpasses(bands_formant,spl1);
        spl0 = 0.0;
        for (int32_t j=0; j < num_bands /*nb*/; j++) {
          bands_out[j].oldval = bands_out[j].oldval
                              + (fabs (bands_formant[j].y)
                              - bands_out[j].oldval)
                              * bands_out[j].decay;
          float x = bands_carrier[j].y * bands_out[j].oldval;
          spl0 += x * bands_out[j].level;
        }

        spl0 *= POST_CARRIER;
        spl1 *= POST_FORMANT;

        spl0 *= mainvol;
        *output0++ = spl0;
        *output1++ = spl0;


    }
  }

  //KODE_Editor* on_openEditor(void* AParent) final { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) final {}
  //void on_updateEditor(KODE_Editor* AEditor) final {}

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
