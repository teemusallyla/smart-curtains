#include "Arduino.h"
#include "curtain_led.h"

Led::Led(int pin){
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    _pin = pin;
    status = HIGH;
}

void Led::turnOn(){
    digitalWrite(_pin, LOW);
    status = LOW;
}

void Led::turnOff(){
    digitalWrite(_pin, HIGH);
    status = HIGH;
}

void Led::toggle(){
    status = !status;
    digitalWrite(_pin, status);
}