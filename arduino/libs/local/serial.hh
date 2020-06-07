#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"
# include <SoftwareSerial.h>
#pragma GCC diagnostic pop

SoftwareSerial gSerial(0, 1);

namespace std {
std::ohserialstream sout(gSerial);
std::ihserialstream sin(gSerial);
}
