#include <perso.h>
#include "common.hh"
#include "serial.hh"
#include "app.hh"

void setup() {
  App::get().setup();
}

void loop() {
  App& lApp = App::get();

  if (true == lApp.isFirstLoop()) {
    lApp.ready();
  }
  lApp.loop();
}
