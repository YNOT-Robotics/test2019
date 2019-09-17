/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Vex Robotics                                              */
/*    Created:      1 Feb 2019                                                */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "v5.h"
#include "v5_vcs.h"

vex::brain Brain;

vex::controller Grant = vex::controller();

vex::motor rightF1 = vex::motor(vex::PORT11,false);
vex::motor rightF2 = vex::motor(vex::PORT18,false);
vex::motor rightB = vex::motor(vex::PORT2,true);
vex::motor leftF1 = vex::motor(vex::PORT12,vex::gearSetting::ratio18_1,true);
vex::motor leftF2 = vex::motor(vex::PORT16,vex::gearSetting::ratio18_1,true);
vex::motor leftB = vex::motor(vex::PORT3,vex::gearSetting::ratio18_1,false);