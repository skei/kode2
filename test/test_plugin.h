
#define KODE_NO_GUI
#define KODE_NO_PAINTER

//----------------------------------------------------------------------

#include "base/kode.h"
#include "plugin/kode_descriptor.h"
#include "plugin/kode_instance.h"
#include "plugin/kode_plugin.h"
#include "plugin/kode_process_context.h"

//----------------------------------------------------------------------


// cat /proc/sys/kernel/random/uuid
// 9ca2b934-948c-46fd-9ecf-0123e5dc544a
uint8_t myPluginId[16]  = {0x9c,0xa2,0xb9,0x34,0x94,0x8c,0x46,0xfd,0x9e,0xcf,0x01,0x23,0xe5,0xdc,0x54,0x4a};

//----------

class myDescriptor
: public KODE_Descriptor {

public:

  myDescriptor() {
    MName = "test_plugin";
    MAuthor = "skai.audio";
    MLongId = (uint8_t*)"test_plugin_1234";

  }

  //virtual ~myDescriptor() {
  //  KODE_PRINT;
  //}

};

//----------------------------------------------------------------------

class myInstance
: public KODE_Instance {

public:

  myInstance(KODE_Descriptor* ADescriptor)
  : KODE_Instance(ADescriptor) {
    //KODE_PRINT;
  }

  //virtual ~myInstance() {
  //  KODE_PRINT;
  //}

public:

  //void on_create() final {
  //  KODE_PRINT;
  //}

  //void on_destroy() final {
  //  KODE_PRINT;
  //}

  //void on_initialize() final {
  //  KODE_PRINT;
  //}

  //void on_terminate() final {
  //  KODE_PRINT;
  //}

  //void on_activate() final {
  //  KODE_PRINT;
  //}

  //void on_deactivate() final {
  //  KODE_PRINT;
  //}

  //void on_prepare(float ASampleRate) final {
  //  KODE_Print("samplerate %.2f\n");
  //}

  //void on_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {
  //  KODE_Print("offset %i msg1 %i msg2 %i msg3 %i\n",AOffset,AMsg1,AMsg2,AMsg3);
  //}

  //void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
  //  KODE_Print("index %i value %.2f (mode %i)\n",AIndex,AValue,AMode);
  //}

  //void on_processBlock(KODE_ProcessContext* AContext) final {
  //  //KODE_PRINT;
  //}

  //KODE_Editor* on_openEditor(void* AParent) final {
  //  KODE_PRINT;
  //  return KODE_NULL;
  //}

  //void on_closeEditor(KODE_Editor* AEditor) final {
  //  KODE_PRINT;
  //}

  //void on_updateEditor(KODE_Editor* AEditor) final {
  //  KODE_PRINT;
  //}

  //uint32_t on_saveState(void** ABuffer, uint32_t AMode) final {
  //  KODE_PRINT;
  //  *ABuffer = KODE_NULL;
  //  return 0;
  //}

  //void on_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {
  //  KODE_PRINT;
  //}

};

//----------------------------------------------------------------------

KODE_PLUGIN_ENTRYPOINT(myDescriptor,myInstance)
