// LEFT MOTOR IS MOTOR1
// RIGHT MOTOR BOTTOM 3 WIRES, LEFT MOTOR IS TOP 3

// ###########  PINS ###########
// #### DO NOT USE ##### 
// PC_13, PC_14, PC_15, PH_0, PH_1 (Email from O'Toole)
// Any shared Arduino pine, however, the list of pins below
// includes some interior pins, these are not used by the shield.

// ####### AVAILABLE PINS ######  
// RESERVED ANALOGUEIN  PC_2, PC_3, PC_4, PC_5, PB_1
// RESERVED BLUETOOTH   PA_11, PA_12
// WITH PWM AVAILABLE   PB_7, PB_13, PB_14, PB_15, PC_6, PC_8, PC_9
// GENERAL USE          PB_2, PB_8, PB_9, PB_10, PB_12
//                      PC_10, PC_11, PC_12, PC_13, PC_14, PC_15
//                      PD_2


// Motors
// LEFT MOTOR IS MOTOR1
// RIGHT MOTOR BOTTOM 3 WIRES, LEFT MOTOR IS TOP 3.
//#define MOTOR_RIGHT_BIPOLAR == GND    Green wire to pin 1
//#define MOTOR_LEFT_BIPOLAR == GND     Green wire to pin 4
#define MOTOR_LEFT_PWM PC_8            // Purple to pin 3
#define MOTOR_RIGHT_PWM PC_6           // Pink to pin 6

#define MOTOR_LEFT_DIRECTION PC_11     // Purple goes to pin 2
#define MOTOR_RIGHT_DIRECTION PD_2     // Pink goes to pin 5
#define MOTORS_ENABLE PB_7            // Red wire goes to pin 7 of JP1A header

// QUADRATURES
#define MOTOR_LEFT_CH_A PC_10     // orange     
#define MOTOR_LEFT_CH_B PC_12     // purple       
#define MOTOR_RIGHT_CH_A PB_13    // orange
#define MOTOR_RIGHT_CH_B PB_14    // purple

// Sensors
#define SENSOR_LEFT_1 PC_3      // Number 3
#define SENSOR_LEFT_2 PC_4      // Number 2
#define SENSOR_LEFT_3 PC_1      // Number 1
#define SENSOR_RIGHT_1 PC_2     // Number 4
#define SENSOR_RIGHT_2 PB_1     // Number 5
#define SENSOR_RIGHT_3 PC_0     // Number 6

//Darlington for LED pair inputs
#define DARLINGTON_PAIR_1 PC_9
#define DARLINGTON_PAIR_2 PB_8
#define DARLINGTON_PAIR_3 PB_9

// Bluetooth Transmit/Recieve
#define BLUETOOTH_TX PA_11      // STM TX Green          
#define BLUETOOTH_RX PA_12      // STM RX Yellow
// BT Power 3.3V next to E5V and GND between PB_7 and PA_15

/* ######  OTHER GLOBALS ###### */
// GEOMETRY STUFF
#define PI 3.14159
#define WHEEL_RADIUS  0.0372
#define MOTOR_BALANCE 1.05            // For balancing efficieny of motors

// CONTROL LOOP
#define SENSOR_3_BIAS 3
#define SENSOR_2_BIAS 2
#define SENSOR_1_BIAS 1

#define CONTROL_FREQUENCY 70
#define PROPORTIONAL_GAIN 0.25
#define DERIVATIVE_GAIN 0.007
#define BASE_DUTY_CYCLE 0.3
#define BASE_SPEED 0.3 //0.4 is safe lap
#define BASE_SPEED_PROPORTIONAL_GAIN 0.03
#define BASE_SPEED_DERIVATIVE_GAIN 0.0007 //0.0004
#define LOW_VOLTAGE_THRESHOLD 0.8
#define HIGH_VOLTAGE_THRESHOLD 1.2