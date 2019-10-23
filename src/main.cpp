#include "vex.h"

using namespace vex;

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
  double motorVelocity = 0;
  vex::timer timer;
  timer.clear();

  // Calculate target average distance for left and right sides of drivetrain
  double targetLocationLeft = getCurrentDistanceLeft() + distance;
  double targetLocationRight = getCurrentDistanceRight() + distance;
  double targetLocationAvg = (targetLocationLeft + targetLocationRight) / 2;

  double distanceToTargetAvg = targetLocationAvg - getCurrentDistanceAvg();

  double currentVelocityAvgAccumulator = 0.0;
  int counter = 0;
  double trueCurrentVelocityAvg = 0.0;

  double error = 0.0;

  while(distanceToTargetAvg > 1.0 || distanceToTargetAvg < -1.0) {

    // Accumulator to measure velocity over 50ms period
    currentVelocityAvgAccumulator += getCurrentVelocityAvg();
    counter++;

    if (timer.time() % 50 == 0) {

      if(motorVelocity <= coastVelocity) {
        motorVelocity += acceleration * timer.time() * 0.0000166667;
      }

      timer.clear();

      distanceToTargetAvg = targetLocationAvg - getCurrentDistanceAvg();

      trueCurrentVelocityAvg = currentVelocityAvgAccumulator / counter;

      Brain.Screen.printAt(0, 35, "Calculated Velocity=%lf\n", motorVelocity);
      Brain.Screen.printAt(0, 60, "Measured Velocity=%lf\n", trueCurrentVelocityAvg);

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

      // Reset accumulator to find measured velocity over next 50ms
      currentVelocityAvgAccumulator = 0;
      counter = 0;

      // Calculate percent error of actual current velocity vs assigned motorVelocity
      error = (motorVelocity - trueCurrentVelocityAvg) / trueCurrentVelocityAvg;

      //motorVelocity *= 1.0 + error;
    }
  }

  // Stop motors
  leftF1.stop();
  leftF2.stop();
  leftB.stop();
  rightF1.stop();
  rightF2.stop();
  rightB.stop();
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

int main() {
  //acclerateConstant();
  driveDistanceAcceleration(25.0, 600.0, 75.0);
  //vex::task D = vex::task(driver);

  while(true) {
    vex::task::sleep(10);
  }
}
