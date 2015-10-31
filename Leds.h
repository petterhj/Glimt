#ifndef Leds_h
#define Leds_h

#include "Arduino.h"
#include "Adafruit_NeoPixel.h"
#include "Constants.h"

class Leds
{
  public:
    Leds();
    void init();

    void homecare(bool on);
    void snap(bool on);
    void menu(bool on);
    void activity(bool on);

    void blink(int r, int g, int b);
    void off();

    Adafruit_NeoPixel* leds;
  private:
    

};

#endif

