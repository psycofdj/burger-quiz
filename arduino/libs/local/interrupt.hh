#pragma once
#include "common.hh"

/**
 ** @brief Attach callbacks to interupts using attachInterrupt
 ** @details
 ** This class allows to define interrupts using c++ lambda functions insted
 ** of void(*ptr(void)) function expected by attachInterrupt function
 */
class Interrupt {
private:
  /**
   ** @brief Suger helper calling attachInterrupt for given pin
   ** @details
   ** Use template parameter as a trick the parameter without need to capture it
   */
  template<id ID> struct helper {
    static bool attach(void) {
      attachInterrupt(digitalPinToInterrupt(ID), [](void) {
          Interrupt::get().interrupt(ID);
        }, RISING);
      return true;
    }
  };

private:
  // private singleton constructor
  Interrupt(void) :
    m_handlers()
  {
    for (std::size_t cIdx = 0; cIdx < 16; cIdx++) {
      m_handlers[cIdx] = [](void) {};
    }
  }

  // run stored function callback for given pin ID
  void interrupt(id pID)
  {
    m_handlers[pID]();
  }

public:
  /**
   ** @brief Return the singleton instance
   */
  static Interrupt& get(void)
  {
    if (ms_instance == 0) {
      ms_instance = new Interrupt();
    }
    return *ms_instance;
  }

  /**
   ** @brief Store given function as interrupt callback for given pin ID
   ** @details
   ** Returned value is pure syntax sugar that allows to use the function breaking power of
   ** the statement in the enumeration switch
   */
  bool store(id pID, t_callback p_callback)
  {
    m_handlers[pID] = p_callback;
    switch (pID) {
    case id::d0:  return helper<id::d0>::attach();
    case id::d1:  return helper<id::d1>::attach();
    case id::d2:  return helper<id::d2>::attach();
    case id::d3:  return helper<id::d3>::attach();
    case id::d4:  return helper<id::d4>::attach();
    case id::d5:  return helper<id::d5>::attach();
    case id::d6:  return helper<id::d6>::attach();
    case id::d7:  return helper<id::d7>::attach();
    case id::d8:  return helper<id::d8>::attach();
    case id::d9:  return helper<id::d9>::attach();
    case id::d10: return helper<id::d10>::attach();
    case id::d11: return helper<id::d11>::attach();
    case id::d12: return helper<id::d12>::attach();
    case id::d13: return helper<id::d13>::attach();
    }
    return false;
  }

private:
  t_callback        m_handlers[16];
  static Interrupt* ms_instance;
};

Interrupt*  Interrupt::ms_instance = 0;

// Local Variables:
// ispell-local-dictionary: "american"
// End:
