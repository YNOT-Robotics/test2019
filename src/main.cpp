#include "vex.h"

using namespace vex;

vex::competition Competition;

// P (proportion) controller to calculate output based on error
double pController(double currentValue, double targetValue, double p) {
  double error = targetValue - currentValue;
  return error * p;
}

//********** AUTO DRIVE CODE **********

// Get current distances
double getCurrentDistanceLeft() {
  return ((leftF1.rotation(vex::deg) + leftB.rotation(vex::deg)) / 2) * (3.14159 / 180) * 2;
}

double getCurrentDistanceRight() {
  return ((rightF1.rotation(vex::deg) + rightB.rotation(vex::deg)) / 2) * (3.14159 / 180) * 2;
}

double getCurrentDistanceAvg() {
  return (getCurrentDistanceLeft() + getCurrentDistanceRight()) / 2;
}

// Get current velocities
double getCurrentVelocityLeft() {
  return (leftF1.velocity(vex::velocityUnits::rpm) + leftB.velocity(vex::velocityUnits::rpm)) / 2;
}

double getCurrentVelocityRight() {
  return (rightF1.velocity(vex::velocityUnits::rpm) + rightB.velocity(vex::velocityUnits::rpm)) / 2;
}

double getCurrentVelocityAvg() {
  return (getCurrentVelocityRight() + getCurrentVelocityLeft()) / 2;
}

// Checks to see if robot should continue to accelerate based on how far robot is projected to move
bool continueAccelerationPhase(double currentDistance, double targetDistance, double acceleration) {
  
  // Use kinematic equation to test distance robot will travel
  double currentVelocity = getCurrentVelocityAvg();
  double projectedDistance = ((currentVelocity * currentVelocity) / (2 * acceleration)) * 4 * 3.14159;

  // Return whether or not the robot will travel the distance to the target end position upon deceleration to stop
  return !(projectedDistance >= (targetDistance - currentDistance));
}

// Drive straight specified distance in inches; using p-controller
void driveDistance(double distance) {
  double targetDistanceLeft = getCurrentDistanceLeft() + distance;
  double targetDistanceRight = getCurrentDistanceRight() + distance;
  double targetDistanceAvg = (targetDistanceLeft + targetDistanceRight) / 2;

  double distanceToTargetAvg = targetDistanceAvg - getCurrentDistanceAvg();

  while((distanceToTargetAvg > 1.0 && distanceToTargetAvg < -1.0) || getCurrentVelocityAvg() > 100) {
    double outputLeft = pController(getCurrentDistanceLeft(), targetDistanceLeft, 0.1);
    double outputRight = pController(getCurrentDistanceRight(), targetDistanceRight, 0.1);

    if(outputLeft > 1.0) {
      outputLeft = 1.0;
    } else if(outputLeft < -1.0) {
      outputLeft = -1.0;
    }

    if(outputRight > 1.0) {
      outputRight = 1.0;
    } else if(outputRight < -1.0) {
      outputRight = -1.0;
    }

    leftF1.spin(vex::fwd,outputLeft, vex::pct);
    leftF2.spin(vex::fwd,outputLeft, vex::pct);
    leftB.spin(vex::fwd,outputLeft, vex::pct);
    rightF1.spin(vex::fwd,outputRight, vex::pct);
    rightF2.spin(vex::fwd,outputRight, vex::pct);
    rightB.spin(vex::fwd,outputRight, vex::pct);
  }

  distanceToTargetAvg = targetDistanceAvg - getCurrentDistanceAvg();
}

// Drive inputted distance with inputted acceleration with inputted coast velocity
// distance is in inches; acceleration is in RPM per minute; coast velocity is in RPM
void driveDistanceAcceleration(double distance, double acceleration, double coastVelocity) {

  vex::timer timer;
  timer.clear();

  int accelerationStage = 1;
  double motorVelocity = 0;
  double changeInMotorVelocity = 0;

  // Calculate target average target distance for left and right sides of drivetrain
  double targetLocationLeft = getCurrentDistanceLeft() + distance;
  double targetLocationRight = getCurrentDistanceRight() + distance;
  double targetLocationAvg = (targetLocationLeft + targetLocationRight) / 2;

  double distanceToTargetAvg = targetLocationAvg - getCurrentDistanceAvg();

  while(distanceToTargetAvg > 5.0 && distanceToTargetAvg < -5.0) {

    // Find change in motor velocity based on specified acceleration
    changeInMotorVelocity = acceleration * timer.time(vex::msec) * (0.00001667);

    if(accelerationStage == 1) {

      // Robot is accelerating; continue accelerating w/o passing goal
      if(continueAccelerationPhase(getCurrentDistanceAvg(), targetLocationAvg, acceleration)) {
        
        // Accelerate
        motorVelocity += changeInMotorVelocity;
        
        // Coast velocity has been reached; begin constant coast velocity stage
        if(motorVelocity >= coastVelocity) {
          motorVelocity = coastVelocity;
          accelerationStage = 2;
        }
      } else {
        // Robot will pass goal; begin deceleration phase
        accelerationStage = 3;
      }
    } else if(accelerationStage == 2) {

      // Robot is at coast velocity; see when it should start decelerating
      if(continueAccelerationPhase(getCurrentDistanceAvg(), targetLocationAvg, acceleration)) {
        accelerationStage = 3;
      }
    } else if(accelerationStage == 3) {
      // Robot is decelerating; continue decelerating until stopped
      if(motorVelocity >= 0) {
        motorVelocity -= changeInMotorVelocity;
      }
    }

    // Set velocities
    leftF1.setVelocity(motorVelocity, vex::rpm);
    leftF2.setVelocity(motorVelocity, vex::rpm);
    leftB.setVelocity(motorVelocity, vex::rpm);
    rightF1.setVelocity(motorVelocity, vex::rpm);
    rightF2.setVelocity(motorVelocity, vex::rpm);
    rightB.setVelocity(motorVelocity, vex::rpm);

    // Spin motors
    leftF1.spin(vex::fwd);
    leftF2.spin(vex::fwd);
    leftB.spin(vex::fwd);
    rightF1.spin(vex::fwd);
    rightF2.spin(vex::fwd);
    rightB.spin(vex::fwd);

    timer.clear();
  }
}

int driver(void){
  double power, turn;

  while(true){
    power = 200*(Grant.Axis3.value()/127.0);
    turn = 200*((Grant.Axis1.value()/127.0));

    rightF1.spin(vex::fwd, power-turn, vex::rpm);
    rightF2.spin(vex::fwd,power-turn, vex::rpm);
    rightB.spin(vex::fwd,power-turn, vex::rpm);
    leftB.spin(vex::fwd,power+turn, vex::rpm);
    leftF1.spin(vex::fwd,power+turn, vex::rpm);
    leftF2.spin(vex::fwd,power+turn, vex::rpm);


    vex::task::sleep(10);
  }
}

void usercontrol() {
  vex::task D = vex::task(driver);
}

void autonomous() {
  driveDistanceAcceleration(20.0, 10.0, 150.0);
}

int main() {

  Competition.autonomous( autonomous );
  Competition.drivercontrol( usercontrol );

    while(true) {
        vex::task::sleep(100);
    }
}
