#ifndef Screen_h
#define Screen_h

#include "Arduino.h"
#include "MemoryFree.h"
#include "SPI.h"
#include "SD.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Constants.h"

class Screen
{
  public:
    Screen();
    void init();

    void homecare(bool on);
    void snap(bool on);
    void menu(bool on);
    void activity(bool on);
    
    void simpleBmpDraw(char *filename, uint8_t x, uint16_t y);

    Adafruit_ILI9341* screen;
    
  private:
    uint16_t read16(File &f);
    uint32_t read32(File &f);

    //void bmpDraw(char *filename, uint8_t x, uint16_t y);
};

#endif

