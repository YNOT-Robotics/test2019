//in each auto file only include Robot.h
//if you include vex.h in more than 1 file big bad
#include "Robot.h"
#include "v5.h"
#include "v5_vcs.h"


//define auto1 here
void Robot::red1(){
      //pick up ball
    Yoink(0);
    Grab(0);
    move(36,100);
    intakeCon(2);
    vex::task Down = vex::task(down);
    Yoink(1);
    Yeet();
    move(-14,90);
    strafe(1,75);
    intakeCon(1);
    wait(100);
    intakeCon(0);

      //line up to cap
    turn(182,75);
    move(-29,100);

        //pickup cap
    Grab(1);
    vex::task::sleep(200);
    Yoink(0);

        //move to pole
    move(20,100);
    turn(-5,75);
    move(22.5,75);
    move(5,40);
    move(-22,75);
    strafe(-3,75);
    turn(-90,75);
    autonLift(1);
    move(-15,75);

        //place cap
    autonLift(2);
    Grab(0);

        //line up to cap
    move(14,75);
    turn(49,75);
    autonLift(0);
    Yoink(1);
    WristCheck();
    wait(1000);

        //pickup cap
    move(-27,90);
    Grab(1);
    vex::task::sleep(200);
    Yoink(0);

        //move to pole
    move(17,75);
    turn(-141,75);
    autonLift(500);
    toggleWrist();
    autonLift(1);
    move(-24.2,75);
    
        //place cap on pole
    autonLift(2);
    Grab(0);

        //shoot   
    move(23,75);
    toggleWrist();
    autonLift(0);
    turn(76,75);
    move(8,75);
    wait(6000);
    autoFire();


    /*
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
    */
}