#pragma once
#include <SoftwareSerial.h>

SoftwareSerial gSerial(0, 1);

namespace std {
std::ohserialstream sout(gSerial);
std::ihserialstream sin(gSerial);
}
