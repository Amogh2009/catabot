using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern controller joystick;
extern motor LeftFront;
extern motor RightFront;
extern motor LeftBack;
extern motor RightBack;
extern motor LeftMiddle;
extern motor RightMiddle;
extern motor catapult;
extern inertial Inertial;
extern digital_out Expansion;
extern digital_out Expansion2;
extern digital_out pneumaticsIndexer;
extern motor IntakeRoller;
extern motor Flywheel1;
extern motor Flywheel2;
extern motor Indexer;
extern limit LimitSwitchH;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );
void inertialTurnClockwise(double targetHeading);
void inertialTurnCounterClockwise(double targetHeading);
void calibrateInertialSensor(void);
void setStopping(vex::brakeType stoppingType);
void drivetrainStop(void);
void turnClockwise(double amount);
void turnCounterClockwise(double amount);
int drivePID ();
extern bool enableDrivePID;
extern int desiredValue;
extern int desiredTurnValue;