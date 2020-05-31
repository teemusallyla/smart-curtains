#include "Arduino.h"
#include "curtain_stepper.h"

cStepper::cStepper(int dir_pin, int step_pin, int sleep_pin) {
    pinMode(dir_pin, OUTPUT);
    pinMode(step_pin, OUTPUT);
    pinMode(sleep_pin, OUTPUT);
    digitalWrite(sleep_pin, HIGH);
    _dir_pin = dir_pin;
    _step_pin = step_pin;
    _sleep_pin = sleep_pin;
    _max_pos = 50;
    _min_pos = -10;
    _cur_pos = 0;
    _speed = 60; // rotation speed in rounds per minute
    _speed_inv = (float) 1 / _speed;
    _max_spd = 150;
    _min_spd = 10;
}

void cStepper::rotateDeg(float deg, float speed) {
    if (deg > 0) digitalWrite(_dir_pin, HIGH);
    else digitalWrite(_dir_pin, LOW);

    int steps = abs(deg)*4/0.9;
    //float usDelay = (float) 1 / speed * 80;
    float usDelay = (float) 60 / 3200 * _speed_inv * 1e6;
    for (int i = 0; i < steps; i++) {
        digitalWrite(_step_pin, HIGH);
        delayMicroseconds(usDelay);
        digitalWrite(_step_pin, LOW);
        delayMicroseconds(usDelay);
    }
}

int cStepper::update() {
    int steps_per_pos = 40; // 40 steps = 9 degrees = 1 pos
    //float delay_us = (float) 1 / _speed * 80;
    if (_target_pos > _cur_pos) digitalWrite(_dir_pin, HIGH);
    else if (_target_pos < _cur_pos) digitalWrite(_dir_pin, LOW);
    else return 1;
    float delay_us = (float) 60 / 3200 * _speed_inv * 1e6;
    for (int i = 0; i < steps_per_pos; i++) {
        digitalWrite(_step_pin, HIGH);
        delayMicroseconds(delay_us);
        digitalWrite(_step_pin, LOW);
        delayMicroseconds(delay_us);
    }
    if (_target_pos > _cur_pos) _cur_pos++;
    else _cur_pos--;
    return 0;
}

int cStepper::setTargetPos(int target_pos) {
    if (target_pos != 1000 && target_pos != -1000)
        target_pos = max(_min_pos, min(_max_pos, target_pos));
    _target_pos = target_pos;
    return _target_pos;
}

void cStepper::disable() {
    digitalWrite(_sleep_pin, LOW);
}

void cStepper::enable() {
    digitalWrite(_sleep_pin, HIGH);
}

int cStepper::setSpeed(int speed) {
    _speed = max(_min_spd, min(_max_spd, speed));
    _speed_inv = (float) 1 / _speed;
    return _speed;
}

int cStepper::getSpeed() {
    return _speed;
}
