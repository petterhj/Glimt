#include "Leds.h"

// LEDs
Leds::Leds()
{
  // RGB LEDs
  this->leds = new Adafruit_NeoPixel(LEDS_COUNT, LEDS_PIN, NEO_GRB + NEO_KHZ800);
}

// Init
void Leds::init() {
  if (DEBUG)
    Serial.print(F("[GLIMT][Leds] Initializing... "));
    
  this->leds->begin();
  this->leds->setBrightness(LEDS_BRIGHTNESS);
  //this->off();
  this->leds->show();

  if (DEBUG)
    Serial.println(F("DONE"));
}

void Leds::homecare(bool on) {
  if (on == true) {
    this->leds->setPixelColor(0, this->leds->Color(0, 255, 0));
    this->leds->setPixelColor(1, this->leds->Color(0, 255, 0));
  } else {
    this->leds->setPixelColor(0, this->leds->Color(0, 0, 0));
    this->leds->setPixelColor(1, this->leds->Color(0, 0, 0));
  }

  this->leds->show();
}
void Leds::snap(bool on) {
  if (on == true) {
    this->leds->setPixelColor(2, this->leds->Color(255, 212, 0));
    this->leds->setPixelColor(3, this->leds->Color(255, 212, 0));
  } else {
    this->leds->setPixelColor(2, this->leds->Color(0, 0, 0));
    this->leds->setPixelColor(3, this->leds->Color(0, 0, 0));
  }

  this->leds->show();
}
void Leds::menu(bool on) {
  if (on == true) {
    this->leds->setPixelColor(4, this->leds->Color(255, 20, 10));
    this->leds->setPixelColor(5, this->leds->Color(255, 20, 10));
  } else {
    this->leds->setPixelColor(4, this->leds->Color(0, 0, 0));
    this->leds->setPixelColor(5, this->leds->Color(0, 0, 0));
  }

  this->leds->show();
}
void Leds::activity(bool on) {
  if (on == true) {
    this->leds->setPixelColor(6, this->leds->Color(0, 0, 255));
    this->leds->setPixelColor(7, this->leds->Color(0, 0, 255));
  } else {
    this->leds->setPixelColor(6, this->leds->Color(0, 0, 0));
    this->leds->setPixelColor(7, this->leds->Color(0, 0, 0));
  }

  this->leds->show();
}

void Leds::blink(int r, int g, int b) {
  if (DEBUG)
    Serial.print(F("[GLIMT][Leds] Feedback: Confirm"));

  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < LEDS_COUNT; i++)
      this->leds->setPixelColor(i, this->leds->Color(r, g, b));
    this->leds->show();
    
    delay(100);
  
    for (int i = 0; i < LEDS_COUNT; i++)
      this->leds->setPixelColor(i, this->leds->Color(0, 0, 0));
    this->leds->show();

    if (j != 2)
      delay(100);
  }
}
void Leds::off() {
  for (int i = 0; i < LEDS_COUNT; i++)
    this->leds->setPixelColor(i, this->leds->Color(0, 0, 0));
}



