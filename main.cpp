#include "mbed.h"
#include "C12832.h"
#include "Motors.h"
#include "Encoders.h"
#include "Control.h"
#include "globals.h"
//QEI library link https://os.mbed.com/users/aberk/code/QEI/

void StraightLineRampTest(Motor *rightMotor, Motor *leftMotor);
void Reverse(Motor *rightMotor, Motor *leftMotor, Control *ControlAlgorithm);
void goForwards(Control* ControlAlgorithm, Motor *rightMotor, Motor *leftMotor);
void ReverseWithEncoders(Motor *rightMotor, Motor *leftMotor, Encoder *rightEncoder, Encoder *leftEncoder, Control* ControlAlgorithm);

int main() {
  
  DigitalOut testLED (D8);
  testLED=1;
  Serial hm10(BLUETOOTH_TX, BLUETOOTH_RX, 9600); // TX, RX, baud for bluetooth
  C12832 lcd(D11, D13, D12, D7, D10);
  lcd.cls();
  AnalogIn pot1(A1);
  DigitalIn turnOn(D4);
  DigitalOut enable(MOTORS_ENABLE);

  DigitalOut LEDPair1 (DARLINGTON_PAIR_1);
  DigitalOut LEDPair2 (DARLINGTON_PAIR_2);
  DigitalOut LEDPair3 (DARLINGTON_PAIR_3);

  LEDPair1=1;
  LEDPair2=1;
  LEDPair3=1;

  enable=1;

  Motor rightMotor (MOTOR_RIGHT_PWM, MOTOR_RIGHT_DIRECTION);
  Motor leftMotor (MOTOR_LEFT_PWM, MOTOR_LEFT_DIRECTION);

  Encoder leftEncoder (MOTOR_LEFT_CH_A,MOTOR_LEFT_CH_B);
  Encoder rightEncoder (MOTOR_RIGHT_CH_A,MOTOR_RIGHT_CH_B);
  
  Control ControlAlgorithm(SENSOR_LEFT_1, SENSOR_LEFT_2, SENSOR_LEFT_3,  SENSOR_RIGHT_1,  SENSOR_RIGHT_2, SENSOR_RIGHT_3, 
  &leftMotor, &rightMotor, &leftEncoder, &rightEncoder);

  while(1) {
    /*if(turnOn.read()==1){
        ControlAlgorithm.start();
        lcd.cls();
    }*/
    if(ControlAlgorithm.getStatus()==false){
        ControlAlgorithm.getSensorMeasurements();
    }
    /*lcd.locate(0, 0);
    lcd.printf("SL1:%.1f SR1:%.1f, P:%.1f", ControlAlgorithm.getLeftSensor1Volts(), ControlAlgorithm.getRightSensor1Volts(),pot1.read());
    lcd.locate(0, 10);
    lcd.printf("SL2:%.1f SR2:%.1f", ControlAlgorithm.getLeftSensor2Volts(), ControlAlgorithm.getRightSensor2Volts());
    lcd.locate(0, 20);
    lcd.printf("SL3:%.1f SR3:%.1f, P:%.1f", ControlAlgorithm.getLeftSensor3Volts(), ControlAlgorithm.getRightSensor3Volts());*/
    lcd.locate(0,0);
    lcd.printf("Right Distance: %f", -rightEncoder.getDistance());
    lcd.locate(0,10);
    lcd.printf("Left Encoder: %f", leftEncoder.getDistance());
    /*lcd.printf("LSpeed:%.2f RSpeed:%.2f",leftEncoder.getSpeed(),-rightEncoder.getSpeed());
    lcd.locate(0,10);
    lcd.printf("LeftM:%.2f  RightM:%.2f",1-leftMotor.getPWM(),1-rightMotor.getPWM());*/
    if(hm10.readable()) { // Check if data is available to read
            char test = hm10.getc(); // Read the character to clear the buffer
            hm10.printf("Received: "); //sends recieved to the ble cnotroller
            hm10.printf("%c", test); //prints out the charater to the ble controller that was sent ? (worked on ios but idk about android)
            if (test == '0') {
                hm10.printf("stop");
                ControlAlgorithm.stop(); //stops the buggy
                }
            else if (test == '1') {
                hm10.printf("start");
                testLED = 0;
                ControlAlgorithm.start(); //starts the buggy
                lcd.cls();
                }
            else if(test=='r'){
                hm10.printf("reverse");
                Reverse(&rightMotor, &leftMotor, &ControlAlgorithm);
                lcd.cls();
            }
            else if(test=='t'){
                hm10.printf("distance");
                goForwards(&ControlAlgorithm, &rightMotor, &leftMotor);
                rightEncoder.resetEncoder();
                leftEncoder.resetEncoder();
            }
            else if(test=='e'){
                hm10.printf("Encoder Reverse");
                Reverse(&rightMotor, &leftMotor, &ControlAlgorithm);
            }
    }

    if(-rightEncoder.getDistance()>=1 && leftEncoder.getDistance()>=1){
        leftMotor.setPWM(0);
        rightMotor.setPWM(0);
    }

  }
}

void StraightLineRampTest(Motor *rightMotor, Motor *leftMotor){
    
    float dutyRamp= 0.0;

    // Ramp Up from 0.0 to 0.5 duty cycle in 50 steps taking 1 second
    for(int i=1; i<=50; i++){
        dutyRamp = ((float) i * 0.01);
        rightMotor->setPWM(dutyRamp);      
        leftMotor->setPWM(dutyRamp*MOTOR_BALANCE);
        wait(0.02);
    }
}

void Reverse(Motor *rightMotor, Motor *leftMotor, Control* ControlAlgorithm){
    Timer reverseTimer;
    DigitalOut led(D8);
    led=1;
    rightMotor->setPWM(0.3);
    leftMotor->setPWM(-0.3*MOTOR_BALANCE);
    reverseTimer.start();
    while(1){
        ControlAlgorithm->getSensorMeasurements();
        if(ControlAlgorithm->getLeftSensor3Volts()<LOW_VOLTAGE_THRESHOLD && ControlAlgorithm->getRightSensor3Volts()<LOW_VOLTAGE_THRESHOLD && ControlAlgorithm->getLeftSensor2Volts()<LOW_VOLTAGE_THRESHOLD
        && ControlAlgorithm->getRightSensor2Volts()<LOW_VOLTAGE_THRESHOLD && ControlAlgorithm->getLeftSensor1Volts()<LOW_VOLTAGE_THRESHOLD && ControlAlgorithm->getRightSensor1Volts()<LOW_VOLTAGE_THRESHOLD){
            reverseTimer.stop();
            break;
        }
        led=0;
    }
    while(1){
        ControlAlgorithm->getSensorMeasurements();
        if(ControlAlgorithm->getLeftSensor3Volts()>HIGH_VOLTAGE_THRESHOLD || ControlAlgorithm->getLeftSensor2Volts()>HIGH_VOLTAGE_THRESHOLD || ControlAlgorithm->getLeftSensor1Volts()>HIGH_VOLTAGE_THRESHOLD){
            break;
            
        }
        led=1;
    }
    //wait(reverseTimer.read());
    reverseTimer.reset();
    rightMotor->setPWM(0);
    leftMotor->setPWM(0);
    wait(0.1);
    ControlAlgorithm->start();
}

void goForwards(Control* ControlAlgorithm, Motor *rightMotor, Motor *leftMotor){

    rightMotor->setPWM(0.4);
    leftMotor->setPWM(0.4);

}

void ReverseWithEncoders(Motor *rightMotor, Motor *leftMotor, Encoder *rightEncoder, Encoder *leftEncoder, Control* ControlAlgorithm){
    rightMotor->setPWM(0.3);
    leftMotor->setPWM(-0.3);
    leftEncoder->resetEncoder();
    rightEncoder->resetEncoder();
    while(leftEncoder->getDistance()<=0.3 && -rightEncoder->getDistance()<=0.3){
         //Nothing
    }
    rightMotor->setPWM(0);
    leftMotor->setPWM(0);
    wait(0.1);
    ControlAlgorithm->start();
}