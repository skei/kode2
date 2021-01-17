#ifndef kode_audio_portaudio_incuded
#define kode_audio_portaudio_incuded
//----------------------------------------------------------------------

// TEMPORARY

#include "portaudio.h"
#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------

/*
const char* host_api_names[] = {
  "paInDevelopment",  // 0
  "paDirectSound",    // 1
  "paMME",            // 2
  "paASIO",           // 3
  "paSoundManager",   // 4
  "paCoreAudio",      // 5
  "(undefined)",      // 6
  "paOSS",            // 7
  "paALSA",           // 8
  "paAL",             // 9
  "paBeOS",           // 10
  "paWDMKS",          // 11
  "paJACK",           // 12
  "paWASAPI",         // 13
  "paAudioScienceHPI" // 14
};
*/

//----------

struct KODE_PortAudioContext {
  void*     input;
  void*     output;
  uint32_t  frames;
  float     time;
  float     input_time;
  float     output_time;
};

//----------

class KODE_PortAudioListener {
public:
  virtual void portaudio_callback(KODE_PortAudioContext* AContext) {}

};

//----------------------------------------------------------------------

class KODE_PortAudio {

//------------------------------
private:
//------------------------------

  PaStream* MStream = KODE_NULL;

//------------------------------
public:
//------------------------------

  KODE_PortAudio() {
  }

  //----------

  ~KODE_PortAudio() {
  }

//------------------------------
private:
//------------------------------

  static
  int KODE_PortAudioCallback(const void* input, void* output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData) {
    KODE_PortAudioContext context;
    context.input = (void*)input;
    context.output = output;
    context.frames = frameCount;
    context.time = timeInfo->currentTime * 1000.0f;
    context.input_time = timeInfo->inputBufferAdcTime * 1000.0f;
    context.output_time = timeInfo->outputBufferDacTime * 1000.0f;
    KODE_PortAudioListener* listener = (KODE_PortAudioListener*)userData;
    listener->portaudio_callback(&context);
    return 0;
  }

  //----------


  bool checkError(PaError err) {
    if (err != paNoError) {
      printf("PortAudio error: %s\n", Pa_GetErrorText(err));
      return false;
    }
    return true;
  }

//------------------------------
public:
//------------------------------

  bool initialize() {
    PaError err = Pa_Initialize();
    return checkError(err);
  }

  //----------

  bool terminate() {
    PaError err = Pa_Terminate();
    return checkError(err);
  }

  //----------

  // if ABlockSize = paFramesPerBufferUnspecified, PortAudio will do its best
  // to manage it for you, but, on some platforms, the framesPerBuffer will
  // change in each call to the callback

  bool open(uint32_t ANumInputs, uint32_t AInputDevice, uint32_t ANumOutputs, uint32_t AOutputDevice, float ASampleRate, uint32_t ABlockSize) {
    PaStreamParameters inputParameters;
    inputParameters.channelCount                = ANumInputs;
    inputParameters.device                      = AInputDevice;
    inputParameters.sampleFormat                = paFloat32;
    inputParameters.suggestedLatency            = Pa_GetDeviceInfo(AInputDevice)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo   = KODE_NULL;
    PaStreamParameters outputParameters;
    outputParameters.channelCount               = ANumOutputs;
    outputParameters.device                     = AOutputDevice;
    outputParameters.sampleFormat               = paFloat32;
    outputParameters.suggestedLatency           = Pa_GetDeviceInfo(AOutputDevice)->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo  = KODE_NULL;
    PaError err = Pa_OpenStream(
      &MStream,
      &inputParameters,
      &outputParameters,
      ASampleRate,
      ABlockSize,
      paNoFlag,
      KODE_PortAudioCallback,
      (void *)this
    );
    return checkError(err);
  }

  //----------

  bool openDefault(float ASampleRate, uint32_t ABlockSize, void* AUserData) {
    PaError err = Pa_OpenDefaultStream(
      &MStream,
      0,
      2,
      paFloat32,
      ASampleRate,
      ABlockSize,
      KODE_PortAudioCallback,
      (void*)this
    );
    return checkError(err);
  }

  //----------

  bool close() {
    PaError err = Pa_CloseStream(MStream);
    return checkError(err);
  }

  //----------

  bool start() {
    PaError err = Pa_StartStream(MStream);
    return checkError(err);
  }

  //----------

  bool stop() {
    PaError err = Pa_StopStream(MStream);
    return checkError(err);
  }

  //----------

  bool abort() {
    PaError err = Pa_AbortStream(MStream);
    return checkError(err);
  }

  //----------

  float getTime() {
    PaTime time = Pa_GetStreamTime(MStream);
    return time;
  }

  //----------

  float getCpuLoad() {
    double load = Pa_GetStreamCpuLoad(MStream);
    return load;
  }

//------------------------------
public:
//------------------------------

  void printDevices() {
    uint32_t num = Pa_GetDeviceCount();
    for (uint32_t i=0; i<num; i++) {
      printf("device %i\n",i);
      const PaDeviceInfo* device_info = Pa_GetDeviceInfo(i);
      printf("  name: %s\n",      device_info->name);
      printf("  ins:  %i\n",      device_info->maxInputChannels);
      printf("  outs: %i\n",      device_info->maxOutputChannels);
      printf("  default low  input  latency:  %.3f ms\n", (float)device_info->defaultLowInputLatency * 1000.0f);
      printf("  default low  output latency:  %.3f ms\n", (float)device_info->defaultLowOutputLatency * 1000.0f);
      printf("  default high input  latency:  %.3f ms\n", (float)device_info->defaultHighInputLatency * 1000.0f);
      printf("  default high output latency:  %.3f ms\n", (float)device_info->defaultHighOutputLatency * 1000.0f);
      printf("  default sample rate:          %.3f\n", device_info->defaultSampleRate);
      PaHostApiIndex api_index = device_info->hostApi;
      PaHostApiInfo* host_api = (PaHostApiInfo*)Pa_GetHostApiInfo(api_index);
      printf("  api:  %i - %s\n", host_api->type,host_api->name);
    }
  }

  //----------

};

//----------------------------------------------------------------------
#endif
