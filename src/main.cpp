/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Thu Sep 26 2019                                           */
/*    Description:  Competition Template                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// Drivetrain           drivetrain    12, 2           
// HoldMotor            motor         1               
// RingMotor            motor         11              
// LockMotor            motor         13              
// LeftDistance         distance      3               
// FrontDistance        distance      4               
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

// A global instance of competition
competition Competition;

// define your global instances of motors and other devices here

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the V5 has been powered on and        */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton(void) {
  // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void lock_arm() {
  HoldMotor.spinFor(forward, 0.3, turns);
  LockMotor.spinFor(forward, 0.3, turns);
}

void unlock_arm() {
  LockMotor.spinFor(reverse, 0.3, turns);
  HoldMotor.spinFor(reverse, 0.3, turns);
}

void autonomous(void) {
  double leftDistance = LeftDistance.objectDistance(mm);
  bool isLeft = leftDistance < 400;
  vex::turnType turnDirection = isLeft ? right : left; // initial turn direction to go to center
  vex::turnType oppositeTurnDirection = isLeft ? left : right;
  Drivetrain.driveFor(reverse, 50, mm);
  // drive to center (tallest) goal
  Drivetrain.turnFor(turnDirection, 45, degrees);
  Drivetrain.driveFor(forward, 1000, mm);
  if (FrontDistance.objectDistance(mm) < 100) {
    // Goal still there
    lock_arm();
    Drivetrain.driveFor(reverse, 1000, mm);
  } else {
    // Goal not there, get the one on our original side
    Drivetrain.turnFor(oppositeTurnDirection, 135, degrees);
    Drivetrain.driveFor(reverse, 1000, mm);
    lock_arm();
    Drivetrain.turnFor(oppositeTurnDirection, 90, degrees);
    Drivetrain.driveFor(reverse, 1000, mm);
  }
 
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

// Twenty for the 2nd layer
double speed = 22;

void print_info() {
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(0,0);
  Controller1.Screen.print(speed);
  Controller1.Screen.print("22 for 1st layer\n, 20 for 2nd layer");
}

void up_presed() {
  speed += 1;
  print_info();
}

void down_presed() {
  speed -= 1;
  print_info();
}

void usercontrol(void) {
  // User control code here, inside the loop
  Controller1.ButtonUp.pressed(up_presed);
  Controller1.ButtonDown.pressed(down_presed);
  while (1) {
    if (Controller1.ButtonL1.pressing()) {
      RingMotor.setVelocity(speed, percent);
      RingMotor.spin(forward);
    } else if (Controller1.ButtonL2.pressing()) {
      RingMotor.setVelocity(speed, percent);
      RingMotor.spin(reverse);
    } else {
      RingMotor.setVelocity(0, percent);
    }
    wait(20, msec); // Sleep the task for a short amount of time to
                    // prevent wasted resources.
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
