#pragma once
#include "common.hh"
#include "object.hh"
#include "serial.hh"


/**
 ** @brief Represents a LED controlled by an output pin
 */
class Light : public Object {
public:
  /**
   ** @brief Cstr
   ** @param id Board pin identifier
   */
  Light(id pID):
    Object(pID, signal::low),
    mState(false)
  { }

public:
  /**
   ** @brief Switch on the LED
   ** @param pFor if given, the light will be switch off after pFor milliseconds
   */
  void on(duration_t pFor = 0)
  {
    mPin.high();
    mState = true;
    mDuration = pFor;
  }

  /**
   ** @brief Switch off the LED
   ** @param pFor if given, the light will be switch on after pFor milliseconds
   */
  void off(duration_t pFor = 0)
  {
    mPin.low();
    mState = false;
    mDuration = pFor;
  }

  /**
   ** @brief Toggle current LED state
   */
  void toggle(void)
  {
    if (true == mState)
      off();
    else
      on();
  }

  /**
   ** @brief Update callback, must be called on every loop
   ** @param pTime current time in milliseconds sonce boot
   ** @param pDelta number of milliseconds since last update call
   */
  void update(time_t /* pTime */, duration_t pDelta)
  {
    if (mDuration > 0)
    {
      mDuration -= pDelta;
      if (mDuration <= 0) {
        toggle();
        mDuration = 0;
      }
    }
  }

private:
  bool       mState;
  duration_t mDuration;
};

// Local Variables:
// ispell-local-dictionary: "american"
// End:
