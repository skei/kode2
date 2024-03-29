
//#define KODE_DEBUG
//#define KODE_DEBUG_VST2

//#define KODE_PLUGIN_DSSI
//#define KODE_PLUGIN_EXE
//#define KODE_PLUGIN_LADSPA
//#define KODE_PLUGIN_LV2
//#define KODE_PLUGIN_VST2
//#define KODE_PLUGIN_VST3

#define KODE_NO_GUI


//----------------------------------------------------------------------

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    MName = "sap_exciter2";
    MAuthor = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    /*
    slider1:0<0,100,0.1>Mix (%)
    slider2:0<0,100,0.1>Drive (%)
    slider3:5000<2000,10000,100>Frequency (Hz)
    */

    appendParameter( new KODE_FloatParameter("mix",    0.0f,     0.0f,     100.0f ) );
    appendParameter( new KODE_FloatParameter("drive",  0.0f,     0.0f,     100.0f ) );
    appendParameter( new KODE_FloatParameter("freq",   5000.0f,  2000.0f,  10000.0f ) );
  }

  //----------

  virtual ~myDescriptor() {
  }

};

//----------------------------------------------------------------------

class myInstance : public KODE_Instance {

private:

  bool need_recalc = false;

  float slider1 = 0.0f;
  float slider2 = 0.0f;
  float slider3 = 0.0f;

  float mix;
  float drive;
  float mix1;
  float drive1;
  float drive2;
  float freq;

  // init
  float gain,  freq1, a1, s1, q1, w01, cosw01, sinw01, alpha1;
  float gain1;
  float gain3, freq3, a3, s3, q3, w03, cosw03, sinw03, alpha3;
  float b01, b11, b21, a01, a11, a21;
  float b03, b13, b23, a03, a13, a23;

  float xl21, xl11, yl21, yl11;
  float xr21, xr11, yr21, yr11;

  float xl23, xl13, yl23, yl13;
  float xr23, xr13, yr23, yr13;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
  }

  virtual ~myInstance() {
  }

public:

  /*
  void init(float srate) {

    gain1   = 0;
    freq1   = freq;
    a1      = 1;
    s1      = 1;
    q1      = 1 / (sqrt((a1 + 1/a1)*(1/s1 - 1) + 2));
    w01     = 2 * KODE_PI * freq1/srate;
    cosw01  = cos(w01);
    sinw01  = sin(w01);
    alpha1  = sinw01 / (2 * q1);

    b01     = (1 + cosw01)/2;
    b11     = -(1 + cosw01);
    b21     = (1 + cosw01)/2;
    a01     = 1 + alpha1;
    a11     = -2 * cosw01;
    a21     = 1 - alpha1;
    b01     /= a01;
    b11     /= a01;
    b21     /= a01;
    a11     /= a01;
    a21     /= a01;

    gain3   = 0;
    freq3   = freq * 2;
    a3      = powf(10, (gain3/40) );;
    s3      = 1;
    q3      = 1 / (sqrt((a3 + 1/a3)*(1/s3 - 1) + 2));
    w03     = 2 * KODE_PI * freq3/srate;
    cosw03  = cos(w03);
    sinw03  = sin(w03);
    alpha3  = sinw03 / (2 * q3);

    b03     = (1 + cosw03)/2;
    b13     = -(1 + cosw03);
    b23     = (1 + cosw03)/2;
    a03     = 1 + alpha3;
    a13     = -2 * cosw03;
    a23     = 1 - alpha3;
    b03     /= a03;
    b13     /= a03;
    b23     /= a03;
    a13     /= a03;
    a23     /= a03;
  }
  */

  void recalc(float srate) {

    need_recalc = false;

    mix     = (10 * slider1) / 100;
    drive   = slider2 / 100;
    mix1    = 1 - mix;
    drive1  = 1 / (1 - (drive / 2));
    drive2  = drive / 2;
    freq    = slider3;

    gain1   = 0;
    freq1   = freq;
    a1      = 1;
    s1      = 1;
    q1      = 1 / (sqrt((a1 + 1/a1)*(1/s1 - 1) + 2));
    w01     = 2 * KODE_PI * freq1/srate;
    cosw01  = cos(w01);
    sinw01  = sin(w01);
    alpha1  = sinw01 / (2 * q1);

    b01     = (1 - cosw01)/2;
    b11     = (1 - cosw01);
    b21     = (1 - cosw01)/2;
    a01     = 1 + alpha1;
    a11     = -2 * cosw01;
    a21     = 1 - alpha1;
    b01     /= a01;
    b11     /= a01;
    b21     /= a01;
    a11     /= a01;
    a21     /= a01;

    gain3   = 0;
    freq3   = freq * 2;
    a3      = powf(10,(gain3/40));
    s3      = 1;
    q3      = 1 / (sqrt((a3 + 1/a3)*(1/s3 - 1) + 2));
    w03     = 2 * KODE_PI * freq3/srate;
    cosw03  = cos(w03);
    sinw03  = sin(w03);
    alpha3  = sinw03 / (2 * q3);

    b03     = (1 + cosw03)/2;
    b13     = -(1 + cosw03);
    b23     = (1 + cosw03)/2;
    a03     = 1 + alpha3;
    a13     = -2 * cosw03;
    a23     = 1 - alpha3;
    b03     /= a03;
    b13     /= a03;
    b23     /= a03;
    a13     /= a03;
    a23     /= a03;

  }


public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch(AIndex) {
      case 0: slider1 = AValue; break;
      case 1: slider2 = AValue; break;
      case 2: slider3 = AValue; break;
    }
    need_recalc = true;
  }

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    if (need_recalc) recalc(AContext->samplerate);

    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];
    for (uint32_t i=0; i<len; i++) {

      //*out0++ = *in0++ * MLeft;
      //*out1++ = *in1++ * MRight;

      float spl0 = *in0++;
      float spl1 = *in1++;

      float dry0 = spl0;
      float dry1 = spl1;

      //hpf to extract HF signal
      float ospl0 = dry0;
      dry0 = b01 * dry0 + b11 * xl11 + b21 * xl21 - a11 * yl11 - a21 * yl21;
      xl21 = xl11;
      xl11 = ospl0;
      yl21 = yl11;
      yl11 = dry0;

      float ospl1 = dry1;
      dry1 = b01 * dry1 + b11 * xr11 + b21 * xr21 - a11 * yr11 - a21 * yr21;
      xr21 = xr11;
      xr11 = ospl1;
      yr21 = yr11;
      yr11 = dry1;

      //enhance HF signal
      float wet0 = drive1 * (1.5 * dry0) * (1 - abs((1.5 * dry0) * drive2));
      float wet1 = drive1 * (1.5 * dry1) * (1 - abs((1.5 * dry1) * drive2));
      wet0 /= 1.5;
      wet1 /= 1.5;

      //hpf to isolate enhance signal
      ospl0 = wet0;
      wet0 = b03 * wet0 + b13 * xl13 + b23 * xl23 - a13 * yl13 - a23 * yl23;
      xl23 = xl13;
      xl13 = ospl0;
      yl23 = yl13;
      yl13 = wet0;

      ospl1 = wet1;
      wet1 = b03 * wet1 + b13 * xr13 + b23 * xr23 - a13 * yr13 - a23 * yr23;
      xr23 = xr13;
      xr13 = ospl1;
      yr23 = yr13;
      yr13 = wet1;


      spl0 = spl0 + (mix) * wet0;
      spl1 = spl1 + (mix) * wet1;

      *out0++ = spl0;
      *out1++ = spl1;

    }
  }

  //KODE_Editor* on_openEditor(void* AParent) { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) {}
  //void on_updateEditor(KODE_Editor* AEditor) {}

};



//----------------------------------------------------------------------

//KODE_EXE_MAIN(KODE_Descriptor,KODE_Instance);
//KODE_VST3_MAIN(KODE_Descriptor,KODE_Instance);

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)



//----------------------------------------------------------------------
#if 0

// Copyright 2007, Thomas Scott Stillwell
// All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, are permitted
//provided that the following conditions are met:
//
//Redistributions of source code must retain the above copyright notice, this list of conditions
//and the following disclaimer.
//
//Redistributions in binary form must reproduce the above copyright notice, this list of conditions
//and the following disclaimer in the documentation and/or other materials provided with the distribution.
//
//The name of Thomas Scott Stillwell may not be used to endorse or
//promote products derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
//IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
//BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
//STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
//THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

desc:Exciter (Treble Enhancer)
desc:Exciter (Treble Enhancer) [Stillwell]
//tags: processing exciter distortion
//author: Stillwell

slider1:0<0,100,0.1>Mix (%)
slider2:0<0,100,0.1>Drive (%)
slider3:5000<2000,10000,100>Frequency (Hz)


in_pin:left input
in_pin:right input
out_pin:left output
out_pin:right output

@init

  gain1 = 0;
  freq1 = freq;
  a1 = 1;
  s1 = 1;
  q1 = 1 / (sqrt((a1 + 1/a1)*(1/s1 - 1) + 2));
  w01 = 2 * $pi * freq1/srate;
  cosw01 = cos(w01);
  sinw01 = sin(w01);
  alpha1 = sinw01 / (2 * q1);

  b01 = (1 + cosw01)/2;
  b11 = -(1 + cosw01);
  b21 = (1 + cosw01)/2;
  a01 = 1 + alpha1;
  a11 = -2 * cosw01;
  a21 = 1 - alpha1;
  b01 /= a01;
  b11 /= a01;
  b21 /= a01;
  a11 /= a01;
  a21 /= a01;

  gain3 = 0;
  freq3 = freq * 2;
  a3 = 10^(gain3/40);
  s3 = 1;
  q3 = 1 / (sqrt((a3 + 1/a3)*(1/s3 - 1) + 2));
  w03 = 2 * $pi * freq3/srate;
  cosw03 = cos(w03);
  sinw03 = sin(w03);
  alpha3 = sinw03 / (2 * q3);

  b03 = (1 + cosw03)/2;
  b13 = -(1 + cosw03);
  b23 = (1 + cosw03)/2;
  a03 = 1 + alpha3;
  a13 = -2 * cosw03;
  a23 = 1 - alpha3;
  b03 /= a03;
  b13 /= a03;
  b23 /= a03;
  a13 /= a03;
  a23 /= a03;

@slider

  mix=(10 * slider1) / 100;
  drive=slider2 / 100;
  mix1 = 1 - mix;
  drive1 = 1 / (1 - (drive / 2));
  drive2 = drive / 2;
  freq = slider3;

  gain1 = 0;
  freq1 = freq;
  a1 = 1;
  s1 = 1;
  q1 = 1 / (sqrt((a1 + 1/a1)*(1/s1 - 1) + 2));
  w01 = 2 * $pi * freq1/srate;
  cosw01 = cos(w01);
  sinw01 = sin(w01);
  alpha1 = sinw01 / (2 * q1);

  b01 = (1 - cosw01)/2;
  b11 = (1 - cosw01);
  b21 = (1 - cosw01)/2;
  a01 = 1 + alpha1;
  a11 = -2 * cosw01;
  a21 = 1 - alpha1;
  b01 /= a01;
  b11 /= a01;
  b21 /= a01;
  a11 /= a01;
  a21 /= a01;

  gain3 = 0;
  freq3 = freq * 2;
  a3 = 10^(gain3/40);
  s3 = 1;
  q3 = 1 / (sqrt((a3 + 1/a3)*(1/s3 - 1) + 2));
  w03 = 2 * $pi * freq3/srate;
  cosw03 = cos(w03);
  sinw03 = sin(w03);
  alpha3 = sinw03 / (2 * q3);

  b03 = (1 + cosw03)/2;
  b13 = -(1 + cosw03);
  b23 = (1 + cosw03)/2;
  a03 = 1 + alpha3;
  a13 = -2 * cosw03;
  a23 = 1 - alpha3;
  b03 /= a03;
  b13 /= a03;
  b23 /= a03;
  a13 /= a03;
  a23 /= a03;


@sample

  dry0 = spl0;
  dry1 = spl1;

  //hpf to extract HF signal
  ospl0 = dry0;
  dry0 = b01 * dry0 + b11 * xl11 + b21 * xl21 - a11 * yl11 - a21 * yl21;
  xl21 = xl11;
  xl11 = ospl0;
  yl21 = yl11;
  yl11 = dry0;

  ospl1 = dry1;
  dry1 = b01 * dry1 + b11 * xr11 + b21 * xr21 - a11 * yr11 - a21 * yr21;
  xr21 = xr11;
  xr11 = ospl1;
  yr21 = yr11;
  yr11 = dry1;

  //enhance HF signal
  wet0 = drive1 * (1.5 * dry0) * (1 - abs((1.5 * dry0) * drive2));
  wet1 = drive1 * (1.5 * dry1) * (1 - abs((1.5 * dry1) * drive2));
  wet0 /= 1.5;
  wet1 /= 1.5;

  //hpf to isolate enhance signal
  ospl0 = wet0;
  wet0 = b03 * wet0 + b13 * xl13 + b23 * xl23 - a13 * yl13 - a23 * yl23;
  xl23 = xl13;
  xl13 = ospl0;
  yl23 = yl13;
  yl13 = wet0;

  ospl1 = wet1;
  wet1 = b03 * wet1 + b13 * xr13 + b23 * xr23 - a13 * yr13 - a23 * yr23;
  xr23 = xr13;
  xr13 = ospl1;
  yr23 = yr13;
  yr13 = wet1;


  spl0 = spl0 + (mix) * wet0;
  spl1 = spl1 + (mix) * wet1;

#endif // 0
//----------------------------------------------------------------------
