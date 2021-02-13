

#if 0

#include "core/kode.h"
#include "core/kode_rect.h"
#include "extern/kode_ladspa.h"
#include "extern/kode_dssi.h"
#include "plugin/base/kode_base_plugin.h"
#include "plugin/base/kode_base_instance.h"
#include "plugin/dssi/kode_dssi_utils.h"

//----------------------------------------------------------------------

class KODE_DssiInstance
: public KODE_BaseInstance {

  friend void  dssi_connect_port_callback(LADSPA_Handle Instance, unsigned long Port, LADSPA_Data* DataLocation);
  friend void  dssi_activate_callback(LADSPA_Handle Instance);
  friend void  dssi_run_callback(LADSPA_Handle Instance, unsigned long SampleCount);
  //friend void  dssi_run_adding_callback(LADSPA_Handle Instance, unsigned long SampleCount);
  //friend void  dssi_set_run_adding_gain_callback(LADSPA_Handle Instance, LADSPA_Data Gain);
  friend void  dssi_deactivate_callback(LADSPA_Handle Instance);
  friend void  dssi_cleanup_callback(LADSPA_Handle Instance);

  friend char* dssi_configure(LADSPA_Handle Instance, const char* Key, const char* Value);
  friend const DSSI_Program_Descriptor* dssi_get_program(LADSPA_Handle Instance, unsigned long Index);
  friend void  dssi_select_program(LADSPA_Handle Instance, unsigned long Bank, unsigned long Program);
  friend int   dssi_get_midi_controller_for_port(LADSPA_Handle Instance, unsigned long Port);
  friend void  dssi_run_synth(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t* Events, unsigned long EventCount);
  //friend void  dssi_run_synth_adding(LADSPA_Handle Instance, unsigned long SampleCount, snd_seq_event_t *Events, unsigned long EventCount);
  //friend void  dssi_run_multiple_synths(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts);
  //friend void  dssi_run_multiple_synths_adding(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts);

  //------------------------------
  //
  //------------------------------

  private:

    KODE_BasePlugin*        MPlugin         = nullptr;

    float**                 MInputPtrs      = nullptr;
    float**                 MOutputPtrs     = nullptr;
    float**                 MParameterPtrs  = nullptr;

    DSSI_Program_Descriptor MProgramDescriptor  = {0};
    KODE_uint32             MCurrentBank        = 0;
    KODE_uint32             MCurrentProgram     = 0;

  protected:

    //KODE_Instance*  MInstance       = nullptr;
    KODE_uint32     MNumInputs      = 0;
    KODE_uint32     MNumOutputs     = 0;
    KODE_uint32     MNumParameters  = 0;
    float*          MHostValues     = nullptr;
    float*          MProcessValues  = nullptr;
    float           MSampleRate     = 0.0f;

    KODE_Rect       MEditorRect = KODE_Rect(0);

  //------------------------------
  //
  //------------------------------

  public:

    KODE_DssiInstance(KODE_BasePlugin* APlugin, void* AHost)
    : KODE_BaseInstance(APlugin,AHost) {

      KODE_TRACE;
      MPlugin         = APlugin;
      //MInstance       = AInstance;
      MNumInputs      = APlugin->getNumInputs();
      MNumOutputs     = APlugin->getNumOutputs();
      MNumParameters  = APlugin->getNumParameters();
      MInputPtrs      = (float**)KODE_Malloc(MNumInputs     * sizeof(float*));
      MOutputPtrs     = (float**)KODE_Malloc(MNumOutputs    * sizeof(float*));
      MParameterPtrs  = (float**)KODE_Malloc(MNumParameters * sizeof(float*));
      MHostValues     = (float*) KODE_Malloc(MNumParameters * sizeof(float ));
      MProcessValues  = (float*) KODE_Malloc(MNumParameters * sizeof(float ));
    }

    //----------

    virtual ~KODE_DssiInstance() {
      KODE_TRACE;
      if (MInputPtrs)     KODE_Free(MInputPtrs);
      if (MOutputPtrs)    KODE_Free(MOutputPtrs);
      if (MParameterPtrs) KODE_Free(MParameterPtrs);
      if (MHostValues)    KODE_Free(MHostValues);
      if (MProcessValues) KODE_Free(MProcessValues);
      //if (MInstance)      KODE_Delete MInstance;
    }

  //------------------------------
  //
  //------------------------------

  public:

    void setSampleRate(float ASampleRate) {
      MSampleRate = ASampleRate;
    }

  //------------------------------
  //
  //------------------------------

  private:

    void updateParametersInProcess(void) {
      for (KODE_uint32 i=0; i<MNumParameters; i++) {
        float v = *MParameterPtrs[i];
        MHostValues[i] = v;
        if (v != MProcessValues[i]) {
          MProcessValues[i] = v;
          //KODE_Parameter* param = MPlugin->getParameter(i);
          //if (param) v = param->from01(v);
          on_parameterChange(i,v,kpc_process);
        }
      }
    }

    //----------

    void updateMidiInputInProcess(snd_seq_event_t* Events, unsigned long EventCount) {
      KODE_uint32 event_index = 0;
      while ((event_index < EventCount)) {
        snd_seq_event_t* event = &Events[event_index];
        KODE_uint32 ofs = event->time.tick;
        KODE_uint32 chn = 0;
        KODE_uint32 idx = 0;
        KODE_uint32 val = 0;
        switch (event->type) {
          case SND_SEQ_EVENT_NOTEOFF:
            // note off
            chn = event->data.note.channel;
            idx = event->data.note.note;
            val = event->data.note.velocity;
            on_midiInput(ofs,0x80+chn,idx,val);
            break;
          case SND_SEQ_EVENT_NOTEON:
            // note on
            chn = event->data.note.channel;
            idx = event->data.note.note;
            val = event->data.note.velocity;
            if (val > 0) on_midiInput(ofs,0x90+chn,idx,val);
            else on_midiInput(ofs,0x80+chn,idx,val);
            break;
          case SND_SEQ_EVENT_KEYPRESS:
            // polyphonic key pressure (aftertouch)
            chn = event->data.note.channel;
            idx = event->data.note.note;
            val = event->data.note.velocity;
            on_midiInput(ofs,0xA0+chn,idx,val);
            break;
          case SND_SEQ_EVENT_CONTROLLER:
            //control_change
            chn = event->data.control.channel;
            idx = event->data.control.param;
            val = event->data.control.value;
            on_midiInput(ofs,0xB0+chn,idx,val);
            break;
          //case // 0xC0 = program change
          //  break;
          case SND_SEQ_EVENT_CHANPRESS:
            //channel_pressure;
            chn = event->data.control.channel;
            idx = 0;
            val = event->data.control.value;
            on_midiInput(ofs,0xD0+chn,0,val);
            break;
          case SND_SEQ_EVENT_PITCHBEND:
            //pitch_bend
            chn = event->data.control.channel;
            idx = 0;
            val = event->data.control.value;
            on_midiInput(ofs,0xE0+chn,0,val);
            break;
        }
        event_index++;
      }
    }

  //------------------------------
  // ladspa
  //------------------------------

  //public:
  private:

    void dssi_connect_port(unsigned long Port, LADSPA_Data* DataLocation) {
      //DSSI_Trace("dssi: connect_port Port:%i DataLocation:%p\n",Port,DataLocation);
      if (Port < MNumInputs) {
        MInputPtrs[Port] = (float*)DataLocation;
        return;
      }
      Port -= MNumInputs;
      if (Port < MNumOutputs) {
        MOutputPtrs[Port] = (float*)DataLocation;
        return;
      }
      Port -= MNumOutputs;
      if (Port < MNumParameters) {
        MParameterPtrs[Port] = (float*)DataLocation;
        return;
      }
      Port -= MNumParameters;
    }

    //----------

    void dssi_activate() {
      DSSI_Trace("dssi: activate\n");
      //MSampleRate = MPlugin->getSampleRate();
      on_stateChange(kps2_activate);
    }

    //----------

    void dssi_run(unsigned long SampleCount) {
      //DSSI_Trace("dssi: run SampleCount:%i\n",SampleCount);
      updateParametersInProcess();
      on_processBlock(MInputPtrs,MOutputPtrs,SampleCount);
    }

    //----------

    //void dssi_run_adding(unsigned long SampleCount) {
    //  //DSSI_Trace("dssi: run_adding SampleCount:%i\n",SampleCount);
    //}

    //----------

    //void dssi_set_run_adding_gain(LADSPA_Data Gain) {
    //  //DSSI_Trace("dssi: set_run_adding_gain Gain:%.3f\n",Gain);
    //}

    //----------

    void dssi_deactivate() {
      DSSI_Trace("dssi: deactivate\n");
      on_stateChange(kps2_deactivate);
    }

    //----------

    void dssi_cleanup() {
      DSSI_Trace("dssi: cleanup\n");
      on_stateChange(kps2_terminate);
      on_stateChange(kps2_close);
    }

  //------------------------------
  // dssi
  //------------------------------

  //public:
  private:

    char* dssi_configure(const char* Key, const char* Value) {
      DSSI_Trace("dssi: configure Key:%s Value:%s\n",Key,Value);
      return nullptr;
    }

    /*
      This function returns a DSSI_Program_Descriptor pointer that is
      guaranteed to be valid only until the next call to get_program,
      deactivate, or configure, on the same plugin instance.  This
      function must return NULL if passed an Index argument out of
      range, so that the host can use it to query the number of
      programs as well as their properties.
    */

    const DSSI_Program_Descriptor* dssi_get_program(unsigned long Index) {
      DSSI_Trace("dssi: get_program Index:%i\n",Index);
      MProgramDescriptor.Bank = 0;
      MProgramDescriptor.Program = 0;
      MProgramDescriptor.Name = "name";
      return nullptr;
    }

    void dssi_select_program(unsigned long Bank, unsigned long Program) {
      DSSI_Trace("dssi: select_program Bank:%i Program:%i\n",Bank,Program);
      MCurrentBank = Bank;
      MCurrentProgram = Program;
    }

    int dssi_get_midi_controller_for_port(unsigned long Port) {
      DSSI_Trace("dssi: dssi_get_midi_controller_for_port Port:%i\n",Port);
      return 0;
    }

    //----------

    void dssi_run_synth(unsigned long SampleCount, snd_seq_event_t* Events, unsigned long EventCount) {
      //DSSI_Trace("dssi: run_synth SampleCount:%i EventCount:%i\n",SampleCount,EventCount);
      updateParametersInProcess();
      updateMidiInputInProcess(Events,EventCount);
      dssi_run(SampleCount);
    }

    //void dssi_run_synth_adding(unsigned long SampleCount, snd_seq_event_t *Events, unsigned long EventCount) {
    //  //DSSI_Trace("dssi: run_synth_adding SampleCount:%i EventCount:%i\n",SampleCount,EventCount);
    //}

    //void dssi_run_multiple_synths(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts) {
    //}

    //void dssi_run_multiple_synths_adding(unsigned long InstanceCount, LADSPA_Handle *Instances, unsigned long SampleCount, snd_seq_event_t **Events, unsigned long *EventCounts) {
    //}


};


#endif // 0
