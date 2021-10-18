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

  * 2 to be sure ..

*/

// 4mb stereo
#define BUFFERSIZE (1024 * 1024 * 4 * 2 * 2)

//----------------------------------------------------------------------

// uint32_t num  = KODE_MaxI(1, rnd_bits(PRangeSliceBits) );

uint32_t KODE_RandomIntFromBits(uint32_t ABits) {
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
  return result;
}


//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class myProcess {

//------------------------------
public:
//------------------------------

  uint32_t  par_BufferNumBeats      = 0;
  uint32_t  par_BufferNumSlices     = 0;

  float     par_RepeatProb          = 0.0;
  uint32_t  par_RepeatSliceBits     = 0;
  uint32_t  par_RepeatSplitBits     = 0;

  float     par_LoopsizeRangeProb   = 0.0;
  float     par_LoopsizeRangeMin    = 0.0;
  float     par_LoopsizeRangeMax    = 0.0;
  float     par_LoopsizeLoopProb    = 0.0;
  float     par_LoopsizeLoopMin     = 0.0;
  float     par_LoopsizeLoopMax     = 0.0;

  float     par_LoopspeedRangeProb  = 0.0;
  float     par_LoopspeedRangeMin   = 0.0;
  float     par_LoopspeedRangeMax   = 0.0;
  float     par_LoopspeedLoopProb   = 0.0;
  float     par_LoopspeedLoopMin    = 0.0;
  float     par_LoopspeedLoopMax    = 0.0;

  float     par_OffsetRangeProb     = 0.0;
  int32_t   par_OffsetRangeMin      = 0.0;
  int32_t   par_OffsetRangeMax      = 0.0;
  float     par_OffsetLoopProb      = 0.0;
  int32_t   par_OffsetLoopMin       = 0.0;
  int32_t   par_OffsetLoopMax       = 0.0;

  float     par_ReverseRangeProb    = 0.0;
  float     par_ReverseLoopProb     = 0.0;

  uint32_t  par_FXMulti             = 0;
  float     par_FXRangeProb         = 0.0;
  float     par_FXRangeMin          = 0.0;
  float     par_FXRangeMax          = 0.0;
  float     par_FXLoopProb          = 0.0;
  float     par_FXLoopMin           = 0.0;
  float     par_FXLoopMax           = 0.0;

  float     par_EnvLoopAtt          = 0.0;
  float     par_EnvLoopDec          = 0.0;
  float     par_EnvSliceAtt         = 0.0;
  float     par_EnvSliceDec         = 0.0;

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
  float     MPrevTempo        = 0.0;

//------------------------------
public:
//------------------------------

  float     MBuffer[BUFFERSIZE] = {0};  // and we double it again, just to be sure :-)
  float     MReadPos            = 0.0;
  float     MBufferPos          = 0.0;
  float     MBufferLength       = 0.0;
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
  bool      MLoopReverse        = false;
  bool      MLoopWrapped        = false;
  uint32_t  MLoopNumDivide      = 0;
  uint32_t  MLoopCounter        = 0;
  float     MFXAmount           = 0;

  float     MOffset             = 0.0;

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

  // called from:
  //   process

  void handle_playback_start() {
    buffer_start();
  }

  //----------

  // called from:
  //   process

  void handle_playback_stop() {
  }

  //----------

  void handle_tempo(float ATempo) {
    if (ATempo != MPrevTempo) {
      //KODE_Print("tempo %f\n",ATempo);
      MRangeActive  = false;
      MLoopActive   = false;
      MLoopReverse  = false;
      //MBufferPos    = 0;
      //MRangePos     = 0;
      MSlicePos     = 0;
      //MLoopPos      = 0;
      //MOffset       = 0;
    }
    MPrevTempo = ATempo;
  }

  //----------

  // called from:
  //   buffer_start
  //   buffer_wraparound
  //   slice_wraparound

  void handle_next_slice(uint32_t ACounter) {
    if (MRangeActive) {
    }
    else {
      if (KODE_Random() < par_RepeatProb) {
        prob_start();
        prob_range();
      }
    }
  }

  //----------

  // called from:
  //   loop_wraparound

  void handle_next_loop(uint32_t ACounter) {
    if (MLoopActive) {
      prob_loop();
    }
  }

  //----------

  void prob_start() {

    MOffset = 0.0;

    uint32_t slicesleft = ((par_BufferNumBeats * par_BufferNumSlices) - MSliceCounter);
    uint32_t numslices = KODE_MaxI(1, KODE_RandomIntFromBits(par_RepeatSliceBits) );
    numslices = KODE_MinI(numslices,slicesleft);
    range_start(numslices);
    uint32_t numdiv = KODE_MaxI(1, KODE_RandomIntFromBits(par_RepeatSplitBits) );
    loop_start(MBufferPos,numdiv,1.0,false);


  }

  //----------

  void prob_range() {
    if (KODE_Random() < par_LoopsizeRangeProb)  set_random_loopsize(par_LoopsizeRangeMin,par_LoopsizeRangeMax);
    if (KODE_Random() < par_LoopspeedRangeProb) set_random_loopspeed(par_LoopspeedRangeMin,par_LoopspeedRangeMax);
    if (KODE_Random() < par_OffsetRangeProb)    set_random_offset(par_OffsetRangeMin,par_OffsetRangeMax);
    if (KODE_Random() < par_ReverseRangeProb)   toggle_random_reverse();
    if (KODE_Random() < par_FXRangeProb)        set_random_fx(par_FXRangeMin,par_FXRangeMax);
  }

  //----------

  void prob_loop() {
    if (KODE_Random() < par_LoopsizeLoopProb)   set_random_loopsize(par_LoopsizeLoopMin,par_LoopsizeLoopMax);
    if (KODE_Random() < par_LoopspeedLoopProb)  set_random_loopspeed(par_LoopspeedLoopMin,par_LoopspeedLoopMax);
    if (KODE_Random() < par_OffsetLoopProb)     set_random_offset(par_OffsetLoopMin,par_OffsetLoopMax);
    if (KODE_Random() < par_ReverseLoopProb)    toggle_random_reverse();
    if (KODE_Random() < par_FXLoopProb)         set_random_fx(par_FXLoopMin,par_FXLoopMax);
  }

//------------------------------
//
//------------------------------

  void set_random_loopsize(float AMin, float AMax) {
    if (AMin >= AMax) {
      float temp = AMin;
      AMin = AMax;
      AMax = temp;
    }
    float range = AMax - AMin;
    float amount = range * KODE_Random();
    //amount = (amount * 2.0) - 1.0;
    MLoopLength *= (AMin + amount);
    MLoopLength = KODE_Clamp(MLoopLength,16.0,MRangeLength);
  }

  //----------

  void set_random_loopspeed(float AMin, float AMax) {
    if (AMin >= AMax) {
      float temp = AMin;
      AMin = AMax;
      AMax = temp;
    }
    float range = AMax - AMin;
    float amount = range * KODE_Random();
    MLoopSpeed *= (AMin + amount);
    MLoopSpeed =  KODE_Clamp(MLoopSpeed, (1.0/16.0), 16.0);
  }

  //----------

  void set_random_offset(int32_t AMin, int32_t AMax) {
    if (AMin >= AMax) {
      int32_t temp = AMin;
      AMin = AMax;
      AMax = temp;
    }

    int32_t num = KODE_RandomRangeInt(AMin,AMax);
    MOffset = (float)num * MSliceLength;

  }

  //----------

  void toggle_random_reverse() {
    if (MLoopReverse ==true) {
      MLoopReverse = false;
    }
    else {
      MLoopReverse = true;
    }
  }

  //----------

  void set_random_fx(float AMin, float AMax) {
    if (AMin >= AMax) {
      float temp = AMin;
      AMin = AMax;
      AMax = temp;
    }
    float range = AMax - AMin;
    float amount = range * KODE_Random();
    MFXAmount *= (AMin + amount);
    MFXAmount = KODE_Clamp(MFXAmount, 0, 1);
  }

//------------------------------
private:
//------------------------------

  //--------------------
  // buffer
  //--------------------

  // called from:
  //   start
  //   handle_playback_start

  void buffer_start() {
    MBufferPos = 0.0;
//    MOffset = 0.0;
    MBufferWrapped = false;
    slice_start();
    MRangeActive = false;
    MLoopActive = false;
    handle_next_slice(MSliceCounter);
  }

  //----------

  // called from:
  //   pre_process

  void buffer_wraparound() {
    MSliceCounter = 0;
    MSlicePos = 0.0;
    MSliceWrapped = false;
    MRangeActive = false;
    MLoopActive = false;
//    MOffset = 0.0;
    handle_next_slice(MSliceCounter);
  }

  //--------------------
  // slice
  //--------------------

  // called from:
  //   buffer_start

  void slice_start() {
    MSliceCounter = 0;
    MSlicePos = 0.0;
    MSliceSpeed = 1.0;
    MSliceWrapped = false;
    //MOffset = 0.0;
  }

  //----------

  // called from:
  //   pre_process

  void slice_wraparound() {
    MSliceCounter += 1;
    handle_next_slice(MSliceCounter);
  }

  //--------------------
  // range
  //--------------------

  // called from:
  //   handle_next_slice

  void range_start_prob() {
    if (KODE_Random() < par_RepeatProb) {

MOffset = 0.0;

      prob_start();
      prob_range();
    }
  }

  //----------

  // called from:
  //   range_start_prob

  void range_start(uint32_t ANumSlices) {
    MRangeActive    = true;
    MRangeStart     = MBufferPos;
    MRangePos       = 0.0;
    MRangeLength    = (MSliceLength * (float)ANumSlices) - 1.0;
    MRangeNumSlices = ANumSlices;
    MRangeWrapped   = false;
    MFXAmount       = 0.0;
//    MOffset         = 0.0;
  }

  //----------

  // called from:
  //   pre_process

  void range_end() {
    MRangeActive    = false;
    MRangeStart     = 0.0;
    MRangePos       = 0.0;
    MRangeLength    = 0.0;
    MRangeNumSlices = 0;
    MRangeWrapped   = false;
    loop_end();
    MFXAmount       = 0.0;
//    MOffset         = 0.0;
}

  //--------------------
  // loop
  //--------------------

  // called from:
  //   handle_next_loop

  //void loop_start_prob() {
  //}

  //----------

  // called from:
  //   range_start_prob

  void loop_start(float AStartPos, uint32_t ANumDiv=1, float ASpeed=1.0, bool AReverse=false) {

    MLoopActive     = true;
    MLoopNumDivide  = KODE_MaxI(1,ANumDiv);
    MLoopStart      = AStartPos;
    MLoopSpeed      = ASpeed;
    float mult      = 1.0 / (float)MLoopNumDivide;
    MLoopLength     = MRangeLength * mult;
    MLoopReverse    = AReverse;
    MLoopWrapped    = false;
    MLoopCounter    = 0;

    if (AReverse) MLoopPos = MLoopLength - MLoopSpeed;
    else MLoopPos = 0.0;

//    MOffset         = 0.0;

  }

  //----------

  // called from:
  //   pre_process

  void loop_wraparound() {
    MLoopCounter += 1;
    handle_next_loop(MLoopCounter);
  }

  //----------

  // called from:
  //   range_end

  void loop_end() {
    MLoopActive     = false;
    MLoopNumDivide  = 0;;
    MLoopPos        = 0.0;
    MLoopLength     = 0.0;
    MLoopSpeed      = 0.0;
    MLoopReverse    = false;
    MLoopStart      = 0.0;
    MLoopWrapped    = false;
    MLoopCounter    = 0;

//    MOffset         = 0.0;

  }

//------------------------------
//
//------------------------------

  // called from:
  //   process

  void pre_process() {
    if (MBufferWrapped) {
      MBufferWrapped = false;
      buffer_wraparound();
    }
    else {
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

  // called from:
  //   process

  void post_process() {
    MBufferPos  += 1.0;
    if ((MBufferPos >= MBufferLength) || (MBufferPos < 0.0)) {
      MBufferPos = KODE_Modulo(MBufferPos,MBufferLength);
      MBufferWrapped = true;
    }
    MBeatPos += MBeatSpeed;
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
        if (MLoopPos >= MLoopLength) {
          MLoopPos = KODE_Modulo(MLoopPos,MLoopLength);
          MLoopWrapped = true;
        }
        if ((MLoopPos >= MLoopLength) && (!MLoopReverse)) {
          while (MLoopPos >= MLoopLength) MLoopPos -= MLoopLength;
          MLoopWrapped = true;
        }
        else
        if ((MLoopPos < 0.0)  && (MLoopReverse)){
          while (MLoopPos < 0.0) MLoopPos += MLoopLength;
          MLoopWrapped = true;
        }

      } // loop_active
    } // range_active
  }

//------------------------------
public:
//------------------------------

  void process(KODE_ProcessContext* AContext) {
    KODE_Assert(par_BufferNumBeats > 0);
    KODE_Assert(par_BufferNumSlices > 0);
    KODE_Assert(AContext->samplerate > 0.0);
    KODE_Assert(AContext->tempo > 0.0);

    // start/stop

    MIsPlaying = (AContext->playstate & KODE_PLUGIN_PLAYSTATE_PLAYING);
    if (MIsPlaying & !MWasPlaying) {
      handle_playback_start();
    }
    if (MWasPlaying & !MIsPlaying) {
      handle_playback_stop();
    }
    MWasPlaying = MIsPlaying;

    //

    handle_tempo(AContext->tempo);

    MBeatPos          = KODE_Modulo(AContext->beatpos,(float)par_BufferNumBeats);
    MBlockSize        = AContext->numsamples;
    MBeatsPerSecond   = AContext->tempo / 60.0;
    MBeatsPerSample   = MBeatsPerSecond / AContext->samplerate;
    MSamplesPerBeat   = AContext->samplerate / MBeatsPerSecond;
    MBufferLength     = MSamplesPerBeat * par_BufferNumBeats;
    MSliceLength      = MSamplesPerBeat / par_BufferNumSlices;
    MBeatSpeed        = MBeatsPerSample;

    //if (AContext->tempo != MPrevTempo) {
    //  handle_tempo(AContext->tempo)
    //}

    //

    float* input0 = AContext->inputs[0];
    float* input1 = AContext->inputs[1];
    float* output0 = AContext->outputs[0];
    float* output1 = AContext->outputs[1];
    for (uint32_t i=0; i<MBlockSize; i++) {
      float in0 = *input0++;
      float in1 = *input1++;
      float out0 = in0;
      float out1 = in1;

if (MIsPlaying /*|| MMFreeWheeling*/) {

      pre_process();
      uint32_t writepos = (uint32_t)MBufferPos * 2;

      writepos %= (BUFFERSIZE-1);

      MBuffer[writepos  ] = in0;
      MBuffer[writepos+1] = in1;
      if (MLoopActive) {
        if (MLoopReverse) {
          MReadPos = MLoopStart + MLoopLength - MLoopPos;
        }
        else {
          MReadPos = MLoopStart + MLoopPos;
        }

        MReadPos += MOffset;
        MReadPos = KODE_Modulo (MReadPos,MBufferLength);
        uint32_t readpos = (uint32_t)MReadPos * 2;

        readpos %= (BUFFERSIZE-1); // needed ???

        out0 = MBuffer[readpos  ];
        out1 = MBuffer[readpos+1];
      }
      else {
        MReadPos = MBufferPos;
      }

      // envelopes

      float env = 1.0f;
      float sa = par_EnvSliceAtt * MSliceLength;
      float sd = par_EnvSliceDec * MSliceLength;
      if ((sa > 0) && (MSlicePos < sa)) env *= MSlicePos / sa;
      if ((sd > 0) && (MSlicePos >= (MSliceLength - sd))) env *= (MSliceLength - MSlicePos) / sd;
      if (MLoopActive) {
        float la = par_EnvLoopAtt * (AContext->samplerate / 10.0); // 0.01 in on_parameter,  1/100 / 10 = 1/1000
        float ld = par_EnvLoopDec * (AContext->samplerate / 10.0);
        if ((la > 0) && (MLoopPos < la)) env *= MLoopPos / la;
        if ((ld > 0) && (MLoopPos >= (MLoopLength - ld))) env *= (MLoopLength - MLoopPos) / ld;
      }
      out0 *= env;
      out1 *= env;

      //

      post_process();

} // playing

      *output0++ = out0;
      *output1++ = out1;
    }
    //KODE_DPrint("%f\n",MLoopPos);

  }

  //----------

};

#undef BUFFERSIZE

//----------------------------------------------------------------------
#endif
