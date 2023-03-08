#include "vex.h";

using namespace vex;

void calibrateInertialSensor(void) {
  Inertial.startCalibration(0);

  while(Inertial.isCalibrating()) {
    joystick.Screen.clearLine(3);
    joystick.Screen.print("calibrating");
    wait(20, msec);
  }

  joystick.Screen.clearLine(3);
  joystick.Screen.print("done");
  joystick.rumble("...");
  wait(10, msec);
}

void turnClockwise(double amount) {
  Inertial.setRotation(0, degrees);
  while(fabs(Inertial.rotation(degrees)) < amount) {
    double error = amount - fabs(Inertial.rotation(degrees));
    
    LeftFront.spin(forward, 0.2*error + 5, percent);
    LeftMiddle.spin(forward, 0.2*error + 5, percent);
    LeftBack.spin(forward, 0.2*error + 5, percent);
    RightFront.spin(reverse, 0.2*error + 5, percent);
    RightMiddle.spin(reverse, 0.2*error + 5, percent);
    RightBack.spin(reverse, 0.2*error + 5, percent);
    wait(5, msec);
  }
  setStopping(hold);
  drivetrainStop();
  wait(0.5, sec);
  Inertial.setRotation(0, degrees);
}

void turnCounterClockwise(double amount) {
  Inertial.setRotation(0, degrees);
  while(fabs(Inertial.rotation(degrees)) < amount) {
    double error = amount - fabs(Inertial.rotation(degrees));
    
    LeftFront.spin(reverse, 0.2*error + 5, percent);
    LeftMiddle.spin(reverse, 0.2*error + 5, percent);
    LeftBack.spin(reverse, 0.2*error + 5, percent);
    RightFront.spin(forward, 0.2*error + 5, percent);
    RightMiddle.spin(forward, 0.2*error + 5, percent);
    RightBack.spin(forward, 0.2*error + 5, percent);
    wait(5, msec);
  }
  setStopping(hold);
  drivetrainStop();
  wait(0.5, sec);
  Inertial.setRotation(0, degrees);
}

void inertialTurnClockwise(double targetHeading) {
  double currentError = targetHeading - Inertial.heading();
  double Kp = 0.5;
  double motorSpeed;

  while(fabs(currentError) < 4) {
    motorSpeed = Kp * currentError;

    LeftFront.spin(fwd, motorSpeed, pct);
    LeftMiddle.spin(fwd, motorSpeed, pct);
    LeftBack.spin(fwd, motorSpeed, pct);
    RightFront.spin(reverse, motorSpeed, pct);
    RightMiddle.spin(reverse, motorSpeed, pct);
    RightBack.spin(reverse, motorSpeed, pct);

    currentError = targetHeading - Inertial.heading();
  }
  LeftFront.stop(hold);
  LeftMiddle.stop(hold);
  LeftBack.stop(hold);
  RightFront.stop(hold);
  RightMiddle.stop(hold);
  RightBack.stop(hold); 
} 

void inertialTurnCounterClockwise(double targetHeading) {
  double currentError = targetHeading - Inertial.heading();
  double Kp = 0.5;
  double motorSpeed;

  while(fabs(currentError) < 4) {
    motorSpeed = Kp * currentError;

    LeftFront.spin(reverse, motorSpeed, pct);
    LeftMiddle.spin(reverse, motorSpeed, pct);
    LeftBack.spin(reverse, motorSpeed, pct);
    RightFront.spin(fwd, motorSpeed, pct);
    RightMiddle.spin(fwd, motorSpeed, pct);
    RightBack.spin(fwd, motorSpeed, pct);

    currentError = targetHeading - Inertial.heading();
  }
  LeftFront.stop(hold);
  LeftMiddle.stop(hold);
  LeftBack.stop(hold);
  RightFront.stop(hold);
  RightMiddle.stop(hold);
  RightBack.stop(hold); 
}