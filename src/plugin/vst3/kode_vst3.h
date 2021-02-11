#ifndef kode_vst3_included
#define kode_vst3_included
//----------------------------------------------------------------------

#ifdef KODE_DEBUG_VST3
  #define VST3_Print   KODE_Print
  #define VST3_DPrint  KODE_DPrint
  #define VST3_PRINT   KODE_PRINT
#else
  #define VST3_Print   KODE_NoPrint
  #define VST3_DPrint  KODE_NoPrint
  #define VST3_PRINT   {}
#endif

#ifdef KODE_DEBUG_VST3_RT
  #define VST3_RT_Print   KODE_Print
  #define VST3_RT_DPrint  KODE_DPrint
  #define VST3_RT_PRINT   KODE_PRINT
#else
  #define VST3_RT_Print   KODE_NoPrint
  #define VST3_RT_DPrint  KODE_NoPrint
  #define VST3_RT_PRINT   {}
#endif

//----------------------------------------------------------------------
//
// based on (extracted from) steinberg vst3 sdk (gpl3)
//
//----------------------------------------------------------------------

#ifdef KODE_LINUX
  #define KODE_VST3_COM_COMPATIBLE 0
  #define KODE_VST3_PLUGIN_API
#endif

#ifdef KODE_WIN32
  #define KODE_VST3_COM_COMPATIBLE 1
  #define KODE_VST3_PLUGIN_API __stdcall
#endif

//----------

#if KODE_VST3_COM_COMPATIBLE
  #define KODE_VST3_INLINE_UID(l1,l2,l3,l4) {                                  \
    (int8_t)((l1 & 0x000000FF)      ), (int8_t)((l1 & 0x0000FF00) >>  8), \
    (int8_t)((l1 & 0x00FF0000) >> 16), (int8_t)((l1 & 0xFF000000) >> 24), \
    (int8_t)((l2 & 0x00FF0000) >> 16), (int8_t)((l2 & 0xFF000000) >> 24), \
    (int8_t)((l2 & 0x000000FF)      ), (int8_t)((l2 & 0x0000FF00) >>  8), \
    (int8_t)((l3 & 0xFF000000) >> 24), (int8_t)((l3 & 0x00FF0000) >> 16), \
    (int8_t)((l3 & 0x0000FF00) >>  8), (int8_t)((l3 & 0x000000FF)      ), \
    (int8_t)((l4 & 0xFF000000) >> 24), (int8_t)((l4 & 0x00FF0000) >> 16), \
    (int8_t)((l4 & 0x0000FF00) >>  8), (int8_t)((l4 & 0x000000FF)      )  \
  }
#else
  #define KODE_VST3_INLINE_UID(l1,l2,l3,l4) {                               \
    (int8_t)((l1 & 0xFF000000) >> 24), (int8_t)((l1 & 0x00FF0000) >> 16), \
    (int8_t)((l1 & 0x0000FF00) >>  8), (int8_t)((l1 & 0x000000FF)      ), \
    (int8_t)((l2 & 0xFF000000) >> 24), (int8_t)((l2 & 0x00FF0000) >> 16), \
    (int8_t)((l2 & 0x0000FF00) >>  8), (int8_t)((l2 & 0x000000FF)      ), \
    (int8_t)((l3 & 0xFF000000) >> 24), (int8_t)((l3 & 0x00FF0000) >> 16), \
    (int8_t)((l3 & 0x0000FF00) >>  8), (int8_t)((l3 & 0x000000FF)      ), \
    (int8_t)((l4 & 0xFF000000) >> 24), (int8_t)((l4 & 0x00FF0000) >> 16), \
    (int8_t)((l4 & 0x0000FF00) >>  8), (int8_t)((l4 & 0x000000FF)      )  \
  }
#endif

//#ifdef KODE_VST3_INIT_CLASS_IID
//  #define KODE_VST3_DECLARE_CLASS_IID(classname,l1,l2,l3,l4)
//	static const vst3_uuid classname##_iid = KODE_VST3_INLINE_UID(l1,l2,l3,l4);
//  const vst3_uuid classname::iid(classname##_iid);
//#else

#define KODE_VST3_DECLARE_CLASS_IID(classname,l1,l2,l3,l4) \
  static const KODE_Vst3Id classname##_iid = KODE_VST3_INLINE_UID(l1,l2,l3,l4);

//----------

//#ifndef kode_vst3_VstVersionString
#define kode_vst3_VstVersionString	"VST 3.6.10"	///< SDK version for PClassInfo2
//#endif

//#ifndef kode_vst3_VstAudioEffectClass
#define kode_vst3_VstAudioEffectClass "Audio Module Class"
//#endif

//----------------------------------------------------------------------
//
// typedefs
//
//----------------------------------------------------------------------

//typedef char                char8;
//typedef int16               char16;
//typedef char16_t            char16;
//typedef __wchar_t           char16;
//typedef char                int8;
//typedef short               int16;
//typedef int                 int32;
//typedef int32_t             int32_t;
//typedef long long           int64;
//typedef char8               tchar;
//typedef char16              tchar;
//typedef int32_t             tresult;
//typedef unsigned char       uchar;
//typedef unsigned char       uint8;
//typedef unsigned short      uint16;
//typedef unsigned int        uint32;
//typedef unsigned long long  uint64;

//typedef int32_t             BusDirection;
//typedef int32_t             BusType;
//typedef int16_t             CtrlNumber;
//typedef const char*         CString;
//typedef const char*         FIDString;
//typedef int                 FileDescriptor;
//typedef TUID                FUID;
//typedef int32_t             IoMode;
//typedef uint32_t            KeyswitchTypeID;
//typedef int32_t             KnobMode;
//typedef int32_t             MediaType;
//typedef uint32_t            NoteExpressionTypeID;
//typedef double              NoteExpressionValue;
//typedef uint32_t            ParamID;
//typedef double              ParamValue;
//typedef int32_t             ProgramListID;
//typedef float               Sample32;
//typedef double              Sample64;
//typedef double              SampleRate;
//typedef uint64_t            Speaker;
//typedef uint64_t            SpeakerArrangement;
//typedef TChar               String128[128];
//typedef uint8_t             TBool;
//typedef char16              TChar;
//typedef uint64_t            TimerInterval;
//typedef double              TQuarterNotes;
//typedef int64_t             TSamples;
//typedef int8                TUID[16];
//typedef int32_t             UnitID;


typedef char      KODE_Vst3Id[16];      // was TUID
typedef char16_t  KODE_Vst3String[128]; // was String128

//----------------------------------------------------------------------
//
// enums
//
//----------------------------------------------------------------------

#if KODE_VST3_COM_COMPATIBLE

  #ifdef KODE_WIN32
  enum {
    kode_vst3_NoInterface     = 0x80004002;   // static_cast<int32_t>(0x80004002L),	// E_NOINTERFACE
    kode_vst3_ResultOk        = 0x00000000;   //static_cast<int32_t>(0x00000000L),	// S_OK
    kode_vst3_ResultTrue      = kode_vst3_ResultOk,
    kode_vst3_ResultFalse     = 0x00000001;   // static_cast<int32_t>(0x00000001L),	// S_FALSE
    kode_vst3_InvalidArgument = 0x80070057;   // static_cast<int32_t>(0x80070057L),	// E_INVALIDARG
    kode_vst3_NotImplemented  = 0x80004001;   // static_cast<int32_t>(0x80004001L),	// E_NOTIMPL
    kode_vst3_InternalError   = 0x80004005;   // static_cast<int32_t>(0x80004005L),	// E_FAIL
    kode_vst3_NotInitialized  = 0x8000FFFF;   // static_cast<int32_t>(0x8000FFFFL),	// E_UNEXPECTED
    kode_vst3_OutOfMemory     = 0x8007000E;   // static_cast<int32_t>(0x8007000EL)		// E_OUTOFMEMORY
  };
  #else
  enum {
    kode_vst3_NoInterface     = 0x80000004; // static_cast<int32_t>(0x80000004L),	// E_NOINTERFACE
    kode_vst3_ResultOk        = 0x00000000; // static_cast<int32_t>(0x00000000L),	// S_OK
    kode_vst3_ResultTrue      = kode_vst3_ResultOk,
    kode_vst3_ResultFalse     = 0x00000001; // static_cast<int32_t>(0x00000001L),	// S_FALSE
    kode_vst3_InvalidArgument = 0x80000003; // static_cast<int32_t>(0x80000003L),	// E_INVALIDARG
    kode_vst3_NotImplemented  = 0x80000001; // static_cast<int32_t>(0x80000001L),	// E_NOTIMPL
    kode_vst3_InternalError   = 0x80000008; // static_cast<int32_t>(0x80000008L),	// E_FAIL
    kode_vst3_NotInitialized  = 0x8000FFFF; // static_cast<int32_t>(0x8000FFFFL),	// E_UNEXPECTED
    kode_vst3_OutOfMemory     = 0x80000002; // static_cast<int32_t>(0x80000002L)		// E_OUTOFMEMORY
  };
  #endif // KODE_WIN32

#else // KODE_VST3_COM_COMPATIBLE

  enum {
    kode_vst3_NoInterface = -1,
    kode_vst3_ResultOk,
    kode_vst3_ResultTrue = kode_vst3_ResultOk,
    kode_vst3_ResultFalse,
    kode_vst3_InvalidArgument,
    kode_vst3_NotImplemented,
    kode_vst3_InternalError,
    kode_vst3_NotInitialized,
    kode_vst3_OutOfMemory
  };

#endif // KODE_VST3_COM_COMPATIBLE

//----------

//enum NoteExpressionTypeIDs {
//  kode_vst3_VolumeTypeID = 0,
//  kode_vst3_PanTypeID,
//  kode_vst3_TuningTypeID,
//  kode_vst3_VibratoTypeID,
//  kode_vst3_ExpressionTypeID,
//  kode_vst3_BrightnessTypeID,
//  kode_vst3_TextTypeID,
//  kode_vst3_PhonemeTypeID,
//  kode_vst3_CustomStart = 100000
//};

enum KODE_Vst3BusDirections {
  kode_vst3_Input = 0,
  kode_vst3_Output
};

enum KODE_Vst3BusTypes {
  kode_vst3_Main = 0,
  kode_vst3_Aux
};

enum KODE_Vst3ControllerNumbers {
  kode_vst3_CtrlBankSelectMSB      = 0,
  kode_vst3_CtrlModWheel           = 1,
  kode_vst3_CtrlBreath             = 2,
  kode_vst3_CtrlFoot               = 4,
  kode_vst3_CtrlPortaTime          = 5,
  kode_vst3_CtrlDataEntryMSB       = 6,
  kode_vst3_CtrlVolume             = 7,
  kode_vst3_CtrlBalance            = 8,
  kode_vst3_CtrlPan                = 10,
  kode_vst3_CtrlExpression         = 11,
  kode_vst3_CtrlEffect1            = 12,
  kode_vst3_CtrlEffect2            = 13,
  kode_vst3_CtrlGPC1               = 16,
  kode_vst3_CtrlGPC2               = 17,
  kode_vst3_CtrlGPC3               = 18,
  kode_vst3_CtrlGPC4               = 19,
  kode_vst3_CtrlBankSelectLSB      = 32,
  kode_vst3_CtrlDataEntryLSB       = 38,
  kode_vst3_CtrlSustainOnOff       = 64,
  kode_vst3_CtrlPortaOnOff         = 65,
  kode_vst3_CtrlSustenutoOnOff     = 66,
  kode_vst3_CtrlSoftPedalOnOff     = 67,
  kode_vst3_CtrlLegatoFootSwOnOff  = 68,
  kode_vst3_CtrlHold2OnOff         = 69,
  kode_vst3_CtrlSoundVariation     = 70,
  kode_vst3_CtrlFilterCutoff       = 71,
  kode_vst3_CtrlReleaseTime        = 72,
  kode_vst3_CtrlAttackTime         = 73,
  kode_vst3_CtrlFilterResonance    = 74,
  kode_vst3_CtrlDecayTime          = 75,
  kode_vst3_CtrlVibratoRate        = 76,
  kode_vst3_CtrlVibratoDepth       = 77,
  kode_vst3_CtrlVibratoDelay       = 78,
  kode_vst3_CtrlSoundCtrler10      = 79,
  kode_vst3_CtrlGPC5               = 80,
  kode_vst3_CtrlGPC6               = 81,
  kode_vst3_CtrlGPC7               = 82,
  kode_vst3_CtrlGPC8               = 83,
  kode_vst3_CtrlPortaControl       = 84,
  kode_vst3_CtrlEff1Depth          = 91,
  kode_vst3_CtrlEff2Depth          = 92,
  kode_vst3_CtrlEff3Depth          = 93,
  kode_vst3_CtrlEff4Depth          = 94,
  kode_vst3_CtrlEff5Depth          = 95,
  kode_vst3_CtrlDataIncrement      = 96,
  kode_vst3_CtrlDataDecrement      = 97,
  kode_vst3_CtrlNRPNSelectLSB      = 98,
  kode_vst3_CtrlNRPNSelectMSB      = 99,
  kode_vst3_CtrlRPNSelectLSB       = 100,
  kode_vst3_CtrlRPNSelectMSB       = 101,
  kode_vst3_CtrlAllSoundsOff       = 120,
  kode_vst3_CtrlResetAllCtrlers    = 121,
  kode_vst3_CtrlLocalCtrlOnOff     = 122,
  kode_vst3_CtrlAllNotesOff        = 123,
  kode_vst3_CtrlOmniModeOff        = 124,
  kode_vst3_CtrlOmniModeOn         = 125,
  kode_vst3_CtrlPolyModeOnOff      = 126,
  kode_vst3_CtrlPolyModeOn         = 127,
  kode_vst3_AfterTouch             = 128,
  kode_vst3_PitchBend,           //= 129,
  kode_vst3_CountCtrlNumber
};

enum KODE_Vst3MediaTypes {
  kode_vst3_Audio = 0,
  kode_vst3_Event,
  kode_vst3_NumMediaTypes
};

enum KODE_Vst3SymbolicSampleSizes {
  kode_vst3_Sample32,
  kode_vst3_Sample64
};

//----------------------------------------------------------------------
//
// const
//
//----------------------------------------------------------------------

const char*     kode_vst3_Editor                        = "editor";
const char*     kode_vst3_Fx					                  = "Fx";
const char*     kode_vst3_Instrument			              = "Instrument";

const char*     kode_vst3_PlatformTypeHWND              = "HWND";
const char*     kode_vst3_PlatformTypeHIView            = "HIView";
const char*     kode_vst3_PlatformTypeNSView            = "NSView";
const char*     kode_vst3_PlatformTypeUIView            = "UIView";
const char*     kode_vst3_PlatformTypeX11EmbedWindowID  = "X11EmbedWindowID";

const uint64_t  kode_vst3_SpeakerL                      = 1 << 0;
const uint64_t  kode_vst3_SpeakerR                      = 1 << 1;

/*static*/ const int32_t   kode_vst3_NoParentUnitId     = -1;
/*static*/ const int32_t   kode_vst3_NoProgramListId    = -1;
/*static*/ const uint32_t  kode_vst3_NoTail             = 0;
/*static*/ const int32_t   kode_vst3_RootUnitId         = 0;

//namespace KODE_Vst3PlugType {
//  const CString kode_vst3_FxAnalyzer			        = "Fx|Analyzer";
//  const CString kode_vst3_FxDelay				        = "Fx|Delay";
//  const CString kode_vst3_FxDistortion		        = "Fx|Distortion";
//  const CString kode_vst3_FxDynamics			        = "Fx|Dynamics";
//  const CString kode_vst3_FxEQ					          = "Fx|EQ";
//  const CString kode_vst3_FxFilter				        = "Fx|Filter";
//  const CString kode_vst3_Fx					            = "Fx";
//  const CString kode_vst3_FxInstrument			      = "Fx|Instrument";
//  const CString kode_vst3_FxInstrumentExternal	  = "Fx|Instrument|External";
//  const CString kode_vst3_FxSpatial			        = "Fx|Spatial";
//  const CString kode_vst3_FxGenerator			      = "Fx|Generator";
//  const CString kode_vst3_FxMastering			      = "Fx|Mastering";
//  const CString kode_vst3_FxModulation			      = "Fx|Modulation";
//  const CString kode_vst3_FxPitchShift			      = "Fx|Pitch Shift";
//  const CString kode_vst3_FxRestoration		      = "Fx|Restoration";
//  const CString kode_vst3_FxReverb				        = "Fx|Reverb";
//  const CString kode_vst3_FxSurround			        = "Fx|Surround";
//  const CString kode_vst3_FxTools                 = "Fx|Tools";
//  const CString kode_vst3_FxNetwork               = "Fx|Network";
//  const CString kode_vst3_Instrument              = "Instrument";
//  const CString kode_vst3_InstrumentDrum          = "Instrument|Drum";
//  const CString kode_vst3_InstrumentSampler       = "Instrument|Sampler";
//  const CString kode_vst3_InstrumentSynth         = "Instrument|Synth";
//  const CString kode_vst3_InstrumentSynthSampler  = "Instrument|Synth|Sampler";
//  const CString kode_vst3_InstrumentExternal	    = "Instrument|External";
//  const CString kode_vst3_Spatial				        = "Spatial";
//  const CString kode_vst3_SpatialFx			= "Spatial|Fx";
//  const CString kode_vst3_OnlyRealTime			= "OnlyRT";
//  const CString kode_vst3_OnlyOfflineProcess	= "OnlyOfflineProcess";
//  const CString kode_vst3_NoOfflineProcess		= "NoOfflineProcess";
//  const CString kode_vst3_UpDownMix			= "Up-Downmix";
//  const CString kode_vst3_Analyzer			    = "Analyzer";
//  const CString kode_vst3_Ambisonic			= "Ambisonic";
//  const CString kode_vst3_Mono					= "Mono";
//  const CString kode_vst3_Stereo				= "Stereo";
//  const CString kode_vst3_Surround				= "Surround";
//};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#define strncpy8 strncpy

char* strncpy8(char* destination, const char* source, size_t maxnum) {
  //memset(destination,0,maxnum);
  destination[maxnum-1] = 0;
  strncpy(destination,source,maxnum-1);
  return destination;
}


//----------------------------------------------------------------------
//
// structs
//
//----------------------------------------------------------------------

struct KODE_Vst3AudioBusBuffers {
  //KODE_Vst3AudioBusBuffers ()
  //  : numChannels(0)
  //  , silenceFlags(0)
  //  , channelBuffers64(0) {}
  int32_t   numChannels;
  uint64_t  silenceFlags;
  union {
    float**   channelBuffers32;
    double**  channelBuffers64;
  };
};

struct KODE_Vst3BusInfo {
  enum BusFlags {
    kode_vst3_DefaultActive = 1 << 0
  };
  //
  int32_t         mediaType;
  int32_t         direction;
  int32_t         channelCount;
  KODE_Vst3String name;
  int32_t         busType;
  uint32_t        flags;
};

struct KODE_Vst3Chord {
  enum Masks {
    kode_vst3_ChordMask    = 0x0FFF,
    kode_vst3_ReservedMask = 0xF000
  };
  //
  uint8_t keyNote;
  uint8_t rootNote;
  int16_t chordMask;
};

struct KODE_Vst3FrameRate {
  enum FrameRateFlags {
    kode_vst3_PullDownRate = 1 << 0,
    kode_vst3_DropRate     = 1 << 1
  };
  //
  uint32_t  framesPerSecond;
  uint32_t  flags;
};

struct KODE_Vst3KeyswitchInfo {
  uint32_t        typeId;
  KODE_Vst3String title;
  KODE_Vst3String shortTitle;
  int32_t         keyswitchMin;
  int32_t         keyswitchMax;
  int32_t         keyRemapped;
  int32_t         unitId;
  int32_t         flags;
};

struct KODE_Vst3ParameterInfo {
  enum ParameterFlags {
    kode_vst3_CanAutomate      = 1 << 0,
    kode_vst3_IsReadOnly       = 1 << 1,
    kode_vst3_IsWrapAround     = 1 << 2,
    kode_vst3_IsList           = 1 << 3,
    kode_vst3_IsProgramChange  = 1 << 15,
    kode_vst3_IsBypass         = 1 << 16
  };
  //
  uint32_t        id;
  KODE_Vst3String title;
  KODE_Vst3String shortTitle;
  KODE_Vst3String units;
  int32_t         stepCount;
  double          defaultNormalizedValue;
  int32_t         unitId;
  int32_t         flags;
};

struct KODE_Vst3PClassInfo {
  enum ClassCardinality {
    kode_vst3_ManyInstances = 0x7FFFFFFF
  };
  enum {
    kode_vst3_CategorySize  = 32,
    kode_vst3_NameSize      = 64
  };
  //
  KODE_Vst3Id cid;
  int32_t     cardinality;
  char        category[kode_vst3_CategorySize];
  char        name[kode_vst3_NameSize];
//  KODE_Vst3PClassInfo(const KODE_Vst3Id _cid, int32_t _cardinality, const char* _category, const char* _name) {
//    memset(this,0,sizeof(KODE_Vst3PClassInfo));
//    memcpy(cid,_cid,sizeof(KODE_Vst3Id));
//    if (_category) strncpy8(category,_category,kode_vst3_CategorySize);
//    if (_name) strncpy8(name,_name,kode_vst3_NameSize);
//    cardinality = _cardinality;
//  }
//  //#if SMTG_CPP11
//  constexpr KODE_Vst3PClassInfo() : cid(), cardinality(), category(), name() {}
//  //#else
//  //PClassInfo () { memset (this, 0, sizeof (PClassInfo)); }
//  //#endif
};

struct KODE_Vst3PClassInfo2 {
	enum {
		kode_vst3_VendorSize = 64,
		kode_vst3_VersionSize = 64,
		kode_vst3_SubCategoriesSize = 128
	};
	//
	KODE_Vst3Id cid;
	int32_t     cardinality;
	char        category[KODE_Vst3PClassInfo::kode_vst3_CategorySize];
	char        name[KODE_Vst3PClassInfo::kode_vst3_NameSize];
	uint32_t    classFlags;
	char        subCategories[kode_vst3_SubCategoriesSize];
	char        vendor[kode_vst3_VendorSize];
	char        version[kode_vst3_VersionSize];
	char        sdkVersion[kode_vst3_VersionSize];
//	KODE_Vst3PClassInfo2 (const KODE_Vst3Id _cid, int32_t _cardinality, const char* _category, const char* _name,
//		int32_t _classFlags, const char* _subCategories, const char* _vendor, const char* _version, const char* _sdkVersion) {
//      memset (this, 0, sizeof (KODE_Vst3PClassInfo2));
//      memcpy (cid, _cid, sizeof (KODE_Vst3Id));
//      cardinality = _cardinality;
//      if (_category) strncpy8 (category, _category, KODE_Vst3PClassInfo::kode_vst3_CategorySize);
//      if (_name) strncpy8 (name, _name, KODE_Vst3PClassInfo::kode_vst3_NameSize);
//      classFlags = static_cast<uint32_t> (_classFlags);
//      if (_subCategories) strncpy8 (subCategories, _subCategories, kode_vst3_SubCategoriesSize);
//      if (_vendor) strncpy8 (vendor, _vendor, kode_vst3_VendorSize);
//      if (_version) strncpy8 (version, _version, kode_vst3_VersionSize);
//      if (_sdkVersion) strncpy8 (sdkVersion, _sdkVersion, kode_vst3_VersionSize);
//    }
//  //#if SMTG_CPP11
//	constexpr KODE_Vst3PClassInfo2()
//    : cid(), cardinality(), category(), name(), classFlags(), subCategories(), vendor(), version(), sdkVersion() {}
//  //#else
//  //	KODE_Vst3PClassInfo2() { memset(this,0,sizeof(KODE_Vst3PClassInfo2)); }
//  //#endif
};

/*
struct KODE_Vst3PClassInfoW {
	KODE_Vst3Id cid;
	int32 cardinality;
	char8 category[PClassInfo::kode_vst3_CategorySize];
	char16 name[PClassInfo::kode_vst3_NameSize];
	enum { kode_vst3_VendorSize = 64,
		kode_vst3_VersionSize = 64,
		kode_vst3_SubCategoriesSize = 128
	};
	uint32  classFlags;
	char8   subCategories[kode_vst3_SubCategoriesSize];
	char16  vendor[kode_vst3_VendorSize];
	char16  version[kode_vst3_VersionSize];
	char16  sdkVersion[kode_vst3_VersionSize];
	KODE_Vst3PClassInfoW(const KODE_Vst3Id _cid, int32 _cardinality, const char8* _category, const char16* _name, int32 _classFlags, const char8* _subCategories, const char16* _vendor, const char16* _version, const char16* _sdkVersion) {
    memset (this, 0, sizeof (PClassInfoW));
    memcpy (cid, _cid, sizeof (KODE_Vst3Id));
    cardinality = _cardinality;
    if (_category) strncpy8 (category, _category, PClassInfo::kode_vst3_CategorySize);
    if (_name) strncpy16 (name, _name, PClassInfo::kode_vst3_NameSize);
    classFlags = static_cast<uint32> (_classFlags);
    if (_subCategories) strncpy8 (subCategories, _subCategories, kode_vst3_SubCategoriesSize);
    if (_vendor) strncpy16 (vendor, _vendor, kode_vst3_VendorSize);
    if (_version) strncpy16 (version, _version, kode_vst3_VersionSize);
    if (_sdkVersion) strncpy16 (sdkVersion, _sdkVersion, kode_vst3_VersionSize);
	}
  //#if SMTG_CPP11
  constexpr KODE_Vst3PClassInfoW ()
    : cid ()
    , cardinality ()
    , category ()
    , name ()
    , classFlags ()
    , subCategories ()
    , vendor ()
    , version ()
    , sdkVersion ()
  {
  }
  //#else
  //	KODE_Vst3PClassInfoW () { memset (this, 0, sizeof (PClassInfoW)); }
  //#endif
  void fromAscii (const PClassInfo2& ci2) {
    //memcpy (cid, ci2.cid, sizeof (KODE_Vst3Id));
    //cardinality = ci2.cardinality;
    //strncpy8 (category, ci2.category, PClassInfo::kode_vst3_CategorySize);
    //str8ToStr16 (name, ci2.name, PClassInfo::kode_vst3_NameSize);
    //classFlags = ci2.classFlags;
    //strncpy8 (subCategories, ci2.subCategories, kode_vst3_SubCategoriesSize);
    //str8ToStr16(vendor, ci2.vendor, kode_vst3_VendorSize);
    //str8ToStr16(version, ci2.version, kode_vst3_VersionSize);
    //str8ToStr16(sdkVersion, ci2.sdkVersion, kode_vst3_VersionSize);
  }
};
*/

struct KODE_Vst3PClassInfoW {
};

struct KODE_Vst3PFactoryInfo {
  enum FactoryFlags {
    kode_vst3_NoFlags					        = 0,
    kode_vst3_ClassesDiscardable			= 1 << 0,
    kode_vst3_LicenseCheck				    = 1 << 1,
    kode_vst3_ComponentNonDiscardable = 1 << 3,
    kode_vst3_Unicode                 = 1 << 4
  };
  enum {
    kode_vst3_URLSize    = 256,
    kode_vst3_EmailSize  = 128,
    kode_vst3_NameSize   = 64
  };
  //
  char    vendor[kode_vst3_NameSize];
  char    url[kode_vst3_URLSize];
  char    email[kode_vst3_EmailSize];
  int32_t flags;
//  KODE_Vst3PFactoryInfo(const char* _vendor, const char* _url, const char* _email, int32_t _flags) {
//    strncpy8(vendor,_vendor,kode_vst3_NameSize);
//    strncpy8(url,_url,kode_vst3_URLSize);
//    strncpy8(email,_email,kode_vst3_EmailSize);
//    flags = _flags;
//    //#ifdef UNICODE
//    //  flags |= kode_vst3_Unicode;
//    //#endif
//  }
//  //#if SMTG_CPP11
//  constexpr KODE_Vst3PFactoryInfo() : vendor(), url(), email(), flags() {}
//  //#else
//  //  KODE_Vst3PFactoryInfo() { memset(this,0,sizeof(PFactoryInfo)); }
//  //#endif
};

struct KODE_Vst3ProgramListInfo {
  int32_t         id;
  KODE_Vst3String name;
  int32_t         programCount;
};

class KODE_Vst3IParameterChanges;
class KODE_Vst3IEventList;

struct KODE_Vst3ProcessContext {
	enum StatesAndFlags {
		kode_vst3_Playing                = 1 << 1,
		kode_vst3_CycleActive            = 1 << 2,
		kode_vst3_Recording              = 1 << 3,
		kode_vst3_SystemTimeValid        = 1 << 8,
		kode_vst3_ContTimeValid          = 1 << 17,
		kode_vst3_ProjectTimeMusicValid  = 1 << 9,
		kode_vst3_BarPositionValid       = 1 << 11,
		kode_vst3_CycleValid             = 1 << 12,
		kode_vst3_TempoValid             = 1 << 10,
		kode_vst3_TimeSigValid           = 1 << 13,
		kode_vst3_ChordValid             = 1 << 18,
		kode_vst3_SmpteValid             = 1 << 14,
		kode_vst3_ClockValid             = 1 << 15
	};
	//
	uint32_t            state;
	double              sampleRate;
	int64_t             projectTimeSamples;
	int64_t             systemTime;
	int64_t             continousTimeSamples;
	double              projectTimeMusic;
	double              barPositionMusic;
	double              cycleStartMusic;
	double              cycleEndMusic;
	double              tempo;
	int32_t             timeSigNumerator;
	int32_t             timeSigDenominator;
	KODE_Vst3Chord      chord;
	int32_t             smpteOffsetSubframes;
	KODE_Vst3FrameRate  frameRate;
	int32_t             samplesToNextClock;
};

struct KODE_Vst3ProcessData {
//  KODE_Vst3ProcessData()
//    : processMode(0),symbolicSampleSize(kode_vst3_Sample32),numSamples(0),numInputs(0)
//    , numOutputs(0),inputs (0),outputs(0),inputParameterChanges(0),outputParameterChanges(0)
//    , inputEvents(0),outputEvents(0),processContext(0) {}
  int32_t                     processMode;
  int32_t                     symbolicSampleSize;
  int32_t                     numSamples;
  int32_t                     numInputs;
  int32_t                     numOutputs;
  KODE_Vst3AudioBusBuffers*   inputs;
  KODE_Vst3AudioBusBuffers*   outputs;
  KODE_Vst3IParameterChanges* inputParameterChanges;
  KODE_Vst3IParameterChanges* outputParameterChanges;
  KODE_Vst3IEventList*        inputEvents;
  KODE_Vst3IEventList*        outputEvents;
  KODE_Vst3ProcessContext*    processContext;
};

struct KODE_Vst3ProcessSetup {
  int32_t processMode;
  int32_t symbolicSampleSize;
  int32_t maxSamplesPerBlock;
  double  sampleRate;
};

struct KODE_Vst3RoutingInfo {
	int32_t mediaType;
	int32_t busIndex;
	int32_t channel;
};

struct KODE_Vst3UnitInfo {
	int32_t         id;
	int32_t         parentUnitId;
	KODE_Vst3String name;
	int32_t         programListId;
};

struct KODE_Vst3ViewRect {
  //ViewRect(int32 l=0, int32 t=0, int32 r=0, int32 b=0)
  //: left(l), top(t), right(r), bottom(b) {}
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
  //int32 getWidth() const { return right - left; }
  //int32 getHeight() const { return bottom - top; }
};

//----------------------------------------------------------------------

struct KODE_Vst3NoteOnEvent {
  int16_t channel;
  int16_t pitch;
  float   tuning;
  float   velocity;
  int32_t length;
  int32_t noteId;
};

struct KODE_Vst3NoteOffEvent {
  int16_t channel;
  int16_t pitch;
  float   velocity;
  int32_t noteId;
  float   tuning;
};

struct KODE_Vst3DataEvent {
  enum DataTypes {
    kode_vst3_MidiSysEx = 0
  };
  //
  uint32_t        size;
  uint32_t        type;
  const uint8_t*  bytes;
};

struct KODE_Vst3PolyPressureEvent {
  int16_t channel;
  int16_t pitch;
  float   pressure;
  int32_t noteId;
};

struct KODE_Vst3ChordEvent {
  int16_t         root;
  int16_t         bassNote;
  int16_t         mask;
  uint16_t        textLen;
  const char16_t* text;
};

struct KODE_Vst3ScaleEvent {
  int16_t         root;
  int16_t         mask;
  uint16_t        textLen;
  const char16_t* text;
};

struct KODE_Vst3NoteExpressionValueEvent {
  uint32_t  typeId;
  int32_t   noteId;
  double    value;
};

struct KODE_Vst3NoteExpressionTextEvent {
  uint32_t        typeId;
  int32_t         noteId;
  uint32_t        textLen;
  const char16_t* text;
};

struct KODE_Vst3Event {
  enum EventFlags {
    kode_vst3_IsLive        = 1 << 0,
    kode_vst3_UserReserved1 = 1 << 14,
    kode_vst3_UserReserved2 = 1 << 15
  };
  enum EventTypes
  {
    kode_vst3_NoteOnEvent = 0,
    kode_vst3_NoteOffEvent,
    kode_vst3_DataEvent,
    kode_vst3_PolyPressureEvent,
    kode_vst3_NoteExpressionValueEvent,
    kode_vst3_NoteExpressionTextEvent,
    kode_vst3_ChordEvent,
    kode_vst3_ScaleEvent
  };
  //
  int32_t   busIndex;
  int32_t   sampleOffset;
  double    ppqPosition;
  uint16_t  flags;
  uint16_t  type;
  union {
    KODE_Vst3NoteOnEvent noteOn;
    KODE_Vst3NoteOffEvent noteOff;
    KODE_Vst3DataEvent data;
    KODE_Vst3PolyPressureEvent polyPressure;
    KODE_Vst3NoteExpressionValueEvent noteExpressionValue;
    KODE_Vst3NoteExpressionTextEvent noteExpressionText;
    KODE_Vst3ChordEvent chord;
    KODE_Vst3ScaleEvent scale;
  };
};

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

class KODE_Vst3FUnknown {
public:
	virtual int32_t   KODE_VST3_PLUGIN_API queryInterface (const KODE_Vst3Id _iid, void** obj) = 0;
	virtual uint32_t  KODE_VST3_PLUGIN_API addRef () = 0;
	virtual uint32_t  KODE_VST3_PLUGIN_API release () = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID(KODE_Vst3FUnknown, 0x00000000, 0x00000000, 0xC0000000, 0x00000046)

//----------

class KODE_Vst3IBStream
: public KODE_Vst3FUnknown {
public:
  enum IStreamSeekMode {
    kode_vst3_IBSeekSet = 0,
    kode_vst3_IBSeekCur,
    kode_vst3_IBSeekEnd
  };
  //
  virtual int32_t KODE_VST3_PLUGIN_API read(void* buffer, int32_t numBytes, int32_t* numBytesRead = 0) = 0;
  virtual int32_t KODE_VST3_PLUGIN_API write(void* buffer, int32_t numBytes, int32_t* numBytesWritten = 0) = 0;
  virtual int32_t KODE_VST3_PLUGIN_API seek(int64_t pos, int32_t mode, int64_t* result = 0) = 0;
  virtual int32_t KODE_VST3_PLUGIN_API tell(int64_t* pos) = 0;
	//
  static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID(KODE_Vst3IBStream,0xC3BF6EA2,0x30994752,0x9B6BF990,0x1EE33E9B)

//----------

class KODE_Vst3IPluginFactory
: public KODE_Vst3FUnknown {
public:
  virtual int32_t  KODE_VST3_PLUGIN_API getFactoryInfo(KODE_Vst3PFactoryInfo* info) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API countClasses() = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API getClassInfo (int32_t index, KODE_Vst3PClassInfo* info) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API createInstance(const char* cid, const char* _iid, void** obj) = 0;
	//
  static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID(KODE_Vst3IPluginFactory, 0x7A4D811C, 0x52114A1F, 0xAED9D2EE, 0x0B43BF9F)

//----------

class KODE_Vst3IPluginFactory2
: public KODE_Vst3IPluginFactory {
public:
	virtual int32_t KODE_VST3_PLUGIN_API getClassInfo2 (int32_t index, KODE_Vst3PClassInfo2* info) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IPluginFactory2, 0x0007B650, 0xF24B4C0B, 0xA464EDB9, 0xF00B2ABB)

//----------

class KODE_Vst3IPluginFactory3
: public KODE_Vst3IPluginFactory2 {
public:
	virtual int32_t KODE_VST3_PLUGIN_API getClassInfoUnicode(int32_t index, KODE_Vst3PClassInfoW* info) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API setHostContext(KODE_Vst3FUnknown* context) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IPluginFactory3, 0x4555A2AB, 0xC1234E57, 0x9B122910, 0x36878931)

//----------

class KODE_Vst3IPluginBase
: public KODE_Vst3FUnknown {
public:
  virtual int32_t  KODE_VST3_PLUGIN_API initialize(KODE_Vst3FUnknown* context) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API terminate() = 0;
	//
  static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID(KODE_Vst3IPluginBase,0x22888DDB,0x156E45AE,0x8358B348,0x08190625)

//----------

class KODE_Vst3IComponent
: public KODE_Vst3IPluginBase {
public:
  virtual int32_t  KODE_VST3_PLUGIN_API getControllerClassId(KODE_Vst3Id classId) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API setIoMode(int32_t mode) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API getBusCount(int32_t type, int32_t dir) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API getBusInfo(int32_t type, int32_t dir, int32_t index, KODE_Vst3BusInfo& bus) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API getRoutingInfo(KODE_Vst3RoutingInfo& inInfo, KODE_Vst3RoutingInfo& outInfo) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API activateBus(int32_t type, int32_t dir, int32_t index, uint8_t state) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API setActive(uint8_t state) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API setState(KODE_Vst3IBStream* state) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API getState(KODE_Vst3IBStream* state) = 0;
	//
  static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID(KODE_Vst3IComponent,0xE831FF31,0xF2D54301,0x928EBBEE,0x25697802)

//----------

class KODE_Vst3IAudioProcessor
: public KODE_Vst3FUnknown {
public:
  virtual int32_t  KODE_VST3_PLUGIN_API setBusArrangements(uint64_t* inputs, int32_t numIns, uint64_t* outputs, int32_t numOuts) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API getBusArrangement(int32_t dir, int32_t index, uint64_t& arr) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API canProcessSampleSize(int32_t symbolicSampleSize) = 0;
  virtual uint32_t KODE_VST3_PLUGIN_API getLatencySamples() = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API setupProcessing(KODE_Vst3ProcessSetup& setup) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API setProcessing(uint8_t state) = 0;
  virtual int32_t  KODE_VST3_PLUGIN_API process(KODE_Vst3ProcessData& data) = 0;
  virtual uint32_t KODE_VST3_PLUGIN_API getTailSamples() = 0;
	//
  static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID(KODE_Vst3IAudioProcessor,0x42043F99,0xB7DA453C,0xA569E79D,0x9AAEC33D)

//----------

class KODE_Vst3IUnitInfo
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API getUnitCount() = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getUnitInfo(int32_t unitIndex, KODE_Vst3UnitInfo& info /*out*/) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getProgramListCount() = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getProgramListInfo(int32_t listIndex, KODE_Vst3ProgramListInfo& info /*out*/) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getProgramName(int32_t listId, int32_t programIndex, KODE_Vst3String name /*out*/) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getProgramInfo(int32_t listId, int32_t programIndex, const char* attributeId /*in*/, KODE_Vst3String attributeValue /*out*/) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API hasProgramPitchNames(int32_t listId, int32_t programIndex) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getProgramPitchName(int32_t listId, int32_t programIndex, int16_t midiPitch, KODE_Vst3String name /*out*/) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getSelectedUnit() = 0;
	virtual int32_t KODE_VST3_PLUGIN_API selectUnit(int32_t unitId) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getUnitByBus(int32_t type, int32_t dir, int32_t busIndex, int32_t channel, int32_t& unitId /*out*/) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API setUnitProgramData(int32_t listOrUnitId, int32_t programIndex, KODE_Vst3IBStream* data) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IUnitInfo, 0x3D4BD6B5, 0x913A4FD2, 0xA886E768, 0xA5EB92C1)

//----------

class KODE_Vst3IAttributeList
: public KODE_Vst3FUnknown {
public:
	//typedef const char* AttrID;
	virtual int32_t KODE_VST3_PLUGIN_API setInt(/*AttrID*/const char* id, int64_t value) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getInt(/*AttrID*/const char* id, int64_t& value) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API setFloat(/*AttrID*/const char* id, double value) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getFloat(/*AttrID*/const char* id, double& value) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API setString(/*AttrID*/const char* id, const char16_t* string) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getString(/*AttrID*/const char* id, char16_t* string, uint32_t size) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API setBinary(/*AttrID*/const char* id, const void* data, uint32_t size) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getBinary(/*AttrID*/const char* id, const void*& data, uint32_t& size) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IAttributeList, 0x1E5F0AEB, 0xCC7F4533, 0xA2544011, 0x38AD5EE4)

//----------

class KODE_Vst3IMessage
: public KODE_Vst3FUnknown {
public:
	virtual const char*               KODE_VST3_PLUGIN_API getMessageID () = 0;
	virtual void                      KODE_VST3_PLUGIN_API setMessageID (const char* id /*in*/) = 0;
	virtual KODE_Vst3IAttributeList*  KODE_VST3_PLUGIN_API getAttributes () = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IMessage, 0x936F033B, 0xC6C047DB, 0xBB0882F8, 0x13C1E613)

//----------

class KODE_Vst3IConnectionPoint
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API connect(KODE_Vst3IConnectionPoint* other) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API disconnect(KODE_Vst3IConnectionPoint* other) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API notify(KODE_Vst3IMessage* message) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IConnectionPoint, 0x70A4156F, 0x6E6E4026, 0x989148BF, 0xAA60D8D1)

//----------

class KODE_Vst3IMidiMapping
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API getMidiControllerAssignment (int32_t busIndex, int16_t channel, int16_t midiControllerNumber, uint32_t& id/*out*/) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IMidiMapping, 0xDF0FF9F7, 0x49B74669, 0xB63AB732, 0x7ADBF5E5)

//----------

class KODE_Vst3IKeyswitchController
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API getKeyswitchCount (int32_t busIndex, int16_t channel) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getKeyswitchInfo (int32_t busIndex, int16_t channel, int32_t keySwitchIndex, KODE_Vst3KeyswitchInfo& info /*out*/) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IKeyswitchController, 0x1F2F76D3, 0xBFFB4B96, 0xB99527A5, 0x5EBCCEF4)

//----------

//INoteExpressionController

//----------

class KODE_Vst3IComponentHandler
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API beginEdit (uint32_t id) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API performEdit (uint32_t id, double valueNormalized) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API endEdit (uint32_t id) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API restartComponent (int32_t flags) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IComponentHandler, 0x93A0BEA3, 0x0BD045DB, 0x8E890B0C, 0xC1E46AC6)

//----------

class KODE_Vst3IComponentHandler2
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API setDirty (uint8_t state) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API requestOpenEditor (const char* name = /*ViewType::*/kode_vst3_Editor) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API startGroupEdit () = 0;
	virtual int32_t KODE_VST3_PLUGIN_API finishGroupEdit () = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IComponentHandler2, 0xF040B4B3, 0xA36045EC, 0xABCDC045, 0xB4D5A2CC)

//----------

class KODE_Vst3IPlugFrame;

class KODE_Vst3IPlugView
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API isPlatformTypeSupported (const char* type) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API attached (void* parent, const char* type) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API removed () = 0;
	virtual int32_t KODE_VST3_PLUGIN_API onWheel (float distance) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API onKeyDown (char16_t key, int16_t keyCode, int16_t modifiers) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API onKeyUp (char16_t key, int16_t keyCode, int16_t modifiers) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getSize (KODE_Vst3ViewRect* size) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API onSize (KODE_Vst3ViewRect* newSize) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API onFocus (uint8_t state) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API setFrame (KODE_Vst3IPlugFrame* frame) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API canResize () = 0;
	virtual int32_t KODE_VST3_PLUGIN_API checkSizeConstraint (KODE_Vst3ViewRect* rect) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IPlugView, 0x5BC32507, 0xD06049EA, 0xA6151B52, 0x2B755B29)

//----------

class KODE_Vst3IPlugFrame
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API resizeView(KODE_Vst3IPlugView* view, KODE_Vst3ViewRect* newSize) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IPlugFrame, 0x367FAF01, 0xAFA94693, 0x8D4DA2A0, 0xED0882A3)

//----------

#define setState setEditorState
#define getState getEditorState

class KODE_Vst3IEditController
: public KODE_Vst3IPluginBase {
public:
	virtual int32_t             KODE_VST3_PLUGIN_API setComponentState (KODE_Vst3IBStream* state) = 0;
	virtual int32_t             KODE_VST3_PLUGIN_API setState (KODE_Vst3IBStream* state) = 0;
	virtual int32_t             KODE_VST3_PLUGIN_API getState (KODE_Vst3IBStream* state) = 0;
	virtual int32_t             KODE_VST3_PLUGIN_API getParameterCount () = 0;
	virtual int32_t             KODE_VST3_PLUGIN_API getParameterInfo (int32_t paramIndex, KODE_Vst3ParameterInfo& info /*out*/) = 0;
	virtual int32_t             KODE_VST3_PLUGIN_API getParamStringByValue (uint32_t id, double valueNormalized /*in*/, KODE_Vst3String string /*out*/) = 0;
	virtual int32_t             KODE_VST3_PLUGIN_API getParamValueByString (uint32_t id, char16_t* string /*in*/, double& valueNormalized /*out*/) = 0;
	virtual double              KODE_VST3_PLUGIN_API normalizedParamToPlain (uint32_t id, double valueNormalized) = 0;
	virtual double              KODE_VST3_PLUGIN_API plainParamToNormalized (uint32_t id, double plainValue) = 0;
	virtual double              KODE_VST3_PLUGIN_API getParamNormalized (uint32_t id) = 0;
	virtual int32_t             KODE_VST3_PLUGIN_API setParamNormalized (uint32_t id, double value) = 0;
	virtual int32_t             KODE_VST3_PLUGIN_API setComponentHandler (KODE_Vst3IComponentHandler* handler) = 0;
	virtual KODE_Vst3IPlugView* KODE_VST3_PLUGIN_API createView (const char* name) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IEditController, 0xDCD7BBE3, 0x7742448D, 0xA874AACC, 0x979C759E)

#undef setState
#undef getState

//----------

class KODE_Vst3IEditController2
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API setKnobMode (int32_t mode) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API openHelp (uint8_t onlyCheck) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API openAboutBox (uint8_t onlyCheck) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IEditController2, 0x7F4EFE59, 0xF3204967, 0xAC27A3AE, 0xAFB63038)

//----------

class KODE_Vst3ITimerHandler
: public KODE_Vst3FUnknown {
public:
	virtual void KODE_VST3_PLUGIN_API onTimer () = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3ITimerHandler, 0x10BDD94F, 0x41424774, 0x821FAD8F, 0xECA72CA9)

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------

class KODE_Vst3IHostApplication
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API getName (KODE_Vst3String name) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API createInstance (KODE_Vst3Id cid, KODE_Vst3Id _iid, void** obj) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IHostApplication, 0x58E595CC, 0xDB2D4969, 0x8B6AAF8C, 0x36A664E5)

//----------

class KODE_Vst3IEventHandler
: public KODE_Vst3FUnknown {
public:
	virtual void KODE_VST3_PLUGIN_API onFDIsSet (int fd) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IEventHandler, 0x561E65C9, 0x13A0496F, 0x813A2C35, 0x654D7983)

//----------

class KODE_Vst3IRunLoop
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API registerEventHandler (KODE_Vst3IEventHandler* handler, int fd) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API unregisterEventHandler (KODE_Vst3IEventHandler* handler) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API registerTimer (KODE_Vst3ITimerHandler* handler, uint64_t milliseconds) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API unregisterTimer (KODE_Vst3ITimerHandler* handler) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IRunLoop, 0x18C35366, 0x97764F1A, 0x9C5B8385, 0x7A871389)

//----------

class KODE_Vst3IParamValueQueue
: public KODE_Vst3FUnknown {
public:
	virtual uint32_t  KODE_VST3_PLUGIN_API getParameterId () = 0;
	virtual int32_t   KODE_VST3_PLUGIN_API getPointCount () = 0;
	virtual int32_t   KODE_VST3_PLUGIN_API getPoint (int32_t index, int32_t& sampleOffset /*out*/, double& value /*out*/) = 0;
	virtual int32_t   KODE_VST3_PLUGIN_API addPoint (int32_t sampleOffset, double value, int32_t& index /*out*/) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IParamValueQueue, 0x01263A18, 0xED074F6F, 0x98C9D356, 0x4686F9BA)

//----------

class KODE_Vst3IParameterChanges
: public KODE_Vst3FUnknown {
public:
	virtual int32_t                     KODE_VST3_PLUGIN_API getParameterCount () = 0;
	virtual KODE_Vst3IParamValueQueue*  KODE_VST3_PLUGIN_API getParameterData (int32_t index) = 0;
	virtual KODE_Vst3IParamValueQueue*  KODE_VST3_PLUGIN_API addParameterData (const uint32_t& id, int32_t& index /*out*/) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IParameterChanges, 0xA4779663, 0x0BB64A56, 0xB44384A8, 0x466FEB9D)

//----------

class KODE_Vst3IEventList
: public KODE_Vst3FUnknown {
public:
	virtual int32_t KODE_VST3_PLUGIN_API getEventCount () = 0;
	virtual int32_t KODE_VST3_PLUGIN_API getEvent (int32_t index, KODE_Vst3Event& e /*out*/) = 0;
	virtual int32_t KODE_VST3_PLUGIN_API addEvent (KODE_Vst3Event& e /*in*/) = 0;
	//
	static const KODE_Vst3Id iid;
};

KODE_VST3_DECLARE_CLASS_IID (KODE_Vst3IEventList, 0x3A2C4214, 0x346349FE, 0xB2C4F397, 0xB9695A44)

//----------------------------------------------------------------------
#endif
