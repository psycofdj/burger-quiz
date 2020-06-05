#pragma once
#include "common.hh"
#include "object.hh"
#include "serial.hh"
#include "lambda.hh"

class Button : public Object {
public:
  const static duration_t mcs_minLastDeltaMS = 100;

  typedef Lambda<void(void)> t_callback;

  enum state {
    unknown,
    pressed,
    released
  };

public:
  Button(pin_t pPin):
    Object(pPin),
    mState(state::unknown),
    mLastEvent(0)
  {
    pinMode(mPin, INPUT);
    mState = readState();
  }

public:
  void update(time_t pTime, duration_t /* pDelta */) override
  {
    if ((pTime - mLastEvent) < mcs_minLastDeltaMS)
      return;

    state lState = readState();
    if (mState != lState) {
      mLastEvent = pTime;
      mState = lState;
      switch (mState) {
      case state::pressed:
        m_pressedCallback();
        break;
      case state::released:
        m_releasedCallback();
        break;
      case state::unknown:
        break;
      }
    }
  }

  void onPressed(t_callback pCallback) {
    m_pressedCallback = pCallback;
  }

  void onReleased(t_callback pCallback) {
    m_releasedCallback = pCallback;
  }

private:
  state readState(void) {
    if (HIGH == digitalRead(mPin)) {
      return state::pressed;
    } else {
      return state::released;
    }
  }

private:
  state       mState;
  time_t      mLastEvent;
  t_callback  m_pressedCallback;
  t_callback  m_releasedCallback;
};
