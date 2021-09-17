
// nc -U -l -k /tmp/kode.socket
//#define KODE_DEBUG_PRINT_SOCKET
//#define KODE_DEBUG_PRINT_THREAD
//#define KODE_DEBUG_PRINT_TIME

//----------------------------------------------------------------------
//
// templates
//
//----------------------------------------------------------------------

//#include "../plugins/templates/effect_template.cpp"
//#include "../plugins/templates/effect_template2.cpp"
//#include "../plugins/templates/synth_template.cpp"

//----------------------------------------------------------------------
//
// plugins
//
//----------------------------------------------------------------------

#include "../plugins/sa_botage.cpp"

// fx_cutter          sa_botage
// fx_frac_delay
// fx_tempo_delay     sa_eculum
// fx_speech
// midi_automata
// midi_fibonacci     sa_cred

//----------------------------------------------------------------------

/*
  sa.bina/brina
  sa.ble
* sa.botage
  sa.bre
  sa.bulum - lat. gravel
  sa,cellum - lat. chapel
  sa.crament
  sa.cratus - lat. sacred
* sa.cred
  sa.crifice
  sa.crilege
  sa.crosanct
  sa.dist
* sa.eculum - lat. age, generation
  sa.epio - lat. to fence in, surround
  sa.evitia - lat. savageness
  sa.evus - lat. savage, violent
  sa.fari
  sa.fe, sa.fely, sa.fety
  sa.gacity
  sa.ge
  sa.gino - lat. to fatten
  sa.gitta - lat. arrow
  sa.ilor
  sa.int
  sa.lad
  sa.lamander
  sa.lary
  sa.lebra - lat. roughness
  sa.liaris - lat. splendid
  sa.lient
  sa.line
  sa.lio, sa.lium - lat. jump
  sa.liva
  sa.loon
  sa.lubris - lat. health-giving, beneficial
  sa.lum - lat. sea
  sa.lute
  sa.lvation
  sa.mantha
  sa.marithan
  sa.murai
  sa.natio - lat. cure
  sa.nitas - lat. health
  sa.nity
  sa.pidus - lat. tasty
  sa.pphire
  sa.tan
  sa.tellite
  sa.tire
  sa.tisfaction
  sa.turate
  sa.turn/us
  sa.tyr
  sa.usage
  sa.vage
  sa.vant
  sa.viour
  sa.vour


*/



//----------------------------------------------------------------------
//
// misc
//
//----------------------------------------------------------------------

//#include "../plugins/misc/sa_brickwall.cpp"
//#include "../plugins/misc/sa_curve_shaper.cpp"
//#include "../plugins/misc/sa_frac_delay.cpp"



//----------------------------------------------------------------------
//
// ports: jesusonic
//
//----------------------------------------------------------------------

//#include "../plugins/ports/jesusonic/sap_compciter.cpp"
//#include "../plugins/ports/jesusonic/sap_event_horizon.cpp"
//#include "../plugins/ports/jesusonic/sap_exciter.cpp"
//#include "../plugins/ports/jesusonic/sap_exciter2.cpp"
//#include "../plugins/ports/jesusonic/sap_hall_reverb.cpp"
//#include "../plugins/ports/jesusonic/sap_large_room_reverb.cpp"
//#include "../plugins/ports/jesusonic/sap_limiter.cpp"
//#include "../plugins/ports/jesusonic/sap_reverseness.cpp"
//#include "../plugins/ports/jesusonic/sap_saturation.cpp"
//#include "../plugins/ports/jesusonic/sap_simpverb.cpp"
//#include "../plugins/ports/jesusonic/sap_small_room_reverb.cpp"
//#include "../plugins/ports/jesusonic/sap_sonic_enhancer.cpp"
//#include "../plugins/ports/jesusonic/sap_sonic_maximizer.cpp"
//#include "../plugins/ports/jesusonic/sap_transient_killer.cpp"
//#include "../plugins/ports/jesusonic/sap_tube_harmonics_amp.cpp"

//----------------------------------------------------------------------
//
// ports: ladspa
//
//----------------------------------------------------------------------

//#include "../plugins/ports/ladspa/sap_cabinet.cpp"
//#include "../plugins/ports/ladspa/sap_vocoder.cpp"

//----------------------------------------------------------------------
//
// ports: other
//
//----------------------------------------------------------------------

//#include "../plugins/ports/other/sap_combo.cpp"                       // buggy
//#include "../plugins/ports/other/sap_dynamic.cpp"
//#include "../plugins/ports/other/sap_filter_ms20.cpp"
//#include "../plugins/ports/other/sap_hrtf.cpp"
//#include "../plugins/ports/other/sap_schroeder_reverb.cpp"
//#include "../plugins/ports/other/sap_tilteq.cpp"
//#include "../plugins/ports/other/sap_transient.cpp"
