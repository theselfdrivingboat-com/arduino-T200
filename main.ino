/* Note: the following code is a modification of the Knob Example that
  comes with the ESC library, so that it works with an ESP32 DevKit V1
  and my particular ESC (generic 30A) and brushless motor (generic 2200KV)
*/

#include <ESP32Servo.h> // ESP32Servo library installed by Library Manager
#include "ESC.h" // RC_ESP library installed by Library Manager

#define ESC_PIN_RIGHT (12) // connected to ESC control wire
#define ESC_PIN_LEFT (13)
#define LED_BUILTIN (2) // not defaulted properly for ESP32s/you must define it
#define POT_PIN (34) // Analog pin used to connect the potentiometer center pin

// Note: the following speeds may need to be modified for your particular hardware.
#define MIN_SPEED 1500 // speed just slow enough to turn motor off
#define MAX_SPEED_FORWARD 1800 // speed where my motor drew 3.6 amps at 12v.
#define MAX_SPEED_BACKWARD 1200

ESC Right_ESC (ESC_PIN_RIGHT, 1200, 1800, 500); // ESC_Name (PIN, Minimum Value, Maximum Value, Arm Value)
ESC Left_ESC (ESC_PIN_LEFT, 1200, 1800, 500); // ESC_Name (PIN, Minimum Value, Maximum Value, Arm Value)

long int val; // variable to read the value from the analog pin
int curr_speed = MIN_SPEED;

void setup() {
  Serial.begin(9600);
  delay(1000);
  pinMode(POT_PIN, INPUT);
  pinMode(ESC_PIN_RIGHT, OUTPUT);
  pinMode(ESC_PIN_LEFT, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // set led to on to indicate arming
  Right_ESC.arm(); // Send the Arm command to ESC
  Left_ESC.arm();
  delay(5000); // Wait a while
  digitalWrite(LED_BUILTIN, LOW); // led off to indicate arming completed

  // Send a stopped signal (1500 microseconds) for a few seconds to initialize the ESC.
  // You will hear two tones indicating initialization, and then you can send a signal
  // from 1100-1900 µs to operate the thruster. 1500 – 1900 µs for forward thrust,
  // 1100 – 1500 µs for reverse.

  // the following loop turns on the motor slowly, so get ready
  //for (int i=0; i<350; i++){ // run speed from 840 to 1190
  //myESC.speed(MIN_SPEED+i); // motor starts up about half way through loop
  //delay(10);
  //Serial.println(i);
  //}
  //Serial.println("finished loop");
} // speed will now jump to pot setting

void loop() {
//  val = analogRead(POT_PIN); // read the pot (value between 0 and 4095 for ESP32 12 bit A to D)
//  val = map(val, 0, 4095, MIN_SPEED, MAX_SPEED); // scale pot reading to valid speed range
//  Serial.println(val);
//  myESC.speed(val); // sets the ESC speed
  curr_speed = forward_accelerate(10, curr_speed);
  delay(10);
  curr_speed = forward_deccelerate(10, curr_speed);
  delay(1000); // Wait for a while
}

int forward_accelerate(int seconds, int start_speed) {
  int speed_of_motor = start_speed;
  for (int i = 0; i < 100*seconds; i++) { // run speed from 840 to 1190
    speed_of_motor = start_speed + i;
    if(speed_of_motor > MAX_SPEED_FORWARD) {
      speed_of_motor = MAX_SPEED_FORWARD;
    }
    Left_ESC.speed(speed_of_motor); // motor starts up about half way through loop
    Right_ESC.speed(speed_of_motor);
    delay(10);
    Serial.println(i);
  }
  Serial.println("finished loop");
  return speed_of_motor;
}

int forward_deccelerate(int seconds, int start_speed) {
  int speed_of_motor = start_speed;
  for (int i = 0; i < 100*seconds; i++) { // run speed from 840 to 1190
    speed_of_motor = start_speed - i;
    if(speed_of_motor < MIN_SPEED) {
      speed_of_motor = MIN_SPEED;
    }
    Left_ESC.speed(speed_of_motor); // motor starts up about half way through loop
    Right_ESC.speed(speed_of_motor);
    delay(10);
    Serial.println(i);
  }
  Serial.println("finished loop");
  return speed_of_motor;
}
