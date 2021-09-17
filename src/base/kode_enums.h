#ifndef kode_enums_included
#define kode_enums_included
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// general
//
//----------------------------------------------------------------------

enum KODE_EDirections {
  KODE_UP                         = 0,
  KODE_DOWN                       = 1,
  KODE_LEFT                       = 2,
  KODE_RIGHT                      = 3,
  KODE_HORIZONTAL                 = 0,
  KODE_VERTICAL                   = 1
};

enum KODE_EEdges {
  KODE_EDGE_NONE                  = 0,
  KODE_EDGE_TOP                   = 1,
  KODE_EDGE_BOTTOM                = 2,
  KODE_EDGE_LEFT                  = 4,
  KODE_EDGE_RIGHT                 = 8,
  KODE_EDGES_ALL                  = 15
};

enum KODE_ECorners {
  KODE_CORNER_NONE                = 0,
  KODE_CORNER_LEFT_TOP            = 1,
  KODE_CORNER_LEFT_BOTTOM         = 2,
  KODE_CORNER_RIGHT_TOP           = 4,
  KODE_CORNER_RIGHT_BOTTOM        = 8
};

//----------------------------------------------------------------------
//
// base
//
//----------------------------------------------------------------------

enum KODE_EThreadIds {
  KODE_THREAD_ID_NONE             = 0,
  KODE_THREAD_ID_KILL             = 666,
  KODE_THREAD_ID_TIMER            = 667,
  KODE_THREAD_ID_IDLE             = 668
};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

enum KODE_EParameterUpdateModes {
  KODE_PARAMETER_UPDATE_NORMAL    = 0,
  KODE_PARAMETER_UPDATE_ALL       = 1//,
  //KODE_PARAMETER_UPDATE_PROCESS   = 2
};

enum KODE_EPluginEventTypes {
  KODE_PLUGIN_EVENT_NONE          = 0,
  KODE_PLUGIN_EVENT_PARAMETER     = 1,
  KODE_PLUGIN_EVENT_MIDI          = 2
};

enum KODE_EPluginPlayStates {
  KODE_PLUGIN_PLAYSTATE_NONE      = 0,
  KODE_PLUGIN_PLAYSTATE_PLAYING   = 1,
  KODE_PLUGIN_PLAYSTATE_RECORDING = 2,
  KODE_PLUGIN_PLAYSTATE_LOOPING   = 4
};

enum KODE_EPluginStates {
  KODE_PLUGIN_STATE_NONE          = 0,
  KODE_PLUGIN_STATE_OPEN          = 1,
  KODE_PLUGIN_STATE_INITIALIZE    = 2,
  KODE_PLUGIN_STATE_ACTIVATE      = 3,
  KODE_PLUGIN_STATE_DEACTIVATE    = 4,
  KODE_PLUGIN_STATE_TERMINATE     = 5,
  KODE_PLUGIN_STATE_CLOSE         = 6
};

//----------------------------------------------------------------------
//
// gui
//
//----------------------------------------------------------------------

enum KODE_EMouseCursors {
  KODE_CURSOR_GRAB                = -4,
  KODE_CURSOR_RELEASE             = -3,
  KODE_CURSOR_SHOW                = -2,
  KODE_CURSOR_HIDE                = -1,
  KODE_CURSOR_DEFAULT             = 0,
  KODE_CURSOR_ARROW               = 1,
  KODE_CURSOR_ARROWUP             = 2,
  KODE_CURSOR_ARROWDOWN           = 3,
  KODE_CURSOR_ARROWLEFT           = 4,
  KODE_CURSOR_ARROWRIGHT          = 5,
  KODE_CURSOR_ARROWUPDOWN         = 6,
  KODE_CURSOR_ARROWLEFTRIGHT      = 7,
  KODE_CURSOR_ARROWDIAGLEFT       = 8,
  KODE_CURSOR_ARROWDIAGRIGHT      = 9,
  KODE_CURSOR_MOVE                = 10,
  KODE_CURSOR_WAIT                = 11,
  KODE_CURSOR_ARROWWAIT           = 12,
  KODE_CURSOR_HAND                = 13,
  KODE_CURSOR_FINGER              = 14,
  KODE_CURSOR_CROSS               = 15,
  KODE_CURSOR_PENCIL              = 16,
  KODE_CURSOR_PLUS                = 17,
  KODE_CURSOR_QUESTION            = 18,
  KODE_CURSOR_IBEAM               = 19
};

enum KODE_EMouseButtons {
  KODE_BUTTON_NONE                = 0,
  KODE_BUTTON_LEFT                = 1,
  KODE_BUTTON_MIDDLE              = 2,
  KODE_BUTTON_RIGHT               = 3,
  KODE_BUTTON_SCROLL_UP           = 4,
  KODE_BUTTON_SCROLL_DOWN         = 5,
  KODE_BUTTON_BACKWARD            = 8,
  KODE_BUTTON_FORWARD             = 9
};

enum KODE_EKeyState {
  KODE_KEY_NONE                   = 0,
  KODE_KEY_SHIFT                  = 1,
  KODE_KEY_CAPS                   = 2,
  KODE_KEY_CTRL                   = 4,
  KODE_KEY_ALT                    = 8,
  KODE_KEY_ALTGR                  = 16
};

enum KODE_EKeyChar {
  KODE_KEY_ENTER      = 13,
  KODE_KEY_ESC        = 27,
  KODE_KEY_HOME       = 36,
  KODE_KEY_END        = 35,
  KODE_KEY_LEFT       = 37,
  KODE_KEY_RIGHT      = 39,
  KODE_KEY_DELETE     = 46,
  KODE_KEY_BACKSPACE  = 8
};

enum KODE_EWidgetPaintModes {
  KODE_WIDGET_PAINT_NORMAL        = 0,
  KODE_WIDGET_PAINT_VALUE         = 1
};


enum KODE_EWidgetAlignment {
  KODE_WIDGET_ALIGN_NONE                = 0,  // relative to window
  KODE_WIDGET_ALIGN_PARENT              = 1,  // relative to parent
  KODE_WIDGET_ALIGN_CLIENT              = 2,  // relative to current client area
  KODE_WIDGET_ALIGN_LEFT                = 3,
  KODE_WIDGET_ALIGN_RIGHT               = 4,
  KODE_WIDGET_ALIGN_TOP                 = 5,
  KODE_WIDGET_ALIGN_BOTTOM              = 6,
  KODE_WIDGET_ALIGN_LEFT_TOP            = 7,
  KODE_WIDGET_ALIGN_LEFT_CENTER         = 8,
  KODE_WIDGET_ALIGN_LEFT_BOTTOM         = 9,
  KODE_WIDGET_ALIGN_RIGHT_TOP           = 10,
  KODE_WIDGET_ALIGN_RIGHT_CENTER        = 11,
  KODE_WIDGET_ALIGN_RIGHT_BOTTOM        = 12,
  KODE_WIDGET_ALIGN_TOP_LEFT            = 13,
  KODE_WIDGET_ALIGN_TOP_CENTER          = 14,
  KODE_WIDGET_ALIGN_TOP_RIGHT           = 15,
  KODE_WIDGET_ALIGN_BOTTOM_LEFT         = 16,
  KODE_WIDGET_ALIGN_BOTTOM_CENTER       = 17,
  KODE_WIDGET_ALIGN_BOTTOM_RIGHT        = 18,
  KODE_WIDGET_ALIGN_FILL_CLIENT         = 19,
  KODE_WIDGET_ALIGN_FILL_LEFT           = 20,
  KODE_WIDGET_ALIGN_FILL_LEFT_TOP       = 21,
  KODE_WIDGET_ALIGN_FILL_LEFT_CENTER    = 22,
  KODE_WIDGET_ALIGN_FILL_LEFT_BOTTOM    = 23,
  KODE_WIDGET_ALIGN_FILL_RIGHT          = 24,
  KODE_WIDGET_ALIGN_FILL_RIGHT_TOP      = 25,
  KODE_WIDGET_ALIGN_FILL_RIGHT_CENTER   = 26,
  KODE_WIDGET_ALIGN_FILL_RIGHT_BOTTOM   = 27,
  KODE_WIDGET_ALIGN_FILL_TOP            = 28,
  KODE_WIDGET_ALIGN_FILL_TOP_LEFT       = 29,
  KODE_WIDGET_ALIGN_FILL_TOP_CENTER     = 30,
  KODE_WIDGET_ALIGN_FILL_TOP_RIGHT      = 31,
  KODE_WIDGET_ALIGN_FILL_BOTTOM         = 32,
  KODE_WIDGET_ALIGN_FILL_BOTTOM_LEFT    = 33,
  KODE_WIDGET_ALIGN_FILL_BOTTOM_CENTER  = 34,
  KODE_WIDGET_ALIGN_FILL_BOTTOM_RIGHT   = 35,
  KODE_WIDGET_ALIGN_CENTER              = 36,
  KODE_WIDGET_ALIGN_CENTER_HORIZ        = 37,
  KODE_WIDGET_ALIGN_CENTER_VERT         = 38,
  KODE_WIDGET_ALIGN_STACK_HORIZ         = 39,
  KODE_WIDGET_ALIGN_STACK_VERT          = 40

};

//enum KODE_EWidgetAnchors {
//  KODE_WIDGET_ANCHOR_NONE         = 0,
//  KODE_WIDGET_ANCHOR_LEFT         = 1,
//  KODE_WIDGET_ANCHOR_TOP          = 2,
//  KODE_WIDGET_ANCHOR_RIGHT        = 4,
//  KODE_WIDGET_ANCHOR_BOTTOM       = 8
//};

//enum KODE_EWidgetTypes {
//  KODE_WIDGET_TYPE_NONE           = 0,
//  KODE_WIDGET_TYPE_WIDGET         = 1,
//  KODE_WIDGET_TYPE_WINDOW         = 2
//};

//----------------------------------------------------------------------
//
// painting
//
//----------------------------------------------------------------------

enum KODE_EFontSlants {
  KODE_FONT_SLANT_NORMAL          = 0,
  KODE_FONT_SLANT_ITALIC          = 1,
  KODE_FONT_SLANT_OBLIQUE         = 2
};

enum KODE_EFfontWeights {
  KODE_FONT_WEIGHT_NORMAL         = 0,
  KODE_FONT_WEIGHT_BOLD           = 1
};

enum KODE_ELineCaps {
  KODE_LINE_CAP_BUTT              = 0,
  KODE_LINE_CAP_ROUND             = 1,
  KODE_LINE_CAP_SQUARE            = 2
};

enum KODE_ELineJoins {
  KODE_LINE_JOIN_MITER            = 0,
  KODE_LINE_JOIN_ROUND            = 1,
  KODE_LINE_JOIN_BEVEL            = 2
};

enum KODE_ESymbols {
  KODE_SYMBOL_NONE                = 0,
  KODE_SYMBOL_HLINE               = 1,
  KODE_SYMBOL_VLINE               = 2,
  KODE_SYMBOL_LINE                = 3,
  KODE_SYMBOL_RECT                = 4,
  KODE_SYMBOL_FILL_RECT           = 5,
  KODE_SYMBOL_TRI_DOWN            = 6,
  KODE_SYMBOL_FILL_TRI_DOWN       = 7,
  KODE_SYMBOL_ELLIPSE             = 8,
  KODE_SYMBOL_FILL_ELLIPSE        = 9
};

enum KODE_ETextAlignment {
  KODE_TEXT_ALIGN_NONE            = 0,
  KODE_TEXT_ALIGN_TOP             = 1,
  KODE_TEXT_ALIGN_BOTTOM          = 2,
  KODE_TEXT_ALIGN_LEFT            = 4,
  KODE_TEXT_ALIGN_RIGHT           = 8,
  KODE_TEXT_ALIGN_CENTER          = 0
};

//----------------------------------------------------------------------

//----------------------------------------------------------------------
#endif
