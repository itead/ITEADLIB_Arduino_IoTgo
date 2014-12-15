/*
  LOG.cpp - Library for standard logging convention.
  Created by Meir Michanie, June 9, 2010.
  Released into the public domain.
  Version 0.1
*/

#include "HW_GSM_LOG.h"

LOG::LOG(int level)
{
     setLevel(level);
}

void LOG::DATA(const char* string)
{
     if (_level > 4) {
          DebugSerial.print(string);
     }
}

void LOG::DATA(int number)
{
     if (_level > 4) {
          DebugSerial.print(number);
     }
}

void LOG::DEBUG(const char* string)
{
     if (_level > 3) {
          DebugSerial.print("\n[DEBUG]: ");
          DebugSerial.println(string);
     }
}

void LOG::INFO(const char* string)
{
     if (_level > 2) {
          DebugSerial.print("\n[INFO]: ");
          DebugSerial.println(string);
     }
}

void LOG::WARNING(const char* string)
{
     if (_level > 1) {
          DebugSerial.print("\n[WARNING]: ");
          DebugSerial.println(string);
     }
}

void LOG::CRITICAL(const char* string)
{
     if (_level > 0) {
          DebugSerial.print("\n[CRITICAL]: ");
          DebugSerial.println(string);
     }
}
