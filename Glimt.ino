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
    
    //  Notification control
    // ===============================================================
    
    // Home care
    if ((button.channel == 12) && (button.state == 1)) {
      leds.homecare(true);
      screen.homecare(true);
    }
    if ((button.channel == 12) && (button.state == 0)) {
      leds.homecare(false);
      screen.homecare(false);
    }
    
    // Snap
    if ((button.channel == 13) && (button.state == 1)) {
      leds.snap(true);
      screen.snap(true);
    }
    if ((button.channel == 13) && (button.state == 0)) {
      leds.snap(false);
      screen.snap(false);
    }
    
    // Menu
    if ((button.channel == 14) && (button.state == 1)) {
      leds.menu(true);
      screen.menu(true);
    }
    if ((button.channel == 14) && (button.state == 0)) {
      leds.menu(false);
      screen.menu(false);
    }

    // Activity
    if ((button.channel == 15) && (button.state == 1)) {
      leds.activity(true);
      screen.activity(true);
    }
    if ((button.channel == 15) && (button.state == 0)) {
      leds.activity(false);
      screen.activity(false);
    }

    //  All off / on
    // ===============================================================
    if ((button.channel == 16) && (button.state == 1)) {
      leds.homecare(true);
      screen.homecare(true);
      leds.snap(true);
      screen.snap(true);
      leds.menu(true);
      screen.menu(true);
      leds.activity(true);
      screen.activity(true);
    }
    if ((button.channel == 16) && (button.state == 0)) {
      leds.homecare(false);
      screen.homecare(false);
      leds.snap(false);
      screen.snap(false);
      leds.menu(false);
      screen.menu(false);
      leds.activity(false);
      screen.activity(false);
    }
    

    //  Battery control
    // ===============================================================
    if ((button.channel == 8) && (button.state == 1)) {
      screen.setBattery(BATTERY_FULL);
      screen.drawBattery();
    }
    if ((button.channel == 9) && (button.state == 1)) {
      screen.setBattery(BATTERY_HALF);
      screen.drawBattery();
    }
    if ((button.channel == 10) && (button.state == 1)) {
      screen.setBattery(BATTERY_EMPTY);
      screen.drawBattery();
    }
    if ((button.channel == 11) && (button.state == 1)) {
      screen.setBattery(BATTERY_CHARGING);
      screen.drawBattery();
    }

    delay(RADIO_DELAY);
    Serial.println(freeMemory());
  }
}

