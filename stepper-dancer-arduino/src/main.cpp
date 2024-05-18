#include <FlexyStepper.h>

const int motorx_step_pin = 15;
const int motorx_direction_pin = 21;
const int motorx_enable_pin = 14;

const int motory_step_pin = 22;
const int motory_direction_pin = 23;
const int motory_enable_pin = 14;

const int motorz_step_pin = 3;
const int motorz_direction_pin = 2;
const int motorz_enable_pin = 26;

const int motore_step_pin = 1;
const int motore_direction_pin = 0;
const int motore_enable_pin = 14;

FlexyStepper stepperx;
FlexyStepper steppery;
FlexyStepper stepperz;
FlexyStepper steppere;

int stepperx_direction = 1;
int steppery_direction = 1;
int stepperz_direction = 1;
int steppere_direction = 1;

FlexyStepper* stepper;

// Set initial speeds and accelerations
float initial_speed = 0.01; // Revolutions per second
float initial_acceleration = 100.0; // Revolutions per second per second

void setup() {

  // Set up serial baud rate
  Serial.begin(115200);

  // Set up and enable motor pins
  stepperx.connectToPins(motorx_step_pin, motorx_direction_pin);
  steppery.connectToPins(motory_step_pin, motory_direction_pin);
  // stepperz.connectToPins(motorz_step_pin, motorz_direction_pin);
  // steppere.connectToPins(motore_step_pin, motore_direction_pin);
  pinMode(motorx_enable_pin, OUTPUT);
  digitalWrite(motorx_enable_pin, LOW);
  // pinMode(motorz_enable_pin, OUTPUT);
  // digitalWrite(motorz_enable_pin, LOW);

  // Set steps per revolution
  stepperx.setStepsPerRevolution( (float)(200 * 16) );
  steppery.setStepsPerRevolution( (float)(200 * 16) );
  // stepperz.setStepsPerRevolution( (float)(200 * 16) );
  // steppere.setStepsPerRevolution( (float)(200 * 16) );

  // Set speeds in steps per revolutions per second
  stepperx.setSpeedInRevolutionsPerSecond( initial_speed );
  steppery.setSpeedInRevolutionsPerSecond( initial_speed );
  // stepperz.setSpeedInRevolutionsPerSecond( initial_speed );
  // steppere.setSpeedInRevolutionsPerSecond( initial_speed );

  // Set acceleration in revolutions per second per second
  stepperx.setAccelerationInRevolutionsPerSecondPerSecond( initial_acceleration );
  steppery.setAccelerationInRevolutionsPerSecondPerSecond( initial_acceleration );
  // stepperz.setAccelerationInRevolutionsPerSecondPerSecond( initial_acceleration );
  // steppere.setAccelerationInRevolutionsPerSecondPerSecond( initial_acceleration );

}

byte incoming_message[13];

void loop() {

  // Wait for an incoming message
  if (Serial.available() != 0) {
    
    for (int i=0; i<13; i++) {
      incoming_message[i] = byte(0);
    }
    // Read incoming message
    Serial.readBytes(incoming_message, 13);
  
    // Select motor with first byte
    if ( incoming_message[0] == byte(0) ) {
      stepper = &stepperx;
    } 
    else if ( incoming_message[0] == byte(1) ) {
      stepper = &steppery;
    } 
    // else if ( incoming_message[0] == byte(2) ) {
    //   stepper = &stepperz;
    // } 
    // else if ( incoming_message[0] == byte(3) ) {
    //   stepper = &steppere;
    // }
  
    // Cast bytes 1-12 (skip the first byte) into 3 4-byte floats
    float input_accel = ( (float *) &incoming_message[1] )[0];
    float input_speed = ( (float *) &incoming_message[1] )[1];
    float input_revs  = ( (float *) &incoming_message[1] )[2];
    
    // stepper->setAccelerationInRevolutionsPerSecondPerSecond(input_accel);
    stepper->setSpeedInRevolutionsPerSecond(input_speed / initial_acceleration);
    // stepper->setupRelativeMoveInRevolutions(input_revs);
  }

  // stepperx.setupRelativeMoveInRevolutions(0.2);
  // steppery.setupRelativeMoveInRevolutions(0.2);
  // stepperz.setupRelativeMoveInRevolutions(0.2);
  // steppere.setupRelativeMoveInRevolutions(0.2);


  if (!stepperx.motionComplete()) {
    stepperx.processMovement();
  } else {
    stepperx_direction *= -1;
    stepperx.setTargetPositionRelativeInRevolutions(stepperx_direction * 0.25);
  }

  if (!steppery.motionComplete()) {
    steppery.processMovement();
  } else {
    steppery_direction *= -1;
    steppery.setTargetPositionRelativeInRevolutions(steppery_direction * 0.25);
  }

  // if (!stepperz.motionComplete()) {
  //   stepperz.processMovement();
  // } else {
  //   stepperz_direction *= -1;
  //   stepperz.setTargetPositionRelativeInRevolutions(stepperz_direction * 0.25);
  // }

  // if (!steppere.motionComplete()) {
  //   steppere.processMovement();
  // } else {
  //   steppere_direction *= -1;
  //   steppere.setTargetPositionRelativeInRevolutions(steppere_direction * 0.25);
  // }

}