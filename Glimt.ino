#include <MemoryFree.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include "Constants.h"
#include "Radio.h"
#include "Leds.h"
#include "Screen.h"


// Modules
Radio radio = Radio();
Leds leds = Leds();
Screen screen = Screen();

// Setup
void setup() {
  Serial.begin(9600);

  if (DEBUG) {
    Serial.println(F("[GLIMT] Starting..."));
    Serial.println(F(" -- Debug: Enabled"));
    Serial.print(F(" -- Free memory: "));
    Serial.println(freeMemory());
    Serial.println(F("[GLIMT] Initializing modules..."));
  }

  // Initialize modules
  leds.init();
  radio.init();
  screen.init();
  
  if (DEBUG) {
    Serial.println(F("[GLIMT] Ready"));
    Serial.print(F(" - Free memory: "));
    Serial.println(freeMemory());
    Serial.println(F("[GLIMT] Listening..."));
  } else {
    Serial.println(freeMemory());
  }
}


// Loop
void loop() {
  Button button = radio.listener();

  if (button.remote == 14177786) {
    // Button 1: Home care
    if ((button.channel == 12) && (button.state == 1)) {
      leds.homecare(true);
      screen.homecare(true);
    }
    if ((button.channel == 12) && (button.state == 0)) {
      leds.homecare(false);
      screen.homecare(false);
    }
    
    // Button 2: Snap
    if ((button.channel == 13) && (button.state == 1)) {
      leds.snap(true);
      screen.snap(true);
    }
    if ((button.channel == 13) && (button.state == 0)) {
      leds.snap(false);
      screen.snap(false);
    }
    
    // Button 3: Menu
    if ((button.channel == 14) && (button.state == 1)) {
      leds.menu(true);
      screen.menu(true);
    }
    if ((button.channel == 14) && (button.state == 0)) {
      leds.menu(false);
      screen.menu(false);
    }

    // Button 4: Activity
    if ((button.channel == 15) && (button.state == 1)) {
      leds.activity(true);
      screen.activity(true);
    }
    if ((button.channel == 15) && (button.state == 0)) {
      leds.activity(false);
      screen.activity(false);
    }

    // Other
    if ((button.channel == 8) && (button.state == 1))
      leds.blink(0, 255, 0);
    if ((button.channel == 9) && (button.state == 1))
      leds.blink(255, 0, 0);

    // FB | HB | EB | CB

    delay(RADIO_DELAY);
    Serial.println(freeMemory());
  }
}

