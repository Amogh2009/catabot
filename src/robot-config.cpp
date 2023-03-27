#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;
controller joystick;
// VEXcode device constructors
controller Controller1 = controller(primary);
motor LeftFront = motor(PORT16, ratio18_1, true);
motor RightFront = motor(PORT19, ratio18_1, true);
motor LeftBack = motor(PORT13, ratio18_1, false);
motor RightBack = motor(PORT17, ratio18_1, true);
motor LeftMiddle = motor(PORT15, ratio18_1, false);
motor RightMiddle = motor(PORT18, ratio18_1, true);
motor catapult = motor(PORT12, ratio18_1, true);
inertial Inertial = inertial(PORT20);
limit LimitSwitchH = limit(Brain.ThreeWirePort.F);

digital_out Expansion = digital_out(Brain.ThreeWirePort.A);
digital_out Expansion2 = digital_out(Brain.ThreeWirePort.B);
digital_out pneumaticsIndexer = digital_out(Brain.ThreeWirePort.H);
motor IntakeRoller = motor(PORT9, ratio18_1, true);
motor Flywheel1 = motor(PORT16, ratio18_1, false);
motor Flywheel2 = motor(PORT9, ratio18_1, false);
motor Indexer = motor(PORT10, ratio18_1, false);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}