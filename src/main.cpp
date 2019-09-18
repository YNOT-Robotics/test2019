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
  return (leftF1.rotation(vex::deg) + leftB.rotation(vex::deg)) * 3.14159 * 2;
}

double getCurrentDistanceRight() {
  return (rightF1.rotation(vex::deg) + rightB.rotation(vex::deg)) * 3.14159 * 2;
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

// Drive straight specified distance in inches
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
  driveDistance(20.0);
}

int main() {

  Competition.autonomous( autonomous );
  Competition.drivercontrol( usercontrol );

    while(true) {
        vex::task::sleep(100);
    }
}
