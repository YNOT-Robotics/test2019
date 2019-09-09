#include "Robot.h"
#include "v5.h"
#include "v5_vcs.h"

using namespace vex;
int choice;

// A global instance of vex::competition
vex::competition Competition;

// define your global instances of motors and other devices here


void pre_auton( void ) {
choice = autoSelect();

}


void autonomous( void ) {
  Robot Echo(true);
  Echo.autoInit();
  if(choice == 1){
    Echo.auto1();
  }
  if(choice == 2){
    Echo.red1();
  }
  if(choice == 3){
    Echo.red2();
  }
  if(choice == 4){
    Echo.blue1();
  }
  
  Echo.endAuto();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol( void ) {
  int number = 1;
	Robot Echo(false);
  //Echo.clearAll();
  Echo.start();
  vex::task drive = vex::task(driver);
  vex::task shoot = vex::task(fire);
  while(true){
	  Echo.intakeCon(number);
    Echo.liftCon();
    Echo.WristCon();
    Echo.toggleMode();
    Echo.CapDrop();
 	  vex::task::sleep(5);  
  }
  
}

//
// Main will set up the competition functions and callbacks.
//
int main() {
    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    
    //Run the pre-autonomous function. 
    pre_auton();
       
    //Prevent main from exiting with an infinite loop.                        
    while(1) {
      vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }    
       
}