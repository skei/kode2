
sketch, work in progress, notes, ..


----------------------------------------------------------------------
plugin
----------------------------------------------------------------------

format: vst3, vst2, lv2, ladspa, dssi, exe

entrypoint

  basically, the entrypoint is one or more exported functions in the plugin
  binary (.so/.dll), that returns a struct or class..

descriptor

  const/static plugin description, features, inuts/outputs, parameters,
  programs, ..

  lv2: lv2_descriptor
  vst3: plugin factory

instance

  one specific instance of the plugin

editor

parameter

  conversion from/to 0..1
  text display

voice manager

modulation matrix

----------------------------------------------------------------------
gui
----------------------------------------------------------------------

backend: xcb, cairo
window
surface
painter
widget
bitmap
  gen, proc
