#include "mbed.h"
#include "QEI.h"
#include "globals.h"

class Encoder{
 private:
    QEI wheel;
    float speed, angularSpeed, encoderTickPeriod, encoderTickFrequency;
    Ticker encoderTicker;
    int previousPulses, currentPulses;
    void CalculateAngularSpeed(){
        currentPulses = wheel.getPulses();
        angularSpeed = (float)(currentPulses-previousPulses)/(1024*encoderTickPeriod); // Revolutions per second // 198mm distance between wheel lines of action
        previousPulses = currentPulses;
    }
 public:
    Encoder(PinName channelA, PinName channelB) : wheel(channelA, channelB, NC, 624, QEI::X4_ENCODING){
        encoderTickFrequency = 150;
        encoderTickPeriod = 1/encoderTickFrequency; //01.s = 100ms;
        encoderTicker.attach(callback(this, &Encoder::CalculateAngularSpeed), encoderTickPeriod);
        previousPulses = 0;
    };

    void resetEncoder(){
        wheel.reset();
    }

    float getRPM(){
        return angularSpeed*60;
    }

    int getEncoderPulses(){
        return wheel.getPulses();
    }

    float getRevolutions(){
        return (float)wheel.getPulses()/1024;
    }

    float getAngle(){
        return wheel.getPulses()/((float)1024)*2*PI;
    }

    float getDistance(){
        return getAngle()*WHEEL_RADIUS;
    }

    float getSpeed(){
        return angularSpeed*WHEEL_RADIUS*2*PI;
    }
};