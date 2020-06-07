#pragma once
#include "pin.hh"

/**
 ** @brief Represents anything that need to be updated on each loop
 */
class Updatable {
public:
  virtual void update(time_t, duration_t)
  { }
};


/**
 ** @brief Represents a physical object on board attached to a particular pin
 */
class Object : public Updatable {
protected:
  Object(id pID, mode pMode):
    Updatable(),
    mPin(pID, pMode)
  { }

  Object(id pID, signal pSignal):
    Updatable(),
    mPin(pID, pSignal)
  { }

protected:
  Pin mPin;
};

// Local Variables:
// ispell-local-dictionary: "american"
// End:
