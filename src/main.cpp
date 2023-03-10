/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\amogh                                            */
/*    Created:      Mon Jul 18 2022                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller                    
// LeftFront            motor         1               
// RightFront           motor         13              
// LeftBack             motor         17              
// RightBack            motor         11             
// Inertial             inertial      21              
// Expansion            digital_out   D               
// IntakeRoller         motor         6               
// Flywheel1            motor         12             
// Flywheel2            motor         9               
// Indexer              motor         10              
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"
#include <cmath>
#include <string>

using namespace vex;
// A global instance of competition
competition Competition;
bool limitStatus;
//Function for determining whether input is positive, negative, or 0
void checkLimit(){
  limitStatus = true;

}
void stopLimit(){
  limitStatus = false;
}
int autonselect = 1;
int numOfAutons = 9;

int getSign (double inputValue) {
  if (inputValue > 0){
    return 1;
  }
  else if (inputValue < 0){
    return -1;
  }
  else return 0;
}

/*Our code uses PID, a control loop used to help the robot move efficiently and accurately
without overshooting its target position. PID takes in input based on the sensors
in the V5 Motors and uses a function to output the target speed for the motors. The "P" in PID
stands for proportional. It makes the motors move based on the distance to the target value.
The "I" in PID stands for integral. It calculates how far the motors have already moved to
give it a little push when proportional control cannot get the robot to its final destination.
The "D" in PID stands for derivative. The derivative calculates how fast the robot has been
accelerating and slows it down if it has been accelerating too rapidly. Combined, these
features create a powerful control loop that keeps our robot's performance consistently high.*/
 
//PID to make the robot drive a certain distance during the autonomous period
void PID (double kP, double kI, double kD, double maxIntegral, double tolerance, double maximumSpeed, double minimumSpeed, double target){
  double error = target;
  double derivative = 0;
  double integral = 0;
  double LastError=error;
  double total = 0;
  LeftBack.setPosition(0, turns);
  Inertial.setRotation(0, degrees);
  while(fabs(tolerance)<fabs(error)){
    LeftBack.spin(forward);
    RightBack.spin(forward);
    LeftFront.spin(forward);
    RightFront.spin(forward);
    double SensorValue = LeftBack.position(turns)*3.25*5/3*M_PI;
    error = target - SensorValue;
    integral = integral + error;
    if(fabs(integral)>fabs(maxIntegral)){
      integral=getSign(integral)*maxIntegral;
    }
    derivative = error-LastError;
    LastError = error;
    total = kP*error + kI*integral + kD*derivative;
    double amountOff = Inertial.rotation(degrees);
    if(-1 < amountOff < 1){
      amountOff = 0;
    }
    if(fabs(total) > fabs(maximumSpeed)){
      LeftBack.setVelocity(getSign(total)*maximumSpeed - 0.5*amountOff, percent);
      RightBack.setVelocity(getSign(total)*maximumSpeed + 0.5*amountOff, percent);
      LeftFront.setVelocity(getSign(total)*maximumSpeed - 0.5*amountOff, percent);
      RightFront.setVelocity(getSign(total)*maximumSpeed + 0.5*amountOff, percent);
    }
    else if(fabs(total) < fabs(minimumSpeed)){
      LeftBack.setVelocity(getSign(total)*minimumSpeed - 0.5*amountOff, percent);
      RightBack.setVelocity(getSign(total)*minimumSpeed + 0.5*amountOff, percent);
      LeftFront.setVelocity(getSign(total)*minimumSpeed - 0.5*amountOff, percent);
      RightFront.setVelocity(getSign(total)*minimumSpeed + 0.5*amountOff, percent);
    }
    else{
      LeftBack.setVelocity(total - 0.5*amountOff, percent);
      RightBack.setVelocity(total + 0.5*amountOff, percent);
      LeftFront.setVelocity(total - 0.5*amountOff, percent);
      RightFront.setVelocity(total + 0.5*amountOff,percent);
    }
  }
  LeftBack.stop(brake);
  RightBack.stop(brake);
  RightFront.stop(brake);
  LeftFront.stop(brake);
}
//Void that controls the drivetrain based on inputs from the joysticks

double speedFactor = 50;

void setStopping(vex::brakeType stoppingType) {
  LeftFront.setStopping(stoppingType);
  LeftMiddle.setStopping(stoppingType);
  LeftBack.setStopping(stoppingType);
  RightFront.setStopping(stoppingType);
  RightMiddle.setStopping(stoppingType);
  RightBack.setStopping(stoppingType);
}

void setVelocity(int velocity) {
  LeftFront.setVelocity(velocity, percent);
  LeftBack.setVelocity(velocity, percent);
  LeftMiddle.setVelocity(velocity, percent);
  RightFront.setVelocity(velocity, percent);
  RightBack.setVelocity(velocity, percent);
  RightMiddle.setVelocity(velocity, percent);
}

typedef int turntype;
const turntype left = 1;
const turntype right = 0;


void botTurn(turntype direction, int rotation) {
  LeftFront.spinFor(direction ? reverse : forward, rotation, degrees, false);
  LeftBack.spinFor(direction ? reverse : forward, rotation, degrees, false);
  RightFront.spinFor(direction ? forward : reverse, rotation, degrees, false);
  RightBack.spinFor(direction ? forward : reverse, rotation, degrees, true);
}

void move(vex::directionType direction, int time) {
  /*LeftFront.spinFor(direction, rotation, degrees, false);
  LeftBack.spinFor(direction, rotation, degrees, false);
  RightFront.spinFor(direction, rotation, degrees, false);
  RightBack.spinFor(direction, rotation, degrees, true);*/
  LeftFront.spin(direction);
  RightFront.spin(direction);
  LeftBack.spin(direction);
  RightBack.spin(direction);
  LeftMiddle.spin(direction);
  RightMiddle.spin(direction);
  wait(time, msec);
  LeftFront.stop(hold);
  RightFront.stop(hold);
  LeftBack.stop(hold);
  RightBack.stop(hold);
  LeftMiddle.stop(hold);
  RightMiddle.stop(hold);
  wait(500, msec);
}

void moveWithoutStop(vex::directionType direction, int time) {
  /*LeftFront.spinFor(direction, rotation, degrees, false);
  LeftBack.spinFor(direction, rotation, degrees, false);
  RightFront.spinFor(direction, rotation, degrees, false);
  RightBack.spinFor(direction, rotation, degrees, true);*/
  LeftFront.spin(direction);
  RightFront.spin(direction);
  LeftBack.spin(direction);
  RightBack.spin(direction);
  LeftMiddle.spin(direction);
  RightMiddle.spin(direction);
  wait(time, msec);
}

void turnLeft(int time) {
  LeftFront.spin(reverse);
  LeftMiddle.spin(reverse);
  LeftBack.spin(reverse);
  RightFront.spin(fwd);
  RightMiddle.spin(fwd);
  RightBack.spin(fwd);
  wait(time, msec);
  LeftFront.stop(hold);
  LeftMiddle.stop(hold);
  LeftBack.stop(hold);
  RightFront.stop(hold);
  RightMiddle.stop(hold);
  RightBack.stop(hold);
}

void turnRight(int time) {
  LeftFront.spin(fwd);
  LeftMiddle.spin(fwd);
  LeftBack.spin(fwd);
  RightFront.spin(reverse);
  RightMiddle.spin(reverse);
  RightBack.spin(reverse);
  wait(time, msec);
  LeftFront.stop(hold);
  LeftMiddle.stop(hold);
  LeftBack.stop(hold);
  RightFront.stop(hold);
  RightMiddle.stop(hold);
  RightBack.stop(hold);
}

void moveLeftDrivetrain(vex::directionType direction) {
  LeftFront.spin(direction);
  LeftMiddle.spin(direction);
  LeftBack.spin(direction);
}

void moveRightDrivetrain(vex::directionType direction) {
  RightFront.spin(direction);
  RightMiddle.spin(direction);
  RightBack.spin(direction);
}

void drivetrainStop(void) {
  LeftFront.stop(hold);
  LeftMiddle.stop(hold);
  LeftBack.stop(hold);
  RightFront.stop(hold);
  RightMiddle.stop(hold);
  RightBack.stop(hold);
}

void botTurn3Motor(turntype direction, int rotation) {
  LeftFront.spinFor(direction ? reverse : forward, rotation, degrees, false);
  LeftBack.spinFor(direction ? reverse : forward, rotation, degrees, false);
  RightFront.spinFor(direction ? forward : reverse, rotation, degrees, true);
}

void botTurn2Motor(turntype direction, int rotation) {
  LeftBack.spinFor(direction ? reverse : forward, rotation, degrees, false);
  RightFront.spinFor(direction ? reverse : forward, rotation, degrees, true);
}

void platformMode() {
  if(Controller1.ButtonY.pressing()){
    speedFactor = 6;
    LeftFront.setStopping(hold);
    LeftBack.setStopping(hold);
    RightFront.setStopping(hold);
    RightBack.setStopping(hold);
  }
  else {
    speedFactor = 1;
    LeftFront.setStopping(coast);
    LeftBack.setStopping(coast);
    RightFront.setStopping(coast);
    RightBack.setStopping(coast);
  }
}

/*void simpleDrive(){
  double forwardAmount = Controller1.Axis3.position();
  double turnAmount = Controller1.Axis4.position(); //Axis 4 for unified joystick
  RightFront.spin(forward, (forwardAmount-turnAmount) / speedFactor, percent);
  RightBack.spin(forward, (forwardAmount-turnAmount) / speedFactor, percent);
  RightMiddle.spin(forward, (forwardAmount-turnAmount) / speedFactor, percent);
  LeftFront.spin(forward, (forwardAmount+turnAmount) / speedFactor, percent);
  LeftBack.spin(forward, (forwardAmount+turnAmount) / speedFactor, percent);
  LeftMiddle.spin(forward, (forwardAmount+turnAmount) / speedFactor, percent);
}*/
void simpleDrive() {
  double forwardAmount = Controller1.Axis3.position();
  double turnAmount = Controller1.Axis4.position();
  double deadband = 10.0; // adjust this value to change the deadband size
  double forwardPower = 0.0;
  double turnPower = 0.0;
  if (abs(forwardAmount) > deadband) {
    forwardPower = (forwardAmount - (abs(forwardAmount) / forwardAmount * deadband)) / speedFactor;
  }
  if (abs(turnAmount) > deadband) {
    turnPower = (turnAmount - (abs(turnAmount) / turnAmount * deadband)) / speedFactor;
  }
  RightFront.spin(forward, (forwardPower-turnPower), percent);
  RightBack.spin(forward, (forwardPower-turnPower), percent);
  RightMiddle.spin(forward, (forwardPower-turnPower), percent);
  LeftFront.spin(forward, (forwardPower+turnPower), percent);
  LeftBack.spin(forward, (forwardPower+turnPower), percent);
  LeftMiddle.spin(forward, (forwardPower+turnPower), percent);
}

void catapultStop() {
  catapult.stop(hold);
}

void catapultMovement() {
  catapult.setStopping(hold);
  catapult.setVelocity(100, pct);
  if(Controller1.ButtonX.pressing()) {
    if(!LimitSwitchH.pressing()) {
      catapult.spin(fwd);
    }
  }
  else if(Controller1.ButtonY.pressing()) {
    catapult.spinFor(fwd, 200, degrees, true);
    if(!LimitSwitchH.pressing()) {
      catapult.spin(fwd);
    }
  }
  else if(!Controller1.ButtonX.pressing() && !Controller1.ButtonY.pressing()) {
    if(LimitSwitchH.pressing()) {
      catapult.stop();
    }
  }
}

/*void Forward(){
  LeftFront.setStopping(coast);
  LeftMiddle.setStopping(hold);
  LeftBack.setStopping(hold);
  RightFront.setStopping(hold);
  RightMiddle.setStopping(hold);
  RightBack.setStopping(hold);

  double currentVel = vel;
  double sign = getSign(dist);
  double target = initalPos+InchtoDegree(dist);
  double initialAngle = Intertial.rotation(degrees);
}*/

bool Controller1XY = true;

double fly_kp = 0.25; // how fast it increases
double fly_ki = 0.3; // how much offshoot/range of fluctuation
double fly_kd = 0.00005; // how many fluctuations are there
double speed_margin = 0;
double speed_marg_pct = 2;
bool flyescvar = false;
double speed_volt = 0;

//flywheel spin

void flywheel_spin_fwd(double flywheel_target_speed_pct) {
  
  Flywheel1.setVelocity(flywheel_target_speed_pct, pct);
  Flywheel2.setVelocity(flywheel_target_speed_pct, pct);
  Flywheel1.spin(directionType::fwd);
  Flywheel2.spin(directionType::rev);
}



//flywheel spin PID code
void flywheel_spin_fwd_PID(double flywheel_target_speed_pct){
  //speed_volt = 0;
double averagevolt = 0;
double preverror = 0;
double errorsum = 0;
double error = 0;
double derivative = 0;
double flywheel_target_speed_volt = (flywheel_target_speed_pct/100)*12;
Controller1.Screen.setCursor(3, 9);
Controller1.Screen.print("    ");
Controller1.Screen.setCursor(1,1);
Controller1.Screen.print("         ");
wait(10,msec);
 
 while (flyescvar == false) {
    averagevolt = ((Flywheel1.voltage() - Flywheel2.voltage()) / 2);
    //averagevolt = ((flywheelMotorA.velocity(velocityUnits::pct) + flywheelMotorB.velocity(velocityUnits::pct) ) / 2);
    error = flywheel_target_speed_volt - averagevolt;
    derivative = preverror - error;
    errorsum += error;
    preverror = error;
    speed_margin = fabs((error/flywheel_target_speed_volt) * 100);
    speed_volt =  error * fly_kp + fly_ki * errorsum + fly_kd * derivative;
  
    Controller1.Screen.setCursor(1,1);
    Controller1.Screen.print("C:%2.1fM:%2.0f", averagevolt,speed_margin);
    wait(5, msec);
  
    if(speed_margin <= speed_marg_pct) {
      flyescvar = true;
    } else {
        //flywheelMotorA.spin(forward, speed_volt, velocityUnits::pct);
        //flywheelMotorB.spin(forward, speed_volt, velocityUnits::pct);
        Flywheel1.spin(forward, speed_volt, volt);
        Flywheel2.spin(reverse, speed_volt, volt);
    }
    wait(10, msec);
  }
 Controller1.Screen.setCursor(3,9);
 Controller1.Screen.print("DONE");
 wait(5,msec);
 
 // Maintain the speed
 //flywheelMotorA.spin(forward, speed_volt, velocityUnits::pct);
 //flywheelMotorB.spin(forward, speed_volt, velocityUnits::pct);
 //Flywheel1.spin(forward, speed_volt, volt);
 //Flywheel2.spin(reverse, speed_volt, volt);
}

double preverror = 0;
double errorsum = 0;
double error = 0;
double derivative = 0;
void flyPIDadjustment(double flywheel_target_speed_pct) {
  double averagevolt = 0;
  
  double flywheel_target_speed_volt = (flywheel_target_speed_pct/100)*12;
  //Controller1.Screen.setCursor(3, 9);
  //Controller1.Screen.print("    ");
  //Controller1.Screen.setCursor(1,1);
  //Controller1.Screen.print("         ");
  //wait(10,msec);

  averagevolt = ((Flywheel1.voltage() - Flywheel2.voltage()) / 2);
  error = flywheel_target_speed_volt - averagevolt;
  derivative = preverror - error;
  errorsum += error;
  preverror = error;
  speed_margin = fabs((error/flywheel_target_speed_volt) * 100);
  speed_volt =  error * fly_kp + fly_ki * errorsum + fly_kd * derivative;
  
  Flywheel1.spin(forward, speed_volt, volt);
  Flywheel2.spin(reverse, speed_volt, volt);
}

void expansionMovement(void) {
  if((Controller1.ButtonUp.pressing()) && (Controller1.ButtonDown.pressing()) && (Controller1.ButtonLeft.pressing()) && (Controller1.ButtonRight.pressing())) {
    Expansion.set(true);
    Expansion2.set(true);
  } else {
    Expansion.set(false);
    Expansion2.set(false);
  }
}
void pistonIndexerMovement(void) {
  if(Controller1.ButtonL1.pressing()) {
    pneumaticsIndexer.set(false);
    wait(200, msec);
    pneumaticsIndexer.set(true);
    wait(200, msec);
  } else if (Controller1.ButtonL2.pressing()) {
    pneumaticsIndexer.set(false);
    //wait(200, msec);
    pneumaticsIndexer.set(true);
    wait(200, msec);
    pneumaticsIndexer.set(false);
    wait(1200, msec);
    pneumaticsIndexer.set(true);
    wait(200, msec);
    pneumaticsIndexer.set(false);
    wait(1500, msec);
    pneumaticsIndexer.set(true);
    wait(200, msec);
  } else {
    pneumaticsIndexer.set(false);
  }
}

void TempBattery() {
  wait(500, msec);
  
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Motor Temps (%)");
  Controller1.Screen.newLine();
  Controller1.Screen.print(catapult.temperature(percent));
  Controller1.Screen.print(",");
  //Controller1.Screen.print(LeftBack.temperature(percent));
  //Controller1.Screen.print(",");
  Controller1.Screen.print(LeftFront.temperature(percent));
  Controller1.Screen.print(",");
  Controller1.Screen.print(IntakeRoller.temperature(percent));
  Controller1.Screen.print(",");
  /*Controller1.Screen.print(RightBack.temperature(percent));
  Controller1.Screen.print(",");
  Controller1.Screen.print(RightFront.temperature(percent));
  Controller1.Screen.print(",");*/
  Controller1.Screen.newLine();
  Controller1.Screen.print("Battery: ");
  Controller1.Screen.print(vexBatteryCapacityGet());
}

void intakeRollerMovement() {
  if(Controller1.ButtonR2.pressing()){
    IntakeRoller.setVelocity(100, percent);
    IntakeRoller.spin(forward);
  }
  else if(Controller1.ButtonR1.pressing()){
    IntakeRoller.setVelocity(100, percent);
    IntakeRoller.spin(reverse);
  }
  else{
    IntakeRoller.setStopping(coast);
    IntakeRoller.stop();
  }
}

void slowRollerMovement() {
  if(Controller1.ButtonL1.pressing()) {
    IntakeRoller.setVelocity(50, pct);
    IntakeRoller.spin(fwd);
  } else {
    IntakeRoller.setStopping(hold);
    IntakeRoller.stop();
  }
}

void flywheelSpin(double velocity) {
  Flywheel1.setVelocity(velocity, pct);
  Flywheel2.setVelocity(velocity, pct);
  Flywheel1.spin(fwd);
  Flywheel2.spin(reverse);
}

bool flywheelStart = false;
int flywheelVelocity = 80;

void flywheelRun() {
  flywheelStart = !flywheelStart;
  if(flywheelStart){ // fast shooter
    Flywheel1.setVelocity(flywheelVelocity, percent);
    Flywheel2.setVelocity(flywheelVelocity, percent);
    Flywheel1.spin(forward);
    Flywheel2.spin(reverse);
  } else {
    Flywheel1.setStopping(coast);
    Flywheel2.setStopping(coast);
    Flywheel1.stop();
    Flywheel2.stop();
  }
}

void flywheelSlow() {
    flywheelVelocity = 70;
    flywheelRun();
}

void flywheelFast() {
    flywheelVelocity = 80;
    flywheelRun();
}

void flywheelPIDFast(){
  flywheelStart = !flywheelStart;
  if(flywheelStart) {
    flywheel_spin_fwd_PID(69);
  } else {
    Flywheel1.setStopping(coast);
    Flywheel2.setStopping(coast);
    Flywheel1.spin(forward, 0, volt);
    Flywheel2.spin(reverse, 0, volt);
    flyescvar = false;
    //Flywheel1.stop();
    //Flywheel2.stop();
  }
}

void flywheelPIDSlow() {
  flywheelStart = !flywheelStart;
  if(flywheelStart) {
    flywheel_spin_fwd_PID(57);
  } else {
    Flywheel1.setStopping(coast);
    Flywheel2.setStopping(coast);
    Flywheel1.spin(forward, 0, volt);
    Flywheel2.spin(reverse, 0, volt);
    flyescvar = false;
    //Flywheel1.stop();
    //Flywheel2.stop();
  }
}

void flywheelPIDmovement() {
  if(Controller1.ButtonX.pressing()) {
    flyPIDadjustment(90);
  }
  else if(Controller1.ButtonY.pressing()) {
    flyPIDadjustment(75);
  }
  else if(!Controller1.ButtonX.pressing() && !Controller1.ButtonY.pressing()) {
    Flywheel1.stop();
    Flywheel2.stop();
  }
}

void flywheelMovement() {
    /*
    Controller1.ButtonY.pressed(flywheelFast);
    Controller1.ButtonX.pressed(flywheelSlow);
    */
    /*
    if (Controller1.ButtonY.pressing()) {
       flywheel_spin_fwd_PID(70);
       Controller1XY = false;
     } else if (Controller1.ButtonX.pressing()) {
       flywheel_spin_fwd_PID(35);
       Controller1XY = false;
     } else if (!Controller1XY) {
       flyescvar = true;
       speed_volt = 0;
       wait(20,msec);
       Flywheel1.stop();
       Flywheel2.stop();
       flyescvar = false;
     }
     */
     
    if(Controller1.ButtonY.pressing()){
      Flywheel1.setVelocity(81, pct);
      Flywheel2.setVelocity(81, pct);
      Flywheel1.spin(forward);
      Flywheel2.spin(reverse);
      //flyescvar = false;
      //flywheel_spin_fwd_PID(85);
      Controller1XY = false;
    } else if(Controller1.ButtonX.pressing()) {
      /*Flywheel1.setVelocity(77, pct);
      Flywheel2.setVelocity(77, pct);
      Flywheel1.spin(forward);
      Flywheel2.spin(reverse);*/
      Flywheel1.setVelocity(77, pct);
      Flywheel2.setVelocity(77, pct);
      Flywheel1.spin(fwd);
      Flywheel2.spin(reverse);
      Controller1XY = false;
    } else if(!Controller1XY) {
      Flywheel1.setStopping(coast);
      Flywheel2.setStopping(coast);
      Flywheel1.stop();
      Flywheel2.stop();
      Controller1XY = true;
      flyescvar = false;
    }
}

void indexerMovement() {
  if(Controller1.ButtonL1.pressing()) {
    Indexer.setVelocity(100, percent);
    Indexer.spinFor(forward, 115, degrees, true);
    Indexer.spinFor(reverse, 115, degrees, true);
  }
  else if (Controller1.ButtonL2.pressing()) {
    Indexer.setVelocity(100,percent);
    Indexer.spin(reverse);
  }
  else {
    Indexer.setStopping(hold);
    Indexer.stop();
  }
}

/*void turnCounterClockwise(double amount){
  Inertial.setRotation(0, degrees);
  while(fabs(Inertial.rotation(degrees)) < amount){
    double error = amount - fabs(Inertial.rotation(degrees));
    LeftBack.spin(reverse, 0.2*error + 5, percent);
    RightBack.spin(forward, 0.2*error + 5, percent);
    LeftFront.spin(reverse, 0.2*error + 5, percent);
    RightFront.spin(forward, 0.2*error + 5, percent);
    wait(5, msec);
  }
  LeftBack.setStopping(hold);
  RightBack.setStopping(hold);
  RightFront.setStopping(hold);
  LeftFront.setStopping(hold);
  LeftBack.stop();
  RightBack.stop();
  RightFront.stop();
  LeftFront.stop();
  wait(0.5, sec);
}*/

/*void turnClockwise(double amount){
  Inertial.setRotation(0, degrees);
  while(fabs(Inertial.rotation(degrees))< amount){
    double error = amount - fabs(Inertial.rotation(degrees));
    LeftBack.spin(forward, 0.2*error + 5, percent);
    RightBack.spin(reverse, 0.2*error + 5, percent);
    LeftFront.spin(forward, 0.2*error + 5, percent);
    RightFront.spin(reverse, 0.2*error + 5, percent);
    wait(5, msec);
  }
  LeftBack.setStopping(hold);
  RightBack.setStopping(hold);
  RightFront.setStopping(hold);
  LeftFront.setStopping(hold);
  LeftBack.stop();
  RightBack.stop();
  RightFront.stop();
  LeftFront.stop();
  wait(0.5, sec);
}*/

void moveDrivetrain(float vel, int dist, bool smooth, bool sync) {
  LeftFront.setStopping(coast);
  LeftBack.setStopping(coast);
  RightFront.setStopping(coast);
  RightBack.setStopping(coast); 
  LeftFront.setVelocity(vel, percent);
  LeftBack.setVelocity(vel, percent);
  RightFront.setVelocity(vel, percent);
  RightBack.setVelocity(vel, percent);

  if (smooth) {
    LeftBack.setPosition(0, degrees);

    LeftFront.spin(forward);
    LeftBack.spin(forward);
    RightFront.spin(forward);
    RightBack.spin(forward);

    while (std::abs(LeftBack.position(degrees)) < std::abs(dist)) {
      wait(10, msec);
    }

    LeftFront.stop();
    LeftBack.stop();
    RightFront.stop();
    RightBack.stop();

  } else {
    LeftFront.spinFor(forward, dist, degrees, false);
    LeftBack.spinFor(forward, dist, degrees, false);
    RightFront.spinFor(forward, dist, degrees, false);
    RightBack.spinFor(forward, dist, degrees, sync);
  }
}

//----------------------------------------------------------------------------------

int selected = 0;
std::string autons[9] = {"Disabled", "Normal", "1 Roller + Low Goal", "1 Roller", "Roller Other Side", "Disc Shooter Two", "Skills", "Inertial Test", "PID test"};
int size = sizeof(autons);

bool elevated = false;

void autonSelector(){
  Controller1.Screen.clearScreen();
  task::sleep(100);
  while(true){
    Controller1.Screen.clearScreen();
    task::sleep(100);
    Controller1.Screen.clearLine(2);
    Controller1.Screen.setCursor(2,1);
    Controller1.Screen.print((autons[selected] + ",").c_str());
    Controller1.Screen.newLine();
    Controller1.Screen.print((elevated ? "Elevated" : "Default"));
    task::sleep(100);
     if(Controller1.ButtonRight.pressing()){
      elevated = !elevated;
        if (!elevated) {
          selected = (selected + 1 + size) % size;
        }
     }else if(Controller1.ButtonLeft.pressing()){
       elevated = !elevated;
       if (elevated) {
        selected = (selected - 1 + size) % size;
       }
     }else if(Controller1.ButtonA.pressing()){
       task::sleep(100);
       if(Controller1.ButtonA.pressing()){
         goto slctEnd;
       }
     }
   }
   slctEnd:
   Controller1.rumble("..");
}

void autonIndexer(void) {
  pneumaticsIndexer.set(true);
  wait(200, msec);
  pneumaticsIndexer.set(false);
  wait(200, msec);
}

void pre_auton(void) {
 // Initializing Robot Configuration. DO NOT REMOVE!
  vexcodeInit();
  pneumaticsIndexer.set(false);
  Expansion.set(false);
  Expansion2.set(false);
  calibrateInertialSensor();
  wait(1, sec);
  autonSelector();

}
/*---------------------------------------------------------------------------*/
/*                              Autonomous Task                              */
/*  This task is used to control the robot during the autonomous phase of    */
/*  a VEX Competition.                                                       */
/*---------------------------------------------------------------------------*/
void autonomous(void) {
  vex::task PIDoperation(drivePID);
  int x = 980; // Degrees for moving forward to the neutral goal
  switch(selected){
    case 0:{ //Disabled
      break;
    }
    case 1:{ // Normal
      IntakeRoller.setVelocity(100, percent);
      //IntakeRoller.spinFor(reverse, 5000, degrees, false);
      setVelocity(10);
      move(fwd, 850);
      IntakeRoller.spin(forward);
      wait(250, msec);
      IntakeRoller.stop();
      move(reverse, 850);
      turnCounterClockwise(86.5);
      setVelocity(100);
      moveWithoutStop(forward, 500);
      turnCounterClockwise(85);
      catapult.spin(fwd, 100, pct);
      LimitSwitchH.pressed(catapultStop);
      IntakeRoller.spin(reverse);
      move(fwd, 500);
      turnClockwise(37);
      setVelocity(30);
      move(fwd, 3000);
      setVelocity(15);
      move(fwd, 200);
      turnClockwise(50);
      IntakeRoller.stop();
      IntakeRoller.spin(forward);
      move(fwd, 450);
      wait(400, msec);
      IntakeRoller.stop();
      /*IntakeRoller.spin(forward);
      catapult.spinFor(fwd, 2265, degrees, false);
      move(forward, 300);
      wait(15, msec);
      IntakeRoller.stop();
      move(reverse, 425);
      turnLeft(825);
      wait(100, msec);
      IntakeRoller.spin(reverse);
      move(forward, 1250);
      wait(1 , sec);
      IntakeRoller.stop();
      turnRight(690);
      wait(100, msec);
      move(reverse, 400);
      wait(100, msec);
      catapult.spinFor(forward, 150, degrees, true);
      catapult.spinFor(forward, 1985, degrees, false);
      move(fwd, 200);
      wait(100, msec);
      turnLeft(710);
      wait(100, msec);
      IntakeRoller.spin(reverse);
      move(fwd, 400);
      turnRight(630);
      wait(100, msec);
      move(reverse, 445);
      wait(1250, msec);
      IntakeRoller.stop();
      catapult.spinFor(forward, 150, degrees, true);*/
      break;
    }
    case 2: { //1 Roller + Low Goal
        setStopping(hold);
        setVelocity(10);
        //catapult.spinFor(fwd, 2265, degrees, false);
        catapult.spin(fwd, 100, pct);
        LimitSwitchH.pressed(catapultStop);
        move(forward, 750); 
        IntakeRoller.spin(forward); //roller
        wait(225, msec);
        IntakeRoller.stop(); 
        move(reverse, 625);
        turnCounterClockwise(130);
        IntakeRoller.setVelocity(100, pct);
        IntakeRoller.spin(reverse);
        setVelocity(50);
        move(fwd, 750);
        setVelocity(15);
        move(fwd, 2000);
        IntakeRoller.stop();
        turnRight(50);
        IntakeRoller.spin(forward);
        setVelocity(50);
        move(fwd, 1700);
        turnCounterClockwise(50);
        setVelocity(15);
        move(fwd, 1650);
        move(reverse, 750);
        turnClockwise(80);
      break;
    }
    case 3: { //1 Roller
      IntakeRoller.setVelocity(100, percent);
      //IntakeRoller.spinFor(reverse, 5000, degrees, false);
      setVelocity(10);
      move(fwd, 850);
      IntakeRoller.spin(forward);
      wait(250, msec);
      IntakeRoller.stop();
      move(reverse, 850);
      break; 
    }
    case 4: { //Roller Other Side
      IntakeRoller.setVelocity(100, pct);
      catapult.spin(fwd, 100, pct);
      LimitSwitchH.pressed(catapultStop);
      setVelocity(50);
      IntakeRoller.spin(reverse);
      move(fwd, 300);
      turnClockwise(40);
      setVelocity(15);
      move(fwd, 1250);
      wait(0.5, seconds);
      move(reverse, 1050);
      IntakeRoller.stop();
      turnClockwise(83.5);
      setVelocity(30);
      move(fwd, 1250);
      turnClockwise(40);
      move(fwd, 250);
      IntakeRoller.spin(forward);
      wait(250, msec);
      IntakeRoller.stop();
      break;
    }
    case 5: { //Disc Shooter 2
      IntakeRoller.setVelocity(100, percent);
      IntakeRoller.spinFor(reverse, 400, degrees, false);

      move(forward, 200);
      //move(reverse, 200);

      flywheelSpin(85);

      //wait(2500, msec);

      LeftFront.spin(reverse);
      LeftBack.spin(reverse);
      RightFront.spin(reverse);
      RightBack.spin(reverse);

      wait(250, msec);
      
      LeftFront.stop();
      LeftBack.stop();
      RightFront.stop();
      RightBack.stop();

      wait(500, msec);

      LeftFront.spin(reverse);
      LeftBack.spin(reverse);
      RightFront.spin(fwd);
      RightBack.spin(fwd);

      wait (1200, msec);

      LeftFront.stop();
      LeftBack.stop();
      RightFront.stop();
      RightBack.stop();

      wait (300, msec);

      IntakeRoller.spin(forward);

      move(forward, 1650);

      move(forward, 1200);

      //IntakeRoller.stop();

      LeftFront.spin(reverse);
      LeftBack.spin(reverse);
      RightFront.spin(fwd);
      RightBack.spin(fwd);

      wait (900, msec);

      LeftFront.stop();
      LeftBack.stop();
      RightFront.stop();
      RightBack.stop();

      wait (300, msec);
      move(fwd, 500);
      IntakeRoller.stop();

      autonIndexer();
      wait(2000, msec);
      autonIndexer();
      wait(2000, msec);
      autonIndexer();
      wait(1000, msec);
      autonIndexer();

      wait(2, sec);

      Flywheel1.stop();
      Flywheel2.stop();
      break;
    }
    case 6: { // Skills
    IntakeRoller.setVelocity(100, percent);
    setStopping(hold);
    setVelocity(10);
    //catapult.spinFor(fwd, 2265, degrees, false);
    catapult.spin(fwd, 100, pct);
    LimitSwitchH.pressed(catapultStop);
    move(forward, 750); 
    IntakeRoller.spin(forward); //roller
    wait(200, msec);
    IntakeRoller.stop(); 
    move(reverse, 625); 
    //wait(20, msec);
    setVelocity(15); // turn to get 3rd disc
    turnRight(1150);
    wait(20, msec);
    IntakeRoller.spin(reverse); // 2nd roller
    move(forward, 2250);
    IntakeRoller.stop();
    setVelocity(5);
    turnLeft(350);
    move(forward, 850);
    //wait(10, msec);
    IntakeRoller.spin(forward);
    wait(500, msec);
    IntakeRoller.stop();
    move(reverse, 1100); // moving to shoot in high goal
    setVelocity(25);
    turnCounterClockwise(111);
    wait(100, msec);
    setVelocity(25);
    moveWithoutStop(reverse, 500);
    setVelocity(37);
    moveWithoutStop(reverse, 500);
    setVelocity(50);
    moveWithoutStop(reverse, 500);
    wait(500, msec);
    turnRight(175);
    wait(100, msec);
    catapult.spin(fwd); // shoots discs
    LimitSwitchH.pressed(catapultStop);
    wait(300, msec);
    setVelocity(15);
    move(reverse, 2000);
    turnClockwise(90.5);
    Expansion.set(true);
    Expansion2.set(true);
    /*turnLeft(175); // getting 2nd set of 3 discs
    wait(50, msec);
    IntakeRoller.spin(reverse);
    move(fwd, 1300);
    setVelocity(15);
    turnCounterClockwise(115);
    move(fwd, 4800);
    turnClockwise(120);
    setVelocity(25); // heading to goal
    moveWithoutStop(reverse, 230);
    setVelocity(37);
    moveWithoutStop(reverse, 230);
    setVelocity(50);
    moveWithoutStop(reverse, 315);
    wait(500, msec);
    turnCounterClockwise(86.5); // lining up
    IntakeRoller.stop();
    move(reverse, 700);
    turnCounterClockwise(30);*/
    break;
  }
  case 7: { //inertial test
     turnClockwise(86.5);
     wait(1, seconds);
     break;
  } 
  case 8: {
    // move forward to the roller
    enableDrivePID = true;
    desiredValue = 300;
    IntakeRoller.spin(forward);
    wait(100, msec);
    enableDrivePID = false;
    IntakeRoller.stop();
    turnClockwise(86.5);
    enableDrivePID = true;
    desiredValue = 500;
    wait(500, msec);
    desiredValue = 0;
    break;
  }
} // switch
} // autonomous


/*---------------------------------------------------------------------------*/
/*                              User Control Task                            */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*---------------------------------------------------------------------------*/
void usercontrol(void) {
  enableDrivePID = false;
 // User control code here, inside the loop
  while (1) {
    simpleDrive();
    //TempBattery();
    intakeRollerMovement();
    //slowRollerMovement();
    pistonIndexerMovement();
    expansionMovement();
    //flywheelPIDmovement();
    catapultMovement();
    /*while(limitStatus==true){
      if(!LimitSwitchH.pressing()){
        catapult.spin(forward, 70, percent);
      }
      if(LimitSwitchH.pressing()){
        catapult.stop(hold);
        stopLimit();
      }
    }*/
    //indexerMovement();
    platformMode();
    /*if(Controller1.ButtonLeft.pressing() && Controller1.ButtonRight.pressing()){
      RightLift.stop(hold);
      while((Controller1.ButtonY.pressing() && Controller1.ButtonA.pressing()) == false){
        //goSlow();
        wait(10, msec);
      }
    }*/
    wait(15, msec);
  } // Sleep the task for a short amount of time to prevent wasted resources.
}
int main() {
 // Set up callbacks for autonomous and driver control periods.
 Competition.autonomous(autonomous);
 Competition.drivercontrol(usercontrol);
 pre_auton();
 // Prevent main from exiting with an infinite loop.
 while (true) {
   wait(100, msec);
 }
}
//test from windows
//test from mac
