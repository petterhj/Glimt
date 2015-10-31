#ifndef Radio_h
#define Radio_h

#include "Arduino.h"
#include "Constants.h"

class Button
{
  public:
    Button(long remote = 0, int channel = 0, int state = 0);

    long remote;
    int channel;
    int state;
    
  private:

};

class Radio
{
  public:
    Radio();
    void init();

    Button listener();
    
  private:

};

#endif

