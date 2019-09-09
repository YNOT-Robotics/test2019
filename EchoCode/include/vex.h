#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>

#include "v5.h"
#include "v5_vcs.h"

//define motors in here
//only include this in the functions file
vex::brain BiiiigBrain;

vex::motor rightF1 = vex::motor(vex::PORT10,vex::gearSetting::ratio6_1,false);
vex::motor rightF2 = vex::motor(vex::PORT9,vex::gearSetting::ratio6_1,true);
vex::motor leftF1 = vex::motor(vex::PORT3,vex::gearSetting::ratio6_1,false);
vex::motor leftF2 = vex::motor(vex::PORT4,vex::gearSetting::ratio6_1,true);

vex::motor rightB1 = vex::motor(vex::PORT20,vex::gearSetting::ratio6_1,true);
vex::motor rightB2 = vex::motor(vex::PORT19,vex::gearSetting::ratio6_1,false);
vex::motor leftB1 = vex::motor(vex::PORT17,vex::gearSetting::ratio6_1,false);
vex::motor leftB2 = vex::motor(vex::PORT18,vex::gearSetting::ratio6_1,true);

vex::controller Dru = vex::controller();

vex::motor catapult = vex::motor(vex::PORT6,vex::gearSetting::ratio36_1,false);

vex::motor uptake = vex::motor(vex::PORT7,vex::gearSetting::ratio36_1,true);
vex::motor pusher = vex::motor(vex::PORT5,vex::gearSetting::ratio36_1,true);

vex::motor roller = vex::motor(vex::PORT8,vex::gearSetting::ratio18_1,true);
vex::motor wrist = vex::motor(vex::PORT11,vex::gearSetting::ratio18_1,false);

vex::motor liftL1 = vex::motor(vex::PORT2,vex::gearSetting::ratio18_1,true);
vex::motor liftL2 = vex::motor(vex::PORT16,vex::gearSetting::ratio18_1,false);
vex::motor liftR1 = vex::motor(vex::PORT1,vex::gearSetting::ratio18_1,false);
vex::motor liftR2 = vex::motor(vex::PORT15,vex::gearSetting::ratio18_1,true);


vex::bumper Cbutton(BiiiigBrain.ThreeWirePort.B);
vex::bumper CapIn(BiiiigBrain.ThreeWirePort.C);

vex::digital_out Yeeter = vex::digital_out(BiiiigBrain.ThreeWirePort.D);
vex::digital_out Yoinker = vex::digital_out(BiiiigBrain.ThreeWirePort.A);
vex::digital_out Grabber = vex::digital_out(BiiiigBrain.ThreeWirePort.E);