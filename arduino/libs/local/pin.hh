#pragma once
#include "common.hh"

/**
 ** @brief Represent a board digital pin
 */
struct Pin
{
  /**
   ** @brief Cstr
   ** @param pID   Board pin identifier
   ** @param pMode Board pin mode: mode::input, mode::output or mode::input_pullup
   */
  Pin(id pID, mode pMode) :
    mID(pID)
  {
    configure(pMode);
  }

  /**
   ** @brief Cstr    Overload that initialize to given output value, implies mode::output
   ** @parma pStatus Signal pin signal statue: signal::high or signal::low
   */
  Pin(id pID, signal pStatus) :
    Pin(pID, mode::output)
  {
    write(pStatus);
  }

  /**
   ** @brief Set output signal to signal::high
   */
  void high(void) {
    write(signal::high);
  }

  /**
   ** @brief Set output signal to signal::low
   */
  void low(void) {
    write(signal::low);
  }

  /**
   ** @brief Return true if signal is signal::high
   */
  bool isHigh(void) {
    return signal::high == read();
  }

  /**
   ** @brief Return true if signal is signal::low
   */
  bool isLigh(void) {
    return !isHigh();
  }

private:
  // wrapper to stdlib pinMode
  void configure(mode pMode) {
    pinMode(static_cast<uint8_t>(mID), static_cast<uint8_t>(pMode));
  }

  // wrapper to stdlib digitalWrite
  void write(signal pValue) {
    digitalWrite(static_cast<uint8_t>(mID), static_cast<uint8_t>(pValue));
  }

  // wrapper to stdlib digitalRead
  signal read(void) {
    uint8_t lVal = digitalRead(static_cast<uint8_t>(mID));
    return static_cast<signal>(lVal);
  }

private:
  id mID;
};

// Local Variables:
// ispell-local-dictionary: "american"
// End:
