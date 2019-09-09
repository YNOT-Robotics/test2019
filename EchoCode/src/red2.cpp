#include "Robot.h"
#include "v5.h"
#include "v5_vcs.h"


void Robot::red2(){
    Yoink(0);
    Grab(0);
    move(35,100);
    vex::task Down = vex::task(down);
    intakeCon(2);
    Yoink(1);
    Yeet();
    move(-14,90);
    intakeCon(1);

      //line up to cap
    turn(180,75);
    move(-28,100);
    intakeCon(0);

        //pickup cap
    Grab(1);
    vex::task::sleep(200);
    Yoink(0);

        //move to pole
    move(10,100);
    turn(-3,75);
    strafe(1.5,75);
    move(30,75);
    move(6,40);
    move(-23,75);
    strafe(-3,75);
    turn(-90,75);
    autonLift(1);
    move(-17,75);

        //place cap
    autonLift(2);
    Grab(0);

        //line up to cap
    move(12,75);
    turn(52,75);
    autonLift(0);
    Yoink(1);
    WristCheck();
    wait(800);

        //pickup cap
    move(-25,75);
    Grab(1);
    vex::task::sleep(200);
    Yoink(0);

        //move to pole
    move(10,75);
    turn(-137.5,75);
    autonLift(500);
    toggleWrist();
    autonLift(1);
    move(-30,75);
    
        //place cap on pole
    autonLift(2);
    Grab(0);

        //shoot
    move(40,75);
    toggleWrist();
    autonLift(0);
    turn(52,75);
    move(6,75);
    autoFire();
}