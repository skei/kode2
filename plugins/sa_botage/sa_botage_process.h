#ifndef sa_botage_process_included
#define sa_botage_process_included
//----------------------------------------------------------------------

#include "kode.h"
#include "base/utils/kode_random.h"
#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------

/*

  max buffer size:

  num_beats           8
  samples_per_second  192000
  beats_per_minute    30

  samples_per_minute = samples_per_second * 60
  samples_per_beat = samples_per_minute / beats_per_minute
  buffersize = samples_per_beat * num_beats

  srate * 60 * beats   192k * 60 * 8   92160k
  ------------------ = ------------- = ------ = 3072k -> 4mb
          bpm                 30         30

*/

// 4mb stereo
#define BUFFERSIZE (1024 * 1024 * 4 * 2)

//----------------------------------------------------------------------

// uint32_t num  = KODE_MaxI(1, rnd_bits(PRangeSliceBits) );

uint32_t randomIntFromBits(uint32_t ABits) {
  uint32_t bit_array[32];
  uint32_t count = 0;
  for (uint32_t i=0; i<32; i++) {
    if (ABits & (1 << i)) {
      bit_array[count] = i;
      count += 1;
    }
  }
  if (count == 0) return 0;
  if (count == 1) return bit_array[0] + 1;
  int32_t rnd = KODE_RandomRangeInt(0,count-1);
  int32_t result = bit_array[rnd] + 1;
  //KODE_Print("%i : %i\n",rnd,result);
  return result;
}


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myProcess {

//------------------------------
//private:
public:
//------------------------------

  bool      MIsPlaying        = false;
  bool      MWasPlaying       = false;

  uint32_t  MBlockSize        = 0;
  float     MBeatsPerSecond   = 0.0;
  float     MBeatsPerSample   = 0.0;
  float     MSamplesPerBeat   = 0.0;
  //float     MSamplesPerSlice  = 0.0;

//------------------------------
public:
//------------------------------

  float     MBuffer[BUFFERSIZE] = {0};

  float     MReadPos            = 0.0;

  float     MBufferPos          = 0.0;
  float     MBufferLength       = 0.0;
  uint32_t  MBufferNumBeats     = 0;
  uint32_t  MBufferNumSlices    = 0;
  bool      MBufferWrapped      = false;

  float     MBeatPos            = 0.0;
  float     MBeatSpeed          = 1.0;
  bool      MBeatWrapped        = false;

  float     MSlicePos           = 0.0;
  float     MSliceLength        = 0.0;
  float     MSliceSpeed         = 1.0;
  bool      MSliceWrapped       = false;
  uint32_t  MSliceCounter       = 0;

  bool      MRangeActive        = false;
  float     MRangeStart         = 0.0;
  float     MRangePos           = 0.0;
  float     MRangeLength        = 0.0;
  uint32_t  MRangeNumSlices     = 0;
  bool      MRangeWrapped       = false;

  bool      MLoopActive         = false;
  float     MLoopStart          = 0.0;
  float     MLoopPos            = 0.0;
  float     MLoopLength         = 0.0;
  float     MLoopSpeed          = 1.0;
  bool      MLoopWrapped        = false;
  uint32_t  MLoopNumDivide      = 0;
  uint32_t  MLoopCounter        = 0;

//------------------------------
public:
//------------------------------

  float     MRepeatProb          = 0.0;
  float     MRepeatProbMinSlices = 0;
  float     MRepeatProbMaxSlices = 0;
  float     MRepeatProbMinDivide = 0;
  float     MRepeatProbMaxDivide = 0;

//------------------------------
public:
//------------------------------

  myProcess() {
  }

  //----------

  ~myProcess() {
  }

//------------------------------

  void start() {
    buffer_start();
  }

//------------------------------
private:
//------------------------------

  void handle_playback_start() {
    buffer_start();
  }

  //----------

  void handle_playback_stop() {
  }

  //----------

  void handle_next_slice(uint32_t ACounter) {
    if (MRangeActive) {
    }
    else {
      range_start_prob();
    }
  }

  //----------

  void handle_next_loop(uint32_t ACounter) {
  }

//------------------------------
private:
//------------------------------

  //--------------------
  // buffer
  //--------------------

  void buffer_start() {
    MBufferPos = 0.0;
    MBufferWrapped = false;
    slice_start();
    //MSliceCounter = 0;
    //MSlicePos = 0.0;
    //MSliceSpeed = 1.0;
    //MSliceWrapped = false;
    //KODE_Print("slice %i\n",MSliceCounter);
    MRangeActive = false;
    MLoopActive = false;
    handle_next_slice(MSliceCounter);
  }

  //----------

  void buffer_wraparound() {
    MSliceCounter = 0;
    MSlicePos = 0.0;
    MSliceWrapped = false;
    //KODE_Print("slice %i\n",MSliceCounter);
    MRangeActive = false;
    MLoopActive = false;
    handle_next_slice(MSliceCounter);
  }

  //--------------------
  // slice
  //--------------------

  void slice_start() {
    MSliceCounter = 0;
    MSlicePos = 0.0;
    MSliceSpeed = 1.0;
    MSliceWrapped = false;
  }

  //----------

  void slice_wraparound() {
    MSliceCounter += 1;
    handle_next_slice(MSliceCounter);
  }

  //--------------------
  // range
  //--------------------

  void range_start_prob() {
    if (KODE_Random() < MRepeatProb) {
      uint32_t slicesleft = ((MBufferNumBeats * MBufferNumSlices) - MSliceCounter);
      uint32_t numslices = KODE_RandomRangeInt(KODE_MaxI(1,MRepeatProbMinSlices),MRepeatProbMaxSlices);
      numslices = KODE_Min(numslices,slicesleft);
      range_start(numslices);
      uint32_t numdiv = KODE_RandomRangeInt(KODE_MaxI(1,MRepeatProbMinDivide),MRepeatProbMaxDivide);
      //float multiplier = 1.0 / numdiv;
      loop_start(MBufferPos,numdiv/*multiplier*/,1.0);
    }
  }

  //----------

  void range_start(uint32_t ANumSlices) {
    MRangeActive    = true;
    MRangeStart     = MBufferPos;
    MRangePos       = 0.0;
    MRangeLength    = MSliceLength * (float)ANumSlices;
    MRangeNumSlices = ANumSlices;
    MRangeWrapped   = false;
  }

  //----------

  void range_end() {
    MRangeActive    = false;
    MRangeStart     = 0.0;
    MRangePos       = 0.0;
    MRangeLength    = 0.0;
    MRangeNumSlices = 0;
    MRangeWrapped   = false;
    loop_end();
  }

  //--------------------
  // loop
  //--------------------

  void loop_start(float AStartPos, uint32_t ANumDiv=1, float ASpeed=1.0) {
    MLoopActive     = true;
    MLoopNumDivide  = KODE_MaxI(1,ANumDiv);
    MLoopStart      = AStartPos;
    MLoopPos        = 0.0;
    float mult      = 1.0 / (float)MLoopNumDivide;
    MLoopLength     = MRangeLength * mult;
    MLoopSpeed      = ASpeed;
    MLoopWrapped    = false;
    MLoopCounter    = 0;
  }

  //----------

  void loop_wraparound() {
    MLoopCounter += 1;
    handle_next_loop(MLoopCounter);
  }

  //----------

  void loop_end() {
    MLoopActive     = false;
    MLoopNumDivide  = 0;;
    MLoopPos        = 0.0;
    MLoopLength     = 0.0;
    MLoopSpeed      = 0.0;
    MLoopStart      = 0.0;
    MLoopWrapped    = false;
    MLoopCounter    = 0;
  }

  //------------------------------
  // process
  //------------------------------

  void pre_process() {

    //if (MBufferChanged) {
    //  MBufferChanged = false;
    //  buffer_changed();
    //}

    if (MBufferWrapped) {
      MBufferWrapped = false;
      buffer_wraparound();
    }

    else { // buffer_wrapped

      if (MRangeActive) {
        if (MRangeWrapped) {
          MRangeWrapped = false;
          range_end();
        }
        else {
          if (MLoopActive) {
            if (MLoopWrapped) {
              MLoopWrapped = false;
              loop_wraparound();
            }
          } // loop_active
        }

      } // range_active

      if (MSliceWrapped) {
        MSliceWrapped = false;
        slice_wraparound();
      }

    } // buffer_wrapped

  }

  //----------

  void post_process() {

    MBufferPos  += 1.0;
    if ((MBufferPos >= MBufferLength) || (MBufferPos < 0.0)) {
      MBufferPos = KODE_Modulo(MBufferPos,MBufferLength);
      MBufferWrapped = true;
    }

    MBeatPos += MBeatSpeed;
    //if ((MBeatPos >= MBeatLength) || (MBeatPos < 0.0)) {
    //  MBeatWrapped = true;
    //  MBeatPos = KODE_Modulo(MBeatPos,MBeatLength);
    //}

    MSlicePos += MSliceSpeed;
    if ((MSlicePos >= MSliceLength) || (MSlicePos < 0.0)) {
      MSlicePos = KODE_Modulo(MSlicePos,MSliceLength);
      MSliceWrapped = true;
    }

    if (MRangeActive) {
      MRangePos += 1.0;
      if ((MRangePos >= MRangeLength) || (MRangePos < 0.0)) {
        MRangePos = KODE_Modulo(MRangePos,MRangeLength);
        MRangeWrapped = true;
      }
      if (MLoopActive) {
        MLoopPos += MLoopSpeed;
        if ((MLoopPos >= MLoopLength) || (MLoopPos < 0.0)) {
          MLoopPos = KODE_Modulo(MLoopPos,MLoopLength);
          MLoopWrapped = true;
        }
      } // loop_active
    } // range_active


  }

//------------------------------
public:
//------------------------------

  void process(KODE_ProcessContext* AContext) {

    KODE_Assert(MBufferNumBeats > 0);
    KODE_Assert(MBufferNumSlices > 0);
    KODE_Assert(AContext->samplerate > 0.0);
    KODE_Assert(AContext->tempo > 0.0);

    //----- start/stop

    MIsPlaying = (AContext->playstate & KODE_PLUGIN_PLAYSTATE_PLAYING);

    if (MIsPlaying & !MWasPlaying) {
      handle_playback_start();
    }

    if (MWasPlaying & !MIsPlaying) {
      handle_playback_stop();
    }

    MWasPlaying = MIsPlaying;

    //-----

    MBeatPos          = KODE_Modulo(AContext->beatpos,(float)MBufferNumBeats);
    MBlockSize        = AContext->numsamples;
    MBeatsPerSecond   = AContext->tempo / 60.0;
    MBeatsPerSample   = MBeatsPerSecond / AContext->samplerate;
    MSamplesPerBeat   = AContext->samplerate / MBeatsPerSecond;
    //MSamplesPerSlice  = MSamplesPerBeat / MBufferNumSlices;
    MBufferLength     = MSamplesPerBeat * MBufferNumBeats;
    MSliceLength      = MSamplesPerBeat / MBufferNumSlices;;//MSamplesPerSlice;
    MBeatSpeed        = MBeatsPerSample;

    // process samples

    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];
    for (uint32_t i=0; i<MBlockSize; i++) {
      float in0 = *input0++;
      float in1 = *input1++;
      float out0 = in0;
      float out1 = in1;

      pre_process();

        //uint32_t readpos = MBufferPos;
        //if (MLoopActive) {
        //  readpos = (MLoopStart + MLoopPos);
        //}
        //float buf0 = MBuffer[(readpos*2)  ];
        //float buf1 = MBuffer[(readpos*2)+1];
        //out0 = buf0;
        //out1 = buf1;

        uint32_t writepos = (uint32_t)MBufferPos * 2;
        MBuffer[writepos  ] = in0;
        MBuffer[writepos+1] = in1;

        if (MLoopActive) {
          MReadPos = MLoopStart + MLoopPos;
          uint32_t readpos = (uint32_t)MReadPos * 2;
          out0 = MBuffer[readpos  ];
          out1 = MBuffer[readpos+1];
        }
        else {
          MReadPos = MBufferPos;
          //out0 = in0;
          //out1 = in1;
        }

      post_process();

      *output0++ = out0;
      *output1++ = out1;
    }

  }

  //----------

};

#undef BUFFERSIZE

//----------------------------------------------------------------------
#endif
