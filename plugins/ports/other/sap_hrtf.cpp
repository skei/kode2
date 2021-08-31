
/*
  hrtf:

  MIT

  Copyright (c) 2017 Benno Straub
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#define KODE_NO_GUI

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

    //KODE_TRACE;

    MName       = "sap_hrtf";
    MAuthor     = "skei.audio";
    MVersion = 0x00000001;

    appendInput(  new KODE_PluginPort("input1")  );
    appendInput(  new KODE_PluginPort("input2")  );
    appendOutput( new KODE_PluginPort("output1") );
    appendOutput( new KODE_PluginPort("output2") );

    //appendParameter( new KODE_FloatParameter("left",   0.5f, 0.0f, 2.0f) );
    //appendParameter( new KODE_FloatParameter("right",  0.5f, 0.0f, 2.0f) );

    appendParameter( new KODE_FloatParameter( "rotation", 0,  -180,180 ));
    appendParameter( new KODE_FloatParameter( "slope",    0,  -90, 90 ));
    appendParameter( new KODE_FloatParameter( "distance", 1,   1,  100 )); // 100^2 = 10000

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

  //float  MSampleRate = 0.0f;
  float  MRotate     = 0.0f;
  float  MSlope      = 0.0f;
  float  MDistance   = 1.0f;

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_TRACE;
  }

  virtual ~myInstance() {
    //KODE_TRACE;
  }

private:

    void update(float* left, float* right) {
      float panl = (cosf(((MRotate + 90.0f) * KODE_PI) / 180.0f) + 1.0f) * 0.5f;
      float panr = 1.0f - panl;
      float pan_intensity = 1.0f / MDistance;
      *left  = powf(panl, pan_intensity);
      *right = powf(panr, pan_intensity);
    }

public:

  void on_plugin_parameter(uint32_t AOffset, uint32_t AIndex, float AValue, uint32_t AMode=0) final {
    switch(AIndex) {
      case 0: MRotate = AValue;           break;
      case 1: MSlope = AValue;            break;
      case 2: MDistance = AValue*AValue;  break;
    }
  }

  void on_plugin_process(KODE_ProcessContext* AContext) final {
    uint32_t len = AContext->numsamples;
    float* in0 = AContext->inputs[0];
    float* in1 = AContext->inputs[1];
    float* out0 = AContext->outputs[0];
    float* out1 = AContext->outputs[1];

    float left, right;
    update(&left,&right);

    for (uint32_t i=0; i<len; i++) {
      *out0++ = *in0++ * left;
      *out1++ = *in1++ * right;
    }
  }

  //KODE_Editor* on_openEditor(void* AParent) final { return KODE_NULL; }
  //void on_closeEditor(KODE_Editor* AEditor) final {}
  //void on_updateEditor(KODE_Editor* AEditor) final {}

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
