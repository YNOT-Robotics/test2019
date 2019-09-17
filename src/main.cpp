/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       BRYSON_GULLETT                                            */
/*    Created:      Tue Sep 17 2019                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#include "vex.h"

using namespace vex;

vex::competition Competition;

// P (proportion) controller to calculate output based on error
double pController(double currentValue, double targetValue, double p) {
  double error = targetValue - currentValue;
  return error * p;
}

// Drive straight specified distance in inches
void driveDistance(double distance) {
  double currentDistanceLeft = (leftF1.rotation(vex::deg) + leftB.rotation(vex::deg)) * 3.14159 * 2;
  double currentDistanceRight = (rightF1.rotation(vex::deg) + rightB.rotation(vex::deg)) * 3.14159 * 2;
  double currentDistanceAvg = (currentDistanceLeft + currentDistanceRight) / 2;

  double targetDistanceLeft = currentDistanceLeft + distance;
  double targetDistanceRight = currentDistanceRight + distance;
  double targetDistanceAvg = (targetDistanceLeft + targetDistanceRight) / 2;

  double currentVelocityLeft = (leftF1.velocity(vex::velocityUnits::rpm) + leftB.velocity(vex::velocityUnits::rpm)) / 2;
  double currentVelocityRight = (rightF1.velocity(vex::velocityUnits::rpm) + rightB.velocity(vex::velocityUnits::rpm)) / 2;
  double currentVelocityAvg = (currentVelocityRight + currentVelocityLeft) / 2;

  double distanceToTargetAvg = targetDistanceAvg - currentDistanceAvg;

  while((distanceToTargetAvg > 1.0 && distanceToTargetAvg < -1.0) || currentVelocityAvg > 100) {
    
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
  driveDistance(20.0);
}

int main() {

  Competition.autonomous( autonomous );
  Competition.drivercontrol( usercontrol );

    while(true) {
        vex::task::sleep(100);
    }
}
