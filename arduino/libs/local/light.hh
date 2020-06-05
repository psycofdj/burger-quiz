#pragma once
#include "common.hh"
#include "object.hh"
#include "serial.hh"

class Light : public Object {
public:
  Light(pin_t pPin):
    Object(pPin),
    mState(false)
  {
    pinMode(mPin, OUTPUT);
    digitalWrite(mPin, LOW);
  }

public:
  void activate(void)
  {
    digitalWrite(mPin, HIGH);
    mState = true;
  }

  void deactivate(void)
  {
    digitalWrite(mPin, LOW);
    mState = false;
  }

private:
  bool mState;
};
