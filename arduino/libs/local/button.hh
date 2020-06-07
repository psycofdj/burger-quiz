#pragma once
#include "common.hh"
#include "object.hh"
#include "serial.hh"

class Button : public Object {
public:
  const static duration_t mcs_minLastDeltaMS = 100;

  enum state {
    unknown,
    pressed,
    released
  };

public:
  Button(id pID):
    Object(pID, mode::input),
    mState(state::unknown),
    mLastEvent(0)
  {
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

  void onPressed(t_callback pCallback)
  {
    m_pressedCallback = pCallback;
  }

  void onReleased(t_callback pCallback)
  {
    m_releasedCallback = pCallback;
  }

private:
  state readState(void)
  {
    if (mPin.isHigh()) {
      return state::pressed;
    }
    return state::released;
  }

private:
  state       mState;
  time_t      mLastEvent;
  t_callback  m_pressedCallback;
  t_callback  m_releasedCallback;
};
