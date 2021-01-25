#!/bin/bash

#TODO

# --------------------------------------------------
# default values
# --------------------------------------------------

ARCH=64
DEBUG="off"
GPL="off"
TYPE="vst"
GUI="nogui"
FLAGS="-Wall "
DEF=""
INC="-I../src "
POST="-s -lstdc++ -lm "

# --------------------------------------------------
# help
# --------------------------------------------------

function print_help {
  echo "usage: ./compile <flags>"
  echo "  -i input file name/path"
  echo "  -o output file name/path"
  echo "  -t type (VST,exe)"
  echo "  -g gui (NOGUI,x11,cairo)"
  echo "  -a architecture (32/64)"
  echo "  -v version (v1)"
  echo "  -d debug"
  echo "  -h this help"
  echo "  -F additional gcc flags"
  echo "  -G use gpl"
  echo "  -D additional defines"
  echo "  -I additional include directories"
  echo "  -L additional linked libraries"
  exit 1
}

# --------------------------------------------------
# parse command line
# --------------------------------------------------

if [ "$#" -eq 0 ]; then
  print_help
fi

while getopts i:o:t:g:a:v:dhF:GD:I:L: opt
do
  case "$opt" in
    i) INPUT="$OPTARG";;
    o) OUTPUT="$OPTARG";;
    t) TYPE="$OPTARG";;
    g) GUI="$OPTARG";;
    a) ARCH="$OPTARG";;
    v) VERSION="$OPTARG";;
    d) DEBUG="on";;
    h) print_help;;
    F) FLAGS+="$OPTARG ";;
    G) GPL="on";;
    D) DEF+="-D$OPTARG ";;
    I) INC+="-I$OPTARG ";;
    L) LIB+="-l$OPTARG ";;
  esac
done

# --------------------------------------------------
# paths..
# --------------------------------------------------

INPUT_FILE=${INPUT##*/}
INPUT_BASE=${INPUT_FILE%%.*}
INPUT_EXT=${INPUT_FILE##*.}
INPUT_DIR=${INPUT%$INPUT_FILE}

# input = ../plugins/fx_pitch.h"
# input_file = fx_pitch.h
# input_base = fx_pitch
# input_ext = h
# input_dir = ../plugins/

OUTPUT_FILE=${OUTPUT##*/}
OUTPUT_BASE=${OUTPUT_FILE%%.*}
OUTPUT_EXT=${OUTPUT_FILE##*.}
OUTPUT_DIR=${OUTPUT%$OUTPUT_FILE}

# --------------------------------------------------
#
# --------------------------------------------------

#if [ "$OUTPUT" = "" ]; then
#  OUTPUT=$INPUT_BASE
#fi

# ----------

if [ "$DEBUG" = "on" ]; then
  DEF+="-DS3_DEBUG "
else
  FLAGS="-O2 "
fi

# ----------

if [ "$GPL" = "on" ]; then
  INC+="-I../plugins/gpl "
fi

# ----------

if [ "$TYPE" = "exe" ]; then
  DEF+="-DS3_PLUGIN_EXE "
fi

if [ "$TYPE" = "vst" ]; then
  FLAGS+="-fPIC -shared "
  DEF+="-DS3_PLUGIN_VST "
fi

# ----------

if [ "$ARCH" = "32" ]; then
  FLAGS+="-m32 "
fi

if [ "$ARCH" = "64" ]; then
  FLAGS+="-m64 "
fi

# ----------

if [ "$GUI" = "nogui" ]; then
  DEF+="-DS3_NOGUI "
fi

if [ "$GUI" = "xlib" ]; then
  DEF+="-DS3_GUI_XLIB "
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
echo '#include "s3.h"' >> compile.cpp
echo '#include "'$INPUT_FILE'"' >> compile.cpp
# echo 'S3_MAIN(myPlugin)' >> compile.cpp

# --------------------------------------------------
# finalize command line
# --------------------------------------------------

CMDLINE=""
CMDLINE+=$FLAGS
CMDLINE+=$DEF
CMDLINE+=$INC
CMDLINE+=$LIB

echo "compiling $INPUT_BASE"
echo "gcc" $CMDLINE compile.cpp -o $OUTPUT $POST
# gcc $CMDLINE compile.cpp -o $OUTPUT.so $POST

rm compile.cpp

