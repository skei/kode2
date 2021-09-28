
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
// test
//
//----------------------------------------------------------------------

//#include "../test/test_widget.cpp"

//----------------------------------------------------------------------
//
// plugins
//
//----------------------------------------------------------------------

#include "../plugins/sa_botage/sa_botage.cpp"

// fx_cutter          sa_botage
// fx_frac_delay      sa_
// fx_tempo_delay     sa_eculum
// fx_speech          sa_ying
// midi_automata      sa_
// midi_fibonacci     sa_cred

//----------------------------------------------------------------------

/*
  sa.baoth
  sa.bel
  sa.bertooth
  sa.bina/brina
  sa.ble
  sa.botage, boteur
  sa.botasje, botør
  sa.bre
  sa.crament
  sa.cred
  sa.crifice
  sa.crilege
  sa.crosanct
  sa.del
  sa.dist
  sa.fari
  sa.fe, sa.fely, sa.fety
  sa.fir
  sa.ga
  sa.gacity
  sa.ge
  sa.gittarus
  sa.igon
  sa.ilor
  sa.int
  sa.ke
  sa.lad
  sa.lamander
  sa.lami
  sa.lary
  sa.lem
  sa.lient
  sa.lig
  sa.line
  sa.liva
  sa.lomon
  sa.long
  sa.loon
  sa.lt
  sa.lute
  sa.lutt
  sa.lvation
  sa.lær
  sa.mael
  sa.mantha
  sa.marithan
  sa.moa
  sa.mojed[y]
  sa.muel
  sa.murai
  sa.natorium
  sa.nere, nering, naritet
  sa.nitet
  sa.nity, nitize
  sa.piens
  sa.pphire
  sa.rah
  sa.rajevo
  sa.rin
  sa.rong
  sa.ruman
  sa.tan
  sa.telitt
  sa.tellite
  sa.tire
  sa.tisfy, tisfaction, tisfied
  sa.toshi
  sa.turate
  sa.turday
  sa.turn/us
  sa.tyr
  sa.usage
  sa.vage
  sa.vannah
  sa.vant
  sa.ved
  sa.viour
  sa.vory
  sa.vour
  sa.vvy
  sa.wing
  sa.xophone
  sa.ying
  sa.yonara

  sa.bulum (gravel)
  sa,cellum (chapel)
  sa.cratus (sacred)
  sa.eculum (age, generation)
  sa.epio (to fence in, surround)
  sa.evitia (savageness)
  sa.evus (savage, violent)
  sa.gino (to fatten)
  sa.gitta (arrow)
  sa.lebra (roughness)
  sa.liaris (splendid)
  sa.lio, sa.lium (jump)
  sa.lubris (health-giving, beneficial)
  sa.lum (sea)
  sa.natio (cure)
  sa.nitas (health)
  sa.pidus (tasty)

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
