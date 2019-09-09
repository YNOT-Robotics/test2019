//in each auto file only include Robot.h
//if you include vex.h in more than 1 file big bad
#include "Robot.h"
#include "v5.h"
#include "v5_vcs.h"

//define auto1 here
void Robot::auto1(){
  move(31,75);
  toggleWrist();
  autonLift(0);
  turn(60,75);
  intakeCon(3);
  move(12,75);
  wait(500);
  vex::task Down = vex::task(down);
  intakeCon(1);
  autoFire();
  wait(1000);
  intakeCon(0);
  move(-9,75);
  
  /*Yoink(0);
  Grab(0);
  vex::task Down = vex::task(down);
  move(12,75);
  intakeCon(1);
  Yoink(0);
  Yeet();
  turn(45,75);
  move(26,75);
  turn(45,75);
  move(-12,75);
  Yoink(1);
  turn(-90,40);
  strafe(-5,75);
  Yoink(0);
  turn(-173,75);
  move(33,75);
  move(-20,75);
  turn(180,75);
  turn(-20,75);
  move(48,75);
  move(-8,75);
    turn(89,75);
    autonLift(1);
    move(-17,75);

        //place cap
    autonLift(2);
    Grab(0);

        //line up to cap
    move(17,80);
    turn(-54.5,80);
    autonLift(0);
    Yoink(1);
    WristCheck();
    wait(800);

        //pickup cap
    move(-34,75);
    Grab(1);
    vex::task::sleep(200);
    Yoink(0);

        //move to pole
    move(13,80);
    turn(140,75);
    autonLift(500);
    toggleWrist();
    autonLift(1);
    move(-22.5,75);
    
        //place cap on pole
    autonLift(2);
    Grab(0);

        //shoot
    move(26,75);
    toggleWrist();
    autonLift(0);
    turn(-80.5,75);
    move(8.5,75);
    autoFire();
    */
}
    
  
  
  /*move(21,75);
  turn(50,75);
  move(-23.5,60);
  autoFire();
  */
