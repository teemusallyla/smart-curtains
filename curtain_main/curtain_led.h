#ifndef curtain_led
#define curtain_led

#include "Arduino.h"

// Led class mainly for debugging purposes
class Led {
    public:
        Led(int pin);
        void turnOn();
        void turnOff();
        void toggle();
        int status;
    private:
        int _pin;
};


#endif