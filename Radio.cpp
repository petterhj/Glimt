#include "Radio.h"

// Radio
Radio::Radio(){}

// Init
void Radio::init() {
  if (DEBUG)
    Serial.print(F("[GLIMT][Radio] Initializing... "));

  if (DEBUG)
    Serial.println(F("DONE"));
}

// Listener
Button Radio::listener() {
  int i = 0;
  unsigned long t = 0;

  byte prevBit = 0;
  byte bit = 0;

  unsigned long sender = 0;
  bool on = false;
  unsigned int recipient = 0;

  // Data collection from reciever circuit
  while (i < 64) {
    t = pulseIn(RADIO_PIN, LOW, 1000000);

    if (t > 200 && t < 400) { 
      bit = 0;
    } else if (t > 1100 && t < 1560) { 
      bit = 1;
    } else { 
      i = 0;
      break;
    }

    if (i % 2 == 1) {
      if ((prevBit ^ bit) == 0) { // must be either 01 or 10, cannot be 00 or 11
        i = 0;
        break;
      }
     
      if (i < 53) { // first 26 data bits
        sender <<= 1;
        sender |= prevBit;
      }
      else if (i == 55) { // 27th data bit
        on = prevBit;
      }
      else { // last 4 data bits
        recipient <<= 1;
        recipient |= prevBit;
      }
    }

    prevBit = bit;
    ++i;
  }

  // Interpret message
  if (i > 0) {
    if (DEBUG) {
      Serial.print(F("[GLIMT][Radio] Button press detected, id = "));
      Serial.print(sender);
      Serial.print(F(", channel = "));
      Serial.print(recipient);
      Serial.print(F(", state = "));
      Serial.println(on);
    }

    return Button(sender, recipient, on);
  }

  return Button();
}


// Button
Button::Button(long remote, int channel, int state) {
  this->remote = remote;
  this->channel = channel;
  this->state = state;
}
