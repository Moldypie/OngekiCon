/* Code based off example code provided by David Madison's GamepadPins example included in the Xinput Library
 * 
 * Before you can use this code, make sure to download needed libraries
 * - https://github.com/dmadison/ArduinoXInput
 * - https://github.com/PaulStoffregen/Encoder
 * 
 * This file was created with Arduino Leonardo board in mind
 * 
 * USAGE:
 * this will cover your basic XInput controller for ONGEKI
 * lever is done using an encoder with a 0 value of straight up. +-87 each direction
 * you will need to have the lever straight up upon each boot of the controller, later I'll do this with a potentiometer so we can avoid this...but for now, good enough.
 * the buttons are standard emulated XInput buttons. Test/Service are still keyboard binds with ONGEKI, so I did not include those buttons
 */

#include <XInput.h>
#include <Encoder.h>

// Lever Setup
float lever = 0;
float old_lever = 0;
float positionL = 0;
float positionR = 0;

// Lever Pins
Encoder enc(0, 1);

// Button Pins
const int Pin_LeftA = 2;
const int Pin_LeftB = 3;
const int Pin_LeftC = 4;
const int Pin_RightA = 5;
const int Pin_RightB = 6;
const int Pin_RightC = 7;
const int Pin_LeftSide = 8;
const int Pin_RightSide = 9;
const int Pin_LeftMenu = 10;
const int Pin_RightMenu = 11;

void setup() {
// Set buttons as inputs, using internal pull-up resistors
  pinMode(Pin_LeftA, INPUT_PULLUP);
  pinMode(Pin_LeftB, INPUT_PULLUP);
  pinMode(Pin_LeftC, INPUT_PULLUP);
  pinMode(Pin_RightA, INPUT_PULLUP);
  pinMode(Pin_RightB, INPUT_PULLUP);
  pinMode(Pin_RightC, INPUT_PULLUP);
  pinMode(Pin_LeftSide, INPUT_PULLUP);
  pinMode(Pin_RightSide, INPUT_PULLUP);
  pinMode(Pin_LeftMenu, INPUT_PULLUP);
  pinMode(Pin_RightMenu, INPUT_PULLUP);
	XInput.setAutoSend(false);  // Wait for all controls before sending
	XInput.begin();
}

void loop() {
  // Lever Setup
  lever = (float)(enc.read());
  
	// Read pin values and store in variables
  boolean leftA = !digitalRead(Pin_LeftA);
  boolean leftB = !digitalRead(Pin_LeftB);
  boolean leftC = !digitalRead(Pin_LeftC);
  boolean rightA = !digitalRead(Pin_RightA);
  boolean rightB = !digitalRead(Pin_RightB);
  boolean rightC = !digitalRead(Pin_RightC);
  boolean leftSide = !digitalRead(Pin_LeftSide);
  boolean rightSide = !digitalRead(Pin_RightSide);
  boolean leftMenu  = !digitalRead(Pin_LeftMenu);
  boolean rightMenu = !digitalRead(Pin_RightMenu);

  // Set XInput DPAD values
  XInput.setDpad(leftB, leftB, leftA, leftC);//leftB listed twice because I needed something to fill that second dpad slot..it's unused in game
  
	// Set XInput buttons
  XInput.setButton(BUTTON_X, rightA);
  XInput.setButton(BUTTON_Y, rightB);
  XInput.setButton(BUTTON_B, rightC);
  XInput.setButton(BUTTON_LB, leftSide);
  XInput.setButton(BUTTON_RB, rightSide);
  XInput.setButton(BUTTON_BACK, leftMenu);
  XInput.setButton(BUTTON_START, rightMenu);

	// Set XInput trigger values
  XInput.setTrigger(TRIGGER_LEFT, positionR);
  XInput.setTrigger(TRIGGER_RIGHT, positionL);

  // Calculate lever values
  if(lever != old_lever){
    // if there's a difference in encoder movement from last pass, move the mouse
    if(lever < old_lever){
      if(lever < 0){
      positionR = -lever;
      }else{
      positionL = lever;
      }
    }else{
      if(lever > 0){
      positionL = lever;
      }else{
      positionR = -lever;
      }
    }
 // we count the difference in the encoders, but we must not go over arduino's int limit
    if(lever < -255){
      enc.write(0);
      old_lever = 0;
    }else if (lever > 255){
      enc.write(0);
      old_lever = 0;
    }else{
      old_lever = lever;
    }
  }
  
	// Send control data to the computer
	XInput.send();
}
