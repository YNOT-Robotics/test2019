#include "Robot.h"
#include "v5.h"
#include "v5_vcs.h"

void Robot::blue1(){
      //pickup ball
    Yoink(0);
    Grab(0);
    move(33,100);
    vex::task Down = vex::task(down);
    intakeCon(2);
    Yoink(1);
    Yeet();
    move(-14,100);
    intakeCon(1);

        //line up to cap
    turn(-177,75);
    move(-28,100);
    intakeCon(0);

        //pickup cap
    Grab(1);
    vex::task::sleep(200);
    Yoink(0);

        //move to pole
    move(15,75);
    turn(4,75);
    move(29,80);
    move(-21.5,75);
    strafe(3,75);
    turn(89.5,75);
    autonLift(1);
    move(-15,75);

        //place cap
    autonLift(2);
    Grab(0);

        //line up to cap
    move(12,80);
    turn(-51,80);
    autonLift(0);
    Yoink(1);
    WristCheck();
    wait(800);

        //pickup cap
    move(-22,75);
    Grab(1);
    vex::task::sleep(200);
    Yoink(0);

        //move to pole
    move(10.5,80);
    turn(135,75);
    autonLift(500);
    toggleWrist();
    autonLift(1);
    move(-27,75);
    
        //place cap on pole
    autonLift(2);
    Grab(0);

        //shoot
    move(31,75);
    toggleWrist();
    autonLift(0);
    turn(-60,75);
    intakeCon(3);
    move(12,75);
    wait(500);
    autoFire();
    move(-9,75);
}