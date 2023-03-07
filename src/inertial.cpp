#include "vex.h";

using namespace vex;

void inertialTurnHeading(double targetHeading) {
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