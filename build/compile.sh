#!/bin/bash

#g++
#-Wall
#-std=c++11
#-m64
#-fexceptions
#-Wl,--as-needed
#-O3
#-O2
#-fPIC
#-Wl,--as-needed
#-faligned-new
#-msse4
#-mfpmath=sse
#-DKODE_PLUGIN_VST3
#-I../../src
#-c compile.cpp
#-o compile.o

#g++
#-shared
#compile.o
#-o plugin.so
#-m64
#-s
#-fPIE
#-lrt
#-lpthread
#-lxcb
#-lxcb-util
#-lxcb-image
#-lxcb-cursor
#-lxcb-keysyms
#-lcairo
#-lportaudio
#-lSDL2
#-lSDL2_image
#-lSDL2_mixer

#TODO

# --------------------------------------------------
# default values
# --------------------------------------------------

#ARCH=64
DEBUG="off"
#GPL="off"
FORMAT="vst3"
GUI="nogui"
PAINTER="nopainter"
#FLAGS="-Wall "
DEF=""
INC="-I../src "
LIB=""
POST="-s -lstdc++ -lm "

# --------------------------------------------------
# help
# --------------------------------------------------

function print_help {
  echo "usage: ./compile <flags>"
  echo "  -i input file name/path"
  echo "  -o output file name/path"
  echo "  -f format (exe, ladspa, dssi, lv2, vst2, VST3)"
  echo "  -g gui (NOGUI,xcb)"
  echo "  -p painter (NOPAINTER,cairo)"
#  echo "  -a architecture (32/64)"
#  echo "  -v version (v1)"
  echo "  -d debug"
#  echo "  -h this help"
#  echo "  -F additional gcc flags"
#  echo "  -G use gpl"
#  echo "  -D additional defines"
#  echo "  -I additional include directories"
#  echo "  -L additional linked libraries"
  exit 1
}

# --------------------------------------------------
# parse command line
# --------------------------------------------------

if [ "$#" -eq 0 ]; then
  print_help
fi

#while getopts i:o:t:g:a:v:dhF:GD:I:L: opt
while getopts i:o:f:g:p:d opt
do
  case "$opt" in
    i) INPUT="$OPTARG";;
    o) OUTPUT="$OPTARG";;
    f) FORMAT="$OPTARG";;
    g) GUI="$OPTARG";;
    g) PAINTER="$OPTARG";;
    d) DEBUG="on";;
#    a) ARCH="$OPTARG";;
#    v) VERSION="$OPTARG";;
#    h) print_help;;
#    F) FLAGS+="$OPTARG ";;
#    G) GPL="on";;
#    D) DEF+="-D$OPTARG ";;
#    I) INC+="-I$OPTARG ";;
#    L) LIB+="-l$OPTARG ";;
  esac
done

# --------------------------------------------------
# paths..
# --------------------------------------------------

INPUT_FILE=${INPUT##*/}
INPUT_BASE=${INPUT_FILE%%.*}
INPUT_EXT=${INPUT_FILE##*.}
INPUT_DIR=${INPUT%$INPUT_FILE}

echo ""
echo "INPUT     : $INPUT"
echo "INPUT_FILE: $INPUT_FILE"
echo "INPUT_BASE: $INPUT_BASE"
echo "INPUT_EXT : $INPUT_EXT"
echo "INPUT_DIR : $INPUT_DIR"

# input = ../plugins/fx_pitch.h"
# input_file = fx_pitch.h
# input_base = fx_pitch
# input_ext = h
# input_dir = ../plugins/

OUTPUT_FILE=${OUTPUT##*/}
OUTPUT_BASE=${OUTPUT_FILE%%.*}
OUTPUT_EXT=${OUTPUT_FILE##*.}
OUTPUT_DIR=${OUTPUT%$OUTPUT_FILE}

echo ""
echo "OUTPUT     : $OUTPUT"
echo "OUTPUT_FILE: $OUTPUT_FILE"
echo "OUTPUT_BASE: $OUTPUT_BASE"
echo "OUTPUT_EXT : $OUTPUT_EXT"
echo "OUTPUT_DIR : $OUTPUT_DIR"
# --------------------------------------------------
#
# --------------------------------------------------

#if [ "$OUTPUT" = "" ]; then
#  OUTPUT=$INPUT_BASE
#fi

# ----------

if [ "$DEBUG" = "on" ]; then
  DEF+="-DKODE_DEBUG "
else
  FLAGS="-O2 "
fi

# ----------

#if [ "$GPL" = "on" ]; then
#  INC+="-I../plugins/gpl "
#fi

# ----------

if [ "$TYPE" = "exe" ]; then
  DEF+="-DKODE_PLUGIN_EXE "
fi

if [ "$TYPE" = "ladspa" ]; then
  FLAGS+="-fPIC -shared "
  DEF+="-DKODE_PLUGIN_LADSPA "
fi

if [ "$TYPE" = "dssi" ]; then
  FLAGS+="-fPIC -shared "
  DEF+="-DKODE_PLUGIN_DSSI "
fi

if [ "$TYPE" = "lv2" ]; then
  FLAGS+="-fPIC -shared "
  DEF+="-DKODE_PLUGIN_LV2 "
fi

if [ "$TYPE" = "vst2" ]; then
  FLAGS+="-fPIC -shared "
  DEF+="-DKODE_PLUGIN_VST2 "
fi

if [ "$TYPE" = "vst3" ]; then
  FLAGS+="-fPIC -shared "
  DEF+="-DKODE_PLUGIN_VST3 "
fi

# ----------

#if [ "$ARCH" = "32" ]; then
#  FLAGS+="-m32 "
#fi

#if [ "$ARCH" = "64" ]; then
#  FLAGS+="-m64 "
#fi

# ----------

if [ "$GUI" = "nogui" ]; then
  DEF+="-DKODE_NO_GUI "
fi

if [ "$GUI" = "xcb" ]; then
  DEF+="-DKODE_GUI_XCB "
  LIB+="-lx11 -lxrender -lxft "
fi

# ----------

if [ "$PAINTER" = "nopainter" ]; then
  DEF+="-DKODE_NO_PAINTER "
fi

if [ "$PAINTER" = "cairo" ]; then
  DEF+="-DKODE_PAINTER_CAIRO "
#  LIB+="-lx11 -lxrender -lxft "
fi

if [ "$PAINTER" = "xcb" ]; then
  DEF+="-DKODE_PAINTER_XCB "
  LIB+="-lx11 -lxrender -lxft "
fi

# ----------

INC+="-I"
INC+=$INPUT_DIR

# --------------------------------------------------
# printout
# --------------------------------------------------

#echo "----------"
#echo "input:" $INPUT
#echo "output:" $OUTPUT
#echo "debug:" $DEBUG
#echo "type:" $TYPE
#echo "gui:" $GUI
#echo "flags:" $FLAGS
#echo "def:" $DEF
#echo "inc:" $INC
#echo "lib:" $LIB

# --------------------------------------------------
# create compile.cpp
# --------------------------------------------------

# echo 'compile.cpp:'
echo '#include "base/kode.h"' >> compile.cpp
echo '#include "'$INPUT_FILE'"' >> compile.cpp

# --------------------------------------------------
# finalize command line
# --------------------------------------------------

CMDLINE=""
CMDLINE+=$FLAGS
CMDLINE+=$DEF
CMDLINE+=$INC
CMDLINE+=$LIB

echo ""
echo "compiling $INPUT_BASE"
echo "gcc" $CMDLINE compile.cpp -o $OUTPUT $POST
# gcc $CMDLINE compile.cpp -o $OUTPUT.so $POST

#rm compile.cpp

