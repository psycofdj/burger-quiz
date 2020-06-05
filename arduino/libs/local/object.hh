#pragma once

class Updatable {
public:
  virtual void update(time_t, duration_t)
  {
  }
};

class Object : public Updatable {
protected:
  Object(pin_t pPin):
    Updatable(),
    mPin(pPin)
  { }

protected:
  pin_t mPin;
};
