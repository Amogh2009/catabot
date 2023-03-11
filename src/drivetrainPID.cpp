#include "vex.h"

using namespace vex;

bool enableDrivePID = true;
int desiredValue = 200;
int desiredTurnValue = 0;

int drivePID () {

  double kP = 0.0;
  double kI = 0.0;
  double kD = 0.0;

  double turnVolts = 0;

  int error; //SensorValue - DesiredValue : Positional Value
  int prevError = 0; //Position 20 miliseconds ago
  int derivative;
  int totalError = 0;


  while(enableDrivePID) {
    
    int leftMotorPosition = LeftFront.position(degrees);
    int rightMotorPosition = RightFront.position(degrees);
    int averagePosition = (leftMotorPosition + rightMotorPosition) / 2;

    // Potential
    error = averagePosition - desiredValue;
    // Derivative
    derivative = error - prevError;
    // Integral
    totalError += error;

    double lateralMotorPower = error * kP + derivative * kD + totalError * kI;

    LeftFront.spin(forward, lateralMotorPower + turnVolts, voltageUnits::volt);
    LeftMiddle.spin(forward, lateralMotorPower + turnVolts, voltageUnits::volt);
    LeftBack.spin(forward, lateralMotorPower + turnVolts, voltageUnits::volt);
    RightFront.spin(forward, lateralMotorPower - turnVolts, voltageUnits::volt);
    RightMiddle.spin(forward, lateralMotorPower - turnVolts, voltageUnits::volt);
    RightBack.spin(forward, lateralMotorPower - turnVolts, voltageUnits::volt);





    prevError = error;
    vex::task::sleep(20);


  }

return 1;
}