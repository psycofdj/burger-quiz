#pragma once
#include "common.hh"
#include "serial.hh"
#include "interrupt.hh"

class Buzzer {
public:
  Buzzer(id pID) :
    mPin(pID, mode::input_pullup)
  {
    auto& l_handler = Interrupt::get();
    l_handler.store(pID, [this](void) {
        this->m_tiggerCallback();
      });
  }

  void onTriggered(t_callback p_callBack)
  {
    m_tiggerCallback = p_callBack;
  }

private:
  Pin        mPin;
  t_callback m_tiggerCallback;
};
