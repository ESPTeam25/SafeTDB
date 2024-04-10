#include "mbed.h"
#include "globals.h"

class Control{
 private:
    AnalogIn SensorL1, SensorL2, SensorL3, SensorR1, SensorR2, SensorR3;
    Motor *leftMotor, *rightMotor;
    Encoder *leftEncoder, *rightEncoder;
    float leftSensor3, leftSensor2, leftSensor1, rightSensor3, rightSensor2, rightSensor1;
    float error, previousError, differential, baseDutyCycle, currentBaseSpeedError, previousBaseSpeedError, noLineDistance;
    bool waitingForLine, running;
    Ticker samplingTicker;

 public:
    Control(PinName sL1, PinName sL2, PinName sL3, PinName sR1, PinName sR2, PinName sR3, Motor* leftM, Motor* rightM, Encoder* leftEnc, Encoder* rightEnc) : 
    SensorL1(sL1), SensorL2(sL2), SensorL3(sL3), SensorR1(sR1), SensorR2(sR2), SensorR3(sR3), leftMotor(leftM), rightMotor(rightM), leftEncoder(leftEnc),rightEncoder(rightEnc){
        baseDutyCycle=0;
        previousBaseSpeedError=0;
        previousError=0;
        noLineDistance=0;
        waitingForLine=false;
        running=false;
        stop();
    }

    void getSensorMeasurements(){

        leftSensor3 = SensorL3.read();
        leftSensor2 = SensorL2.read();
        leftSensor1 = SensorL1.read();
        
        rightSensor3 = SensorR3.read();
        rightSensor2 = SensorR2.read();
        rightSensor1 = SensorR1.read();

    }

    void calculatePIDError(){

        //Turn left when error is positive, turn right when error is negative
        error = SENSOR_3_BIAS*leftSensor3 + SENSOR_2_BIAS * leftSensor2 + SENSOR_1_BIAS * leftSensor1 - (SENSOR_3_BIAS*rightSensor3 + SENSOR_2_BIAS * rightSensor2 + SENSOR_1_BIAS * rightSensor1);

        differential = PROPORTIONAL_GAIN * error + DERIVATIVE_GAIN*(error-previousError)*CONTROL_FREQUENCY;

        previousError = error;
    }

    void calculateBaseSpeedError(){
        currentBaseSpeedError = 0.5*leftEncoder->getSpeed()-0.5*rightEncoder->getSpeed() - BASE_SPEED;
        baseDutyCycle -= BASE_SPEED_PROPORTIONAL_GAIN*currentBaseSpeedError - BASE_SPEED_DERIVATIVE_GAIN*(currentBaseSpeedError-previousBaseSpeedError)*CONTROL_FREQUENCY;

        previousBaseSpeedError = currentBaseSpeedError;
    }

    void checkNoLineDistance(){
        
        if(getLeftSensor3Volts()<LOW_VOLTAGE_THRESHOLD && getRightSensor3Volts()<LOW_VOLTAGE_THRESHOLD && getLeftSensor2Volts()<LOW_VOLTAGE_THRESHOLD
        && getRightSensor2Volts()<LOW_VOLTAGE_THRESHOLD && getLeftSensor1Volts()<LOW_VOLTAGE_THRESHOLD && getRightSensor1Volts()<LOW_VOLTAGE_THRESHOLD){
            
            if(waitingForLine==false){
                rightEncoder->resetEncoder();
                leftEncoder->resetEncoder();
                waitingForLine=true;
            }

            else{
                if(-rightEncoder->getDistance()>=0.2 && leftEncoder->getDistance()>=0.2){
                    stop();
                    waitingForLine=false;
                }
            }    
         }
        else{
             waitingForLine = false;
         }
    }
    
    void runAlgorithm(){

        getSensorMeasurements();
        calculateBaseSpeedError();
        calculatePIDError();
        
        leftMotor->setPWM(baseDutyCycle - differential/2);
        rightMotor->setPWM(baseDutyCycle + differential/2);

        checkNoLineDistance();

    }

    void stop(){
        samplingTicker.detach();
        leftMotor->setPWM(0);
        rightMotor->setPWM(0);
        running=false;
    }

    void start(){
        samplingTicker.attach(callback(this, &Control::runAlgorithm), 1/(float)CONTROL_FREQUENCY);
        running=true;
    }

    float getLeftSensor3Volts(){
        return 3.3*leftSensor3;
    }

    float getLeftSensor2Volts(){
        return 3.3*leftSensor2;
    }

    float getLeftSensor1Volts(){
        return 3.3*leftSensor1;
    }

    float getRightSensor3Volts(){
        return 3.3*rightSensor3;
    }

    float getRightSensor2Volts(){
        return 3.3*rightSensor2;
    }

    float getRightSensor1Volts(){
        return 3.3*rightSensor1;
    }

    bool getStatus(){
        return running;
    }
};