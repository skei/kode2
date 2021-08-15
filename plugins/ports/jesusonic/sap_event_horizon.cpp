
#define KODE_NO_GUI

#include "kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_parameters.h"
#include "plugin/kode_plugin.h"

//----------------------------------------------------------------------

/*
  based on:
    event horizon (peak-eating limiter) jesusonic plugin
    Copyright 2006, Thomas Scott Stillwell
    see bottom of this file for original and license info
*/

#define _log2db (20.0f / logf(10.0f))
#define _db2log (logf(10.0f) / 20.0f)

//----------------------------------------------------------------------

class myDescriptor : public KODE_Descriptor {

public:

  myDescriptor()
  : KODE_Descriptor() {

    //KODE_TRACE;

    MName       = "sap_event_horizon";
    MAuthor     = "skei.audio";
    MVersion    = 0x00000001;

    appendInput(  KODE_New KODE_PluginPort("input1")  );
    appendInput(  KODE_New KODE_PluginPort("input2")  );
    appendOutput( KODE_New KODE_PluginPort("output1") );
    appendOutput( KODE_New KODE_PluginPort("output2") );

    //appendParameter( KODE_New KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( KODE_New KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );

      appendParameter( KODE_New KODE_FloatParameter("threshold",   0,   -30, 0 ));
      appendParameter( KODE_New KODE_FloatParameter("ceiling",    -0.1, -20, 0 ) );
      appendParameter( KODE_New KODE_FloatParameter("soft clip",   2,    0,  6 ) );
      //appendParameter( KODE_New KODE_FloatParameter("soft clip ratio","",   10.0,   3.0, 20.0, 0.1  ) );

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

    //float _log2db, _db2log;
    float slider1, slider2, slider3, slider4;
    float thresh, threshdb;
    float ceiling, ceildb, makeupdb;
    float makeup;
    float sc, scv, sccomp, peakdb, peaklvl;
    float scratio, scmult;

    bool need_recalc = true;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
    slider1   = 0.0f;
    slider2   = 0.0f;
    slider3   = 0.0f;
    slider4   = 0.0f; //10.0;
    thresh    = 0.0f;
    threshdb  = 0.0f;
    ceiling   = 0.0f;
    ceildb    = 0.0f;
    makeupdb  = 0.0f;
    makeup    = 0.0f;
    sc        = 0.0f;
    scv       = 0.0f;
    sccomp    = 0.0f;
    peakdb    = 0.0f;
    peaklvl   = 0.0f;
    scratio   = 0.0f;
    scmult    = 0.0f;
  }

  virtual ~myInstance() {
    //KODE_TRACE;
  }

private:

  void recalc(void) {
    thresh    = expf(slider1 * _db2log);
    threshdb  = slider1;
    ceiling   = expf(slider2 * _db2log);
    ceildb    = slider2;
    makeup    = expf((ceildb-threshdb) * _db2log);
    makeupdb  = ceildb - threshdb;
    sc        = -slider3;
    scv       = expf( sc * _db2log);
    sccomp    = expf(-sc * _db2log);
    peakdb    = ceildb + 25.0f;
    peaklvl   = expf(peakdb * _db2log);
    scratio   = slider4;
    scmult    = fabs((ceildb - sc) / (peakdb - sc));
  }

public:

  //void on_open()        final { KODE_TRACE; }
  //void on_close()       final { KODE_TRACE; }
  //void on_initialize()  final { KODE_TRACE; }
  //void on_terminate()   final { KODE_TRACE; }
  //void on_activate()    final { KODE_TRACE; }
  //void on_deactivate()  final { KODE_TRACE; }
  //void on_startPreset() final { KODE_TRACE; }
  //void on_endPreset()   final { KODE_TRACE; }

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch (AIndex) {
      case 0: slider1 = AValue; break;
      case 1: slider2 = AValue; break;
      case 2: slider3 = AValue; break;
      //case 3: slider4 = AValue; break;
    }
    //recalc();
    need_recalc = true;
  }

  //void on_midiInput(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}

  void on_plugin_process(KODE_ProcessContext* AContext) final {

    if (need_recalc) {
      need_recalc = false;
      recalc();
    }

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

      //float peak = SMax(abs(spl0),abs(spl1));
      spl0 *= makeup;
      spl1 *= makeup;

      float sign0 = KODE_Sign(spl0);
      float sign1 = KODE_Sign(spl1);
      float abs0 = fabs(spl0);
      float abs1 = fabs(spl1);

      float overdb0 = 2.08136898 * logf(abs0) * _log2db - ceildb; // c++ Log = pascal Ln ?????
      float overdb1 = 2.08136898 * logf(abs1) * _log2db - ceildb;

      if (abs0 > scv) spl0 = sign0 * (scv + expf(overdb0*scmult)*_db2log);
      if (abs1 > scv) spl1 = sign1 * (scv + expf(overdb1*scmult)*_db2log);

      spl0 = KODE_Min(ceiling,fabs(spl0)) * KODE_Sign(spl0);
      spl1 = KODE_Min(ceiling,fabs(spl1)) * KODE_Sign(spl1);

      *out0++ = spl0;
      *out1++ = spl1;


    }
  }

  //KODE_Editor* on_openEditor(void* AParent) final { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) final {}
  //void on_updateEditor(KODE_Editor* AEditor) final {}

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)









/*

// Copyright 2006, Thomas Scott Stillwell
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
desc:peak-eating limiter

slider1:0.0<-30.0,0.0,0.1>Threshold
slider2:-0.1<-20.0,0.0,0.1>Ceiling
slider3:2.0<0,6.0,0.01>Soft clip (dB)
//slider4:10<3,20,0.1>Soft clip ratio

@init
  pi = 3.1415926535;
  log2db = 8.6858896380650365530225783783321; // 20 / ln(10)
  db2log = 0.11512925464970228420089957273422; // ln(10) / 20

@slider
  thresh=exp(slider1 * db2log);
  threshdb=slider1;
  ceiling=exp(slider2 * db2log);
  ceildb=slider2;
  makeup=exp((ceildb-threshdb) * db2log);
  makeupdb=ceildb-threshdb;
  sc = -slider3;
  scv = exp(sc * db2log);
  sccomp = exp(-sc * db2log);
  peakdb = ceildb+25;
  peaklvl = exp(peakdb * db2log);
  scratio = slider4;
  scmult = abs((ceildb - sc) / (peakdb - sc));

@sample
  peak=max(abs(spl0),abs(spl1));
  spl0=spl0*makeup;
  spl1=spl1*makeup;
  sign0 = sign(spl0);
  sign1 = sign(spl1);
  abs0=abs(spl0);
  abs1=abs(spl1);
  overdb0 = 2.08136898 * log(abs0) * log2db - ceildb;
  overdb1 = 2.08136898 * log(abs1) * log2db - ceildb;
  abs0 > scv ? (
    spl0=sign0*(scv + exp(overdb0*scmult)*db2log);
  );
  abs1 > scv ? (
    spl1=sign1*(scv + exp(overdb1*scmult)*db2log);
  );
  spl0=min(ceiling,abs(spl0))*sign(spl0);
  spl1=min(ceiling,abs(spl1))*sign(spl1);

*/

