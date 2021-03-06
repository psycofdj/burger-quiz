#pragma once
#include "lambda.hh"

/**
 ** @brief represents time
 */
typedef unsigned long time_t;

/**
 ** @brief represents a time duration
 */
typedef long duration_t;

/**
 ** @brief type for storing `void function(void)` lambdas
 */
typedef Lambda<void(void)> t_callback;

/**
 ** @brief Represent the board digital pin indexes
 */
enum class id : int8_t {
  d0 = 0,
  d1 = 1,
  d2 = 2,
  d3 = 3,
  d4 = 4,
  d5 = 5,
  d6 = 6,
  d7 = 7,
  d8 = 8,
  d9 = 9,
  d10 = 10,
  d11 = 11,
  d12 = 12,
  d13 = 13,
};

/**
 ** @brief Represent the board digital pin modes
 */
enum class mode : uint8_t {
  input        = INPUT,
  output       = OUTPUT,
  input_pullup = INPUT_PULLUP,
};


/**
 ** @brief Represent signal status
 */
enum class signal : uint8_t {
  low  = LOW,
  high = HIGH,
};


/**
 ** @brief Represent signal status
 */
enum class isr : uint8_t {
  low     = LOW,
  change  = CHANGE,
  raising = RISING,
  falling = FALLING,
};


// Local Variables:
// ispell-local-dictionary: "american"
// End:
