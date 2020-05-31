#ifndef curtain_stepper
#define curtain_stepper

#include "Arduino.h"

class cStepper {
    public:
        cStepper(int dir_pin, int step_pin, int sleep_pin);
        void rotateDeg(float deg, float speed);
        int update();
        int setTargetPos(int target_pos);
        int setSpeed(int speed);
        int getSpeed();
        int _cur_pos;
        int _target_pos;
        int _max_pos;
        int _min_pos;
        void enable();
        void disable();
    private:
        int _dir_pin;
        int _step_pin;
        int _sleep_pin;
        int _max_spd;
        int _min_spd;
        int _speed;
        float _speed_inv;
};


#endif
