/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       Christian Copeland                                        */
/*    Created:      3/15/2023                                                 */
/*    Description:  A+                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    1, 10, D        
// ClawMotor            motor         3               
// ArmMotor             motor         8               
// LeftDrive            motor         2               
// RightDrive           motor         4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  

  // Example: clearing encoders, setting servo positions, ...
}

/*NOTE: rpm command is more accurate than pct based movements. Conditions like heat in the motors may
  cause the onboard ?'FIRMWARE' to scale down the maximum percentages available for movement */

void autonomous(void) {

  /* The '100, deg' arguments for startRotateTo are referencing the degrees of rotation in 
  absolute position - rotating to 50 directly after rotating to 100 will cause the robot 
  to reverse 50 degrees. You can use .resetRotation() to be able to send startRotateTo 
  '100, deg' again and instead of remaining in the position the wheel was in when you first called
  startRotateTo, the wheel will go 100 more degrees forward. At least I hope so, so long as
  .resetRotation() doesn't reverse the wheels back to 0... */

LeftDrive.startRotateTo(100, deg, 100, rpm);     // NOTE: Reference Gear Cartridge in Motor Settings for
RightDrive.startRotateTo(100, deg, 100, rpm);   //rpm range available per motor //

// After each rotateTo, the motors will automatically brake after the rotation

  wait(1, sec);

  LeftDrive.resetRotation();
  RightDrive.resetRotation();
  
  LeftDrive.startRotateTo(50, deg, 100, rpm);
  RightDrive.startRotateTo(50, deg, 100, rpm);

  LeftDrive.resetRotation();
  RightDrive.resetRotation();

  ArmMotor.startRotateTo(100, deg, 100, rpm);
  ClawMotor.startRotateTo(100, deg, 100, rpm);
  ArmMotor.startRotateTo(20, deg, 100, rpm);
  ClawMotor.startRotateTo(20, deg, 100, rpm);

  ArmMotor.resetRotation();
  ClawMotor.resetRotation();
}

void usercontrol(void) {
  // User control code here, inside the loop
  while (1) {
// If the left joystick is not being moved, let the drive motors coast - avoid abrupt stopping
    if(Controller1.Axis3.position(pct) == 0){
      LeftDrive.stop(coast);
      RightDrive.stop(coast);
    }
/* If the left joystick is moved either forward or backward, push the analog signal from the joystick
   to the drive motors for more fluid driving */
    else if(Controller1.Axis3.position(pct) != 0){
        LeftDrive.spin(forward, Controller1.Axis3.position(pct), pct);
        RightDrive.spin(forward, Controller1.Axis3.position(pct), pct);
    }

/* If the right joystick is being moved left or right, push the analog signal from the joystick 
// to the drive motors - same fluidity as left joystick. One must be reverse while the other is
   forward for turning */

/* Note only one 'else if' is required because when forward is given a negative number from Axis1,
   just like the ability to add a negative number to a positive one, it will go 'forward' in the negative 
   direction. The same applies to 'reverse' */
   
    else if(Controller1.Axis1.position(pct) != 0){
      LeftDrive.spin(forward,Controller1.Axis1.position(pct) != 0, pct);
      RightDrive.spin(reverse,Controller1.Axis1.position(pct) != 0, pct);
        }

// If there are no signals coming from the left or right joystick, stop the robot. This may
// result in rough driving, another coast block may fix this if testing shows it is necessary
    else{
      LeftDrive.stop(brake);
      RightDrive.stop(brake);
    }

// Now for arm and claw control.

/* If the R1 button is pressed, push the ArmMotor forward at 100% capacity - may need to lower
   the % if the rpms are too much. RPM commands may be useful in that case */
    if(Controller1.ButtonR1.pressing()){
      ArmMotor.spin(forward, 100, pct);

// If the R2 button is pressed, push the ArmMotor backward at 100% capacity
    }
    else if(Controller1.ButtonR2.pressing()){
      ArmMotor.spin(reverse, 100, pct);
    }

// If no signal is coming from the R1 or R2 buttons, brake the ArmMotor
    else{
      ArmMotor.stop(brake);
    }

/* If the A button is pressed, push the ClawMotor forward at 100% capacity - depending on motor
   type and torque rating, lower pct or RPM commands may be necessary */
    if(Controller1.ButtonA.pressing()){
      ClawMotor.spin(forward, 100, pct);
    }

  //If the B button is pressed, pull the ClawMotor backwards at 100% capacity
    else if(Controller1.ButtonB.pressing()){
      ClawMotor.spin(reverse, 100, pct);
    }

//If no signal is coming from the A or B buttons, brake the ClawMotor
    else{
      ClawMotor.stop(brake);
    }
    wait(10, msec); /* Sleep for a short amount of time , clock cyles per loop can be 
    adjusted for higher or lower 'frames per second' */
  }
}

//
// Main will set up the competition functions and callbacks.
//

int main() {
  // Set up callbacks for autonomous and driver control periods.
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  // Run the pre-autonomous function.
  pre_auton();

  // Prevent main from exiting with an infinite loop.
  while (true) {
    wait(100, msec);
  }
}
