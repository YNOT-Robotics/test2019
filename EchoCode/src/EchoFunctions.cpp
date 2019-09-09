#include "vex.h"
#include "Robot.h"

bool r1Pressed = false;
bool yPressed = false;
bool xPressed = false;
bool APress = false;
int mode = 1;

//Tracking math
//
//
double angle0 = 0;
int ang0 = angle0;
double xm0 = 12;
double ym0 = 60;
double distx, disty, angEnd, ang, leng;
int r = 18;
int quad = 1; 
double xg0 = xm0-18;
double yg0 = ym0;
//
//
//
Robot::Robot(bool autonomous){
  auton = autonomous;
}

int driver(void){
  double power, turn, strafe;
  std::string errorCheck;
  while(true){
    power = 600*(Dru.Axis3.value()/127.0);
    turn = 600*(Dru.Axis1.value()/127.0);
    strafe = 600*(1.25*(Dru.Axis4.value()/127.0));
    if(fabs(power) < 7){
      power = 0;
    }
    if(fabs(turn) < 7){
      turn = 0;
    }
    if(fabs(strafe) < 20){
      strafe = 0;
    }
    rightF1.spin(vex::fwd, (power-strafe*.97)*mode-turn, vex::rpm);
    rightF2.spin(vex::fwd, (power-strafe*.97)*mode-turn, vex::rpm);
    rightB1.spin(vex::fwd, (power+strafe)*mode-turn, vex::rpm);
    rightB2.spin(vex::fwd, (power+strafe)*mode-turn, vex::rpm);
    leftF1.spin(vex::fwd, (power+strafe*.97)*mode+turn, vex::rpm);
    leftF2.spin(vex::fwd, (power+strafe*.97)*mode+turn, vex::rpm);
    leftB1.spin(vex::fwd, (power-strafe)*mode+turn, vex::rpm);
    leftB2.spin(vex::fwd, (power-strafe)*mode+turn, vex::rpm);

    vex::task::sleep(10);
  }
}

void Robot::start(){
  state = true;
  Grabber.set(0);
  Yeeter.set(1);
  catapult.setBrake(vex::coast);
  wrist.setBrake(vex::hold);
  rightF1.setBrake(vex::coast);
  rightF2.setBrake(vex::coast);
  rightB1.setBrake(vex::coast);
  rightB2.setBrake(vex::coast);
  leftF1.setBrake(vex::coast);
  leftF2.setBrake(vex::coast);
  leftB1.setBrake(vex::coast);
  leftB2.setBrake(vex::coast);
  Yeeter.set(0);
}


int fire(void){
  bool reload = true;
  while(true){
    if(!Cbutton.pressing() && reload){
			   catapult.spin(vex::fwd, 100, vex::rpm);
		} else {
		    if(Dru.ButtonL1.pressing()){
				      catapult.spin(vex::fwd, 100, vex::rpm);
              reload = true;
			  } else {
				      catapult.spin(vex::fwd, 0, vex::rpm);
              reload = false;
        }
		}
  }
}

void Robot::clearAll(){
  Yeeter.set(1);
  Grabber.set(0);
  liftL1.setBrake(vex::hold);
  liftL2.setBrake(vex::hold);
  liftR1.setBrake(vex::hold);
  liftR2.setBrake(vex::hold);
  pusher.setBrake(vex::hold);
  wrist.setBrake(vex::hold);
  bool going = true;
  vex::task::sleep(200);
  while(going){
    vex::task::sleep(20);
    going = false;
    if(liftL1.velocity(vex::rpm) > 1){
      going = true;
    } else {
      liftL1.spin(vex::fwd, 0, vex::rpm);
      liftL1.resetRotation();
      liftL2.spin(vex::fwd, 0, vex::rpm);
      liftL2.resetRotation();
    }
    if(liftR1.velocity(vex::rpm) > 1){
      going = true;
    } else {
      liftR1.spin(vex::fwd, 0, vex::rpm);
      liftR1.resetRotation();
      liftR2.spin(vex::fwd, 0, vex::rpm);
      liftR2.resetRotation();
    }
  }
  //pusher.spin(vex::fwd, 25);
  wrist.spin(vex::fwd, 20, vex::rpm);
    while(going){
      vex::task::sleep(20);
      going = false;
    if(pusher.velocity(vex::rpm) > 1){
      going = true;
    } else {
      pusher.spin(vex::fwd, 0, vex::rpm);
      pusher.resetRotation();
    }
    if(wrist.velocity(vex::rpm) > 3){
      going = true;
    } else {
      wrist.spin(vex::fwd, 0, vex::rpm);
      wrist.resetRotation();
    }
  }
  pusher.setBrake(vex::coast);
  vex::task::sleep(20);
}

void Robot::wait(int millis){
  vex::task::sleep(millis);
}

int down(void){
  while(!Cbutton.pressing()){
    catapult.spin(vex::fwd, 100, vex::rpm);
    vex::task::sleep(10);
  }
  catapult.spin(vex::fwd, 0, vex::rpm);
  return 1;
}

void Robot::toggleMode(){
  if(Dru.ButtonB.pressing() && !bPress){
    bPress = true;
    mode *= -1;
    go = !go;
    if(mode == 1 && go){
      Yoinker.set(0);
      roller.spin(vex::fwd, 400, vex::rpm);
      uptake.spin(vex::fwd, 100, vex::rpm);
    }else if(mode == -1 && !go){
      roller.spin(vex::fwd, 0, vex::rpm);
      uptake.spin(vex::fwd, 0, vex::rpm);
      if(liftState == 0 && !Cap){
        Yoinker.set(1);
      }
    }
  }else if(!Dru.ButtonB.pressing()){
    bPress = false;
  }
}

void Robot::toggleWrist(){
  int turn = 0;
  if(wrist.rotation(vex::deg) < 50){
    turn = 307;
  }
  wrist.rotateTo(turn, vex::deg, 200, vex::rpm);
  while(fabs(wrist.rotation(vex::deg)-turn)>5||fabs(wrist.rotation(vex::deg)-turn)<-5){
    wait(20);
  };
}
void Robot::CapDrop(){
 if(Dru.ButtonDown.pressing()){
    Grab(0);
    Yoink(1);
    vex::task::sleep(700);
  }
}

void Robot::WristCheck(){
  if(wrist.rotation(vex::deg) > 10){
    wrist.rotateTo(0, vex::deg, 200, vex::rpm);
    while(fabs(wrist.rotation(vex::deg)-0)>5||fabs(wrist.rotation(vex::deg)-0)<-5){
      vex::task::sleep(20);
    };
  }
}

void Robot::WristCon(){
  if(Dru.ButtonY.pressing() && !yPressed){
    yPressed = true;
    if(Yoinker.value() == 1){
      Yoinker.set(0);
      wait(200);
    }
    if(liftState == 0){
      if(Cap){  
        toggleWrist();
        Grabber.set(0);
        Yoinker.set(1);
      }
      else{
        Yoinker.set(0);
        wait(200);
        toggleWrist();
        wait(300);
        Yoinker.set(1);
      }
    }
    else if(liftState == 1){
      if(wrist.rotation(vex::deg) < 100){
        toggleWrist();
      }
      else{
        autonLift(2);
        Grabber.set(0);
      }
    }
    else if(liftState == 2 && Cap){
      Grab(1);
      autonLift(1);
      liftState = 1;
    }
  }else if(!Dru.ButtonY.pressing()){
    yPressed = false;
  }
  if(wrist.rotation(vex::deg)>5 && wrist.rotation(vex::deg)<154){
    wrist.rotateTo(0, vex::deg, 200, vex::rpm);
  }
  else if(wrist.rotation(vex::deg)>153 && wrist.rotation(vex::deg)<302){
    wrist.rotateTo(0, vex::deg, 200, vex::rpm);
  }
}

void Robot::liftCon(){
  if(mode == -1){
    if(!Broke){
      if(Dru.ButtonUp.pressing() && !Uppressed){
        Uppressed = true;
        Broke = !Broke; 
        liftState = 0;
      }else if(Dru.ButtonUp.pressing()){
        Uppressed = false;
      }
      if(CapIn.pressing()){
        Cap = true;
        if(liftState != 2){
          Grabber.set(1);
        }
      }else{
        Cap = false;
        Grabber.set(0);
      }
      if(liftState == 0 && Cap && !capSwitch){
        capSwitch = true;
        Grabber.set(1);
        Yoinker.set(0);
      }else if(liftState == 0 && !Cap && capSwitch){
        capSwitch = false;
        Yoinker.set(1);
        Grabber.set(0);
      }
      if(Dru.ButtonR1.pressing() && !r1Pressed){
        r1Pressed = true;
        if(liftState == 0){
          liftState = 1;
          Yoink(0);
          distance = 1700;
          liftR1.startRotateTo(distance, vex::deg, 200, vex::rpm);
          liftR2.startRotateTo(distance, vex::deg, 200, vex::rpm);
          liftL1.startRotateTo(distance, vex::deg, 200, vex::rpm);
          liftL2.startRotateTo(distance, vex::deg, 200, vex::rpm);   
        }else if(liftState == 1){
          liftState = 2;
          distance = 1400;
          liftR1.startRotateTo(distance, vex::deg, -200, vex::rpm);
          liftR2.startRotateTo(distance, vex::deg, -200, vex::rpm);
          liftL1.startRotateTo(distance, vex::deg, -200, vex::rpm);
          liftL2.startRotateTo(distance, vex::deg, -200, vex::rpm);
          Grabber.set(0);
        } else if(liftState >= 2){
          liftState = 0;
          if(wrist.rotation(vex::deg) > 100){
            wrist.startRotateTo(0,vex::deg, 200, vex::rpm);
          }
          Yoinker.set(1);
          distance = 0;
          liftR1.startRotateTo(distance, vex::deg, -200, vex::rpm);
          liftR2.startRotateTo(distance, vex::deg, -200, vex::rpm);
          liftL1.startRotateTo(distance, vex::deg, -200, vex::rpm);
          liftL2.startRotateTo(distance, vex::deg, -200, vex::rpm);
        }
      }else if(!Dru.ButtonR1.pressing()){
        r1Pressed = false;
      }
      if(Dru.ButtonA.pressing() && liftState == 0 && !APress){
        APress = true;
        liftState = 3;
        Yoinker.set(0);
        distance = 500;
        liftR1.startRotateTo(distance, vex::deg, 200, vex::rpm);
        liftR2.startRotateTo(distance, vex::deg, 200, vex::rpm);
        liftL1.startRotateTo(distance, vex::deg, 200, vex::rpm);
        liftL2.startRotateTo(distance, vex::deg, 200, vex::rpm);
      }else if(!Dru.ButtonA.pressing()){
        APress = false;
      }
      /*
      while(fabs(distance - rotation) > 10){
        if(Dru.ButtonDown.pressing()){
          Broke = true;
          break;
        }
        rotation = (liftL1.rotation(vex::deg) + liftR1.rotation(vex::deg))/2;
        vex::task::sleep(20);
      }*/
    }
    else{
      liftR1.startRotateTo(0, vex::deg, 150, vex::rpm);
      liftR2.startRotateTo(0, vex::deg, 150, vex::rpm);
      liftL1.startRotateTo(0, vex::deg, 150, vex::rpm);
      liftL2.startRotateTo(0, vex::deg, 150, vex::rpm);
      Broke = false; 
    }
  }
}

void Robot::intakeCon(int isZero){
  if(auton){
    if(isZero == 0){
      roller.spin(vex::fwd, 0, vex::rpm);
      uptake.spin(vex::fwd, 0, vex::rpm);
    } else if(isZero == 1){
      roller.spin(vex::fwd, 200, vex::rpm);
      uptake.spin(vex::fwd, 100, vex::rpm);
    } else if(isZero == -1){
      roller.spin(vex::fwd, -200, vex::rpm);
      uptake.spin(vex::fwd, -100, vex::rpm);
    } else if(isZero == 2){
      roller.spin(vex::fwd, 200, vex::rpm);
      uptake.spin(vex::fwd, 0, vex::rpm);
    } else if(isZero == 3){
      roller.spin(vex::fwd, 0, vex::rpm);
      uptake.spin(vex::fwd, 100, vex::rpm);
    }

  } else {
    if(Dru.ButtonL2.pressing() && go){
      roller.spin(vex::fwd, -200, vex::rpm);
      uptake.spin(vex::fwd, -100, vex::rpm);
    } else if(Dru.ButtonL1.pressing()){
      roller.spin(vex::fwd, 0, vex::rpm);
      uptake.spin(vex::fwd, 0, vex::rpm);
    }else if((Dru.ButtonX.pressing() && !xPressed)){
      xPressed = true;
      go = !go;
      if(!go){
        roller.spin(vex::fwd, 0, vex::rpm);
        uptake.spin(vex::fwd, 0, vex::rpm);
      }
    }else if(go){
      roller.spin(vex::fwd, 200, vex::rpm);
      uptake.spin(vex::fwd, 100, vex::rpm);
    }
    if(!Dru.ButtonX.pressing()){
      xPressed = false;
    }
  }
}

/*void Robot::togglePusher(){
  if (auton){
    if(pusher.rotation(vex::deg) < 100){
      pusher.rotateTo(200, vex::deg, -100, vex::rpm);
      pusher.setBrake(vex::hold);
    } else if(pusher.rotation(vex::deg) > 100){
      pusher.rotateTo(5, vex::deg, 100, vex::rpm);
      pusher.setBrake(vex::coast);
    }
  }
  else{
    if(Dru.ButtonA.pressing() && !aPressed){
      aPressed = true;
      if(pusher.rotation(vex::deg) < 100){
        pusher.rotateTo(120, vex::deg, -100, vex::rpm);
        pusher.setBrake(vex::hold);
      } else if(pusher.rotation(vex::deg) > 100){
          pusher.rotateTo(5, vex::deg, 100, vex::rpm);
          pusher.setBrake(vex::coast);
      }
    }else{
      aPressed = false;
    }
  }
}
*/


////Auto Functions

void TrackerMath(){
  quad = (ang0%360)/90;
  if(quad == 1){
    xg0 = xm0-r*cos(angle0);
    yg0 = ym0-r*sin(angle0);
  }
  else if(quad == 2){
    xg0 = xm0+r*cos(angle0);
    yg0 = ym0-r*sin(angle0);
  }
  else if(quad == 3){
    xg0 = xm0+r*cos(angle0);
    yg0 = ym0+r*sin(angle0);
  }
  else if(quad == 4){
    xg0 = xm0-r*cos(angle0);
    yg0 = ym0+r*sin(angle0);
  }
}


void Robot::reset(){
  rightF1.resetRotation();
  rightF2.resetRotation();
  rightB1.resetRotation();
  rightB2.resetRotation();
  leftF1.resetRotation();
  leftF2.resetRotation();
  leftB1.resetRotation();
  leftB2.resetRotation();
}
void Robot::autoInit(){
  rightF1.setBrake(vex::hold);
  rightF2.setBrake(vex::hold);
  rightB1.setBrake(vex::hold);
  rightB2.setBrake(vex::hold);
  leftF1.setBrake(vex::hold);
  leftF2.setBrake(vex::hold);
  leftB1.setBrake(vex::hold);
  leftB2.setBrake(vex::hold);
}
void Robot::endAuto(){
  rightF1.setBrake(vex::coast);
  rightF2.setBrake(vex::coast);
  rightB1.setBrake(vex::coast);
  rightB2.setBrake(vex::coast);
  leftF1.setBrake(vex::coast);
  leftF2.setBrake(vex::coast);
  leftB1.setBrake(vex::coast);
  leftB2.setBrake(vex::coast);
}

void Robot::turn(double angle, double speed){
  reset();
  int dir = angle / fabs(angle);
  double turn90 = 1680;
  double distance = (angle * turn90) / 90.0;
  double leftAvg = (leftF1.rotation(vex::deg) + leftB1.rotation(vex::deg)) / 2;
  double rightAvg = (rightF1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
  double targetLeft = -distance + leftAvg;
  double targetRight = distance + rightAvg;
  double toTargetLeft = -distance;
  double toTargetRight = distance;
  double leftSpeed = 0;
  double rightSpeed = 0;


  while(fabs(toTargetLeft) > 50 && fabs(toTargetRight) > 50){
    leftAvg = (leftF1.rotation(vex::deg) + leftB1.rotation(vex::deg)) / 2;
    rightAvg = (rightF1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
    toTargetLeft = targetLeft - leftAvg;
    toTargetRight = targetRight - rightAvg;

    if(fabs(toTargetLeft) < 150){
      leftSpeed = (fabs(toTargetLeft / 375) + .6) * -speed * dir;
    }else if(fabs(distance - toTargetLeft) < 150){
      leftSpeed = (.6 + fabs((distance - toTargetLeft)/375)) * -speed * dir;
    }else{
      leftSpeed = -speed * dir;
    }
    if(fabs(toTargetRight) < 150){
      rightSpeed = (fabs(toTargetRight / 375) + .6) * speed * dir;
    }else if(fabs(distance - toTargetRight) < 150){
      rightSpeed = (.6 + fabs((distance - toTargetRight)/375)) * speed * dir;
    }else{
      rightSpeed = speed * dir;
    }
    leftF1.spin(vex::fwd,6*leftSpeed, vex::rpm);
    leftF2.spin(vex::fwd,6*leftSpeed, vex::rpm);
    leftB1.spin(vex::fwd,6*leftSpeed, vex::rpm);
    leftB2.spin(vex::fwd,6*leftSpeed, vex::rpm);
    rightF1.spin(vex::fwd,6*rightSpeed, vex::rpm);
    rightF2.spin(vex::fwd,6*rightSpeed, vex::rpm);
    rightB1.spin(vex::fwd,6*rightSpeed, vex::rpm);
    rightB2.spin(vex::fwd,6*rightSpeed, vex::rpm);
    vex::task::sleep(2);
  }
  leftF1.spin(vex::fwd,0, vex::rpm);
  leftF2.spin(vex::fwd,0, vex::rpm);
  leftB1.spin(vex::fwd,0, vex::rpm);
  leftB2.spin(vex::fwd,0, vex::rpm);
  rightF1.spin(vex::fwd,0, vex::rpm);
  rightF2.spin(vex::fwd,0, vex::rpm);
  rightB1.spin(vex::fwd,0, vex::rpm);
  rightB2.spin(vex::fwd,0, vex::rpm);
  vex::task::sleep(200);

  double newx = r*cos(angle);
  double newy = r*sin(angle);
  xg0 = xm0 + newx;
  yg0 = ym0 + newy;
  angle0 += angle;
}
void Robot::testMove(double inch, double per){
  reset();
  double distance = inch * 68;
  double maxSpeed = per*6;
  double leftAvg = (leftF1.rotation(vex::deg) + leftB1.rotation(vex::deg)) / 2;
  double rightAvg = (rightF1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
  double targetLeft = distance + leftAvg;
  double targetRight = distance + rightAvg;
  double toTargetLeft = distance;
  double toTargetRight = distance;
  double toTargetAvg = distance;
  double leftSpeed = 0;
  double rightSpeed = 0;
  bool accel = true;
  bool deccel = false;
  double accelDis;
  double startSpeed = maxSpeed*.1;
  double speed = startSpeed;
  double rampSpeed = 12;
  double decMult = 2.4*per*.01;
  int dir = distance/fabs(distance);
  reset();
  printf("\n\nNext Run\n\n");

  while(fabs(toTargetLeft) > 30 && fabs(toTargetRight) > 30){
    leftAvg = (leftF1.rotation(vex::deg) + leftB1.rotation(vex::deg)) / 2;
    rightAvg = (rightF1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
    toTargetLeft = targetLeft - leftAvg;
    toTargetRight = targetRight - rightAvg;
    toTargetAvg = (toTargetLeft + toTargetRight) / 2;
    
    if(fabs(inch) < 24 && fabs(speed) > 60){
      rampSpeed = 12*fabs(inch/24);
    }

    /////////////Do Not Change this////////////
    if(accel){
      printf("sp:%f ",speed);
      if(fabs(speed)<fabs(maxSpeed)){
        if(dir == 1){
          speed += rampSpeed;
        }
        else{
          speed -= rampSpeed;
        }
      }
      else{
        accel = false;
        accelDis = distance - toTargetAvg;
        printf("%f",accelDis);
        printf(" = accell dis\n\n");
      }
      printf("a:%f b:%f",toTargetAvg,(distance/2));
      if(fabs(toTargetAvg)<fabs(distance/2)+30){
        printf("accell half\n\n");
        deccel = true;
        accel = false;
        accelDis = distance-toTargetAvg;
        decMult = 3*per*.01;
      };
    }
    else if(deccel){
      printf("dec:%f",speed);
      if(fabs(speed) > fabs(.1*maxSpeed)){
        if(dir == 1){
          speed -= rampSpeed*decMult;
        }
        else{
          speed += rampSpeed*decMult;
        }
      }
    }
    else{
      printf("Yeet");
      if(fabs(toTargetAvg)>fabs(accelDis)){
        speed = maxSpeed*dir;
      }
      else{
        deccel = true;
      }
    }
    
    if(fabs(leftAvg) > fabs(rightAvg)+30){
      double difference = (leftSpeed-rightSpeed)/rightSpeed;
      leftSpeed = rightSpeed * (1-difference);
    }
    else if(fabs(rightAvg) > fabs(leftAvg)+30){
      double difference = (rightSpeed-leftSpeed)/leftSpeed;
      rightSpeed = leftSpeed * (1-difference);
    }
    else{
      leftSpeed = speed;
      rightSpeed = speed;
    }

      printf("sp2:%f r:%f l:%f tar:%f \n",speed,rightSpeed,leftSpeed,toTargetAvg);
    
    leftF1.spin(vex::fwd, leftSpeed, vex::rpm);
    leftF2.spin(vex::fwd, leftSpeed, vex::rpm);
    leftB1.spin(vex::fwd, leftSpeed, vex::rpm);
    leftB2.spin(vex::fwd, leftSpeed, vex::rpm);
    rightF1.spin(vex::fwd, rightSpeed, vex::rpm);
    rightF2.spin(vex::fwd, rightSpeed, vex::rpm);
    rightB1.spin(vex::fwd, rightSpeed, vex::rpm);
    rightB2.spin(vex::fwd, rightSpeed, vex::rpm);
    vex::task::sleep(20);
  }
  leftF1.spin(vex::fwd, 0, vex::rpm);
  leftF2.spin(vex::fwd, 0, vex::rpm);
  leftB1.spin(vex::fwd, 0, vex::rpm);
  leftB2.spin(vex::fwd, 0, vex::rpm);
  rightF1.spin(vex::fwd, 0, vex::rpm);
  rightF2.spin(vex::fwd, 0, vex::rpm);
  rightB1.spin(vex::fwd, 0, vex::rpm);
  rightB2.spin(vex::fwd, 0, vex::rpm);
  vex::task::sleep(150);
}

void Robot::move(double inch, double per){
  double distance = inch * 68;
  double speed = per*6;
  double leftAvg = (leftF1.rotation(vex::deg) + leftB1.rotation(vex::deg)) / 2;
  double rightAvg = (rightF1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
  double targetLeft = distance + leftAvg;
  double targetRight = distance + rightAvg;
  double toTargetLeft = distance;
  double toTargetRight = distance;
  double toTargetAvg = distance;
  double agg = 5;
  double leftSpeed = 0;
  double rightSpeed = 0;
  double adjDistance = fabs(distance / 2);
  int dir = distance / fabs(distance);


  while(fabs(toTargetLeft) > 30 && fabs(toTargetRight) > 30){
    leftAvg = (leftF1.rotation(vex::deg) + leftB1.rotation(vex::deg)) / 2;
    rightAvg = (rightF1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
    toTargetLeft = targetLeft - leftAvg;
    toTargetRight = targetRight - rightAvg;
    toTargetAvg = (toTargetLeft + toTargetRight) / 2;

    /////////////Do Not Change this////////////
    if(fabs(distance) >= 2800){
      agg = 2000; //2000
      if(fabs(toTargetAvg) < 1600){
        leftSpeed = speed * ((toTargetAvg / 1800) + (dir * .06) + ((toTargetLeft - toTargetRight) / agg));
        rightSpeed = speed * ((toTargetAvg / 1800) + (dir * .06) - ((toTargetLeft - toTargetRight) / agg));
      }else if(fabs(distance - toTargetAvg) < 1200){
        leftSpeed = speed * (((distance - toTargetAvg) / 2000) + (dir * .35) + ((toTargetLeft - toTargetRight) / agg));
        rightSpeed = speed * (((distance - toTargetAvg) / 2000) + (dir * .35) - ((toTargetLeft - toTargetRight) / agg));
      }else{
        leftSpeed = speed * ((dir * .95) + ((toTargetLeft - toTargetRight) / agg));
        rightSpeed = speed * ((dir * .95) - ((toTargetLeft - toTargetRight) / agg));
      }
    }else{
      agg = 1600;
      if(fabs(toTargetAvg) < adjDistance){
        leftSpeed = speed * ((toTargetAvg / 1800) + (dir * .25) + ((toTargetLeft - toTargetRight) / agg));
        rightSpeed = speed * ((toTargetAvg / 1800) + (dir * .25) - ((toTargetLeft - toTargetRight) / agg));
      }else if(fabs(distance - toTargetAvg) < adjDistance){
        leftSpeed = speed * (((distance - toTargetAvg) / 2000) + (dir * .35) + ((toTargetLeft - toTargetRight) / agg));
        rightSpeed = speed * (((distance - toTargetAvg) / 2000) + (dir * .35) - ((toTargetLeft - toTargetRight) / agg));
      }else{
        leftSpeed = speed * ((dir * .95) + ((toTargetLeft - toTargetRight) / agg));
        rightSpeed = speed * ((dir * .95) - ((toTargetLeft - toTargetRight) / agg));
      }
    }
    leftF1.spin(vex::fwd, leftSpeed, vex::rpm);
    leftF2.spin(vex::fwd, leftSpeed, vex::rpm);
    leftB1.spin(vex::fwd, leftSpeed, vex::rpm);
    leftB2.spin(vex::fwd, leftSpeed, vex::rpm);
    rightF1.spin(vex::fwd, rightSpeed, vex::rpm);
    rightF2.spin(vex::fwd, rightSpeed, vex::rpm);
    rightB1.spin(vex::fwd, rightSpeed, vex::rpm);
    rightB2.spin(vex::fwd, rightSpeed, vex::rpm);
    vex::task::sleep(2);
  }
  leftF1.spin(vex::fwd, 0, vex::rpm);
  leftF2.spin(vex::fwd, 0, vex::rpm);
  leftB1.spin(vex::fwd, 0, vex::rpm);
  leftB2.spin(vex::fwd, 0, vex::rpm);
  rightF1.spin(vex::fwd, 0, vex::rpm);
  rightF2.spin(vex::fwd, 0, vex::rpm);
  rightB1.spin(vex::fwd, 0, vex::rpm);
  rightB2.spin(vex::fwd, 0, vex::rpm);
  vex::task::sleep(150);

  xm0 += inch*cos(angle0);
  ym0 += inch*sin(angle0);
}

void Robot::strafe(double inch, double per){
  reset();
  double distance = inch * 83;
  double speed = per*6;
  double backAvg = (-leftB1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
  double frontAvg = (-rightF1.rotation(vex::deg) + leftF1.rotation(vex::deg)) / 2;
  double targetBack = distance;
  double targetFront = distance;
  double totargetBack = distance;
  double totargetFront = distance;
  double toTargetAvg = distance;
  double agg = 5;
  double frontSpeed = .9*speed;
  double backSpeed = speed;
  double adjDistance = fabs(distance / 2);
  int dir = distance / fabs(distance);

  while(fabs(totargetBack) > 30 && fabs(totargetFront) > 30){
    
    backAvg = (-leftB1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
    frontAvg = (-rightF1.rotation(vex::deg) + leftF1.rotation(vex::deg)) / 2;
    totargetBack = targetBack - backAvg;
    totargetFront = targetFront - frontAvg;
    toTargetAvg = (totargetBack + totargetFront) / 2;

    /////////////Do Not Change this////////////
    if(fabs(distance) >= 2800){
      agg = 2000;
      if(fabs(toTargetAvg) < 1600){
        frontSpeed = speed * ((toTargetAvg / 1800) + (dir * .06) - ((totargetBack - totargetFront) / agg));
        backSpeed = speed * ((toTargetAvg / 1800) + (dir * .06) + ((totargetBack - totargetFront) / agg));
      }else if(fabs(distance - toTargetAvg) < 1200){
        frontSpeed = speed * (((distance - toTargetAvg) / 2000) + (dir * .35) - ((totargetBack - totargetFront) / agg));
        backSpeed = speed * (((distance - toTargetAvg) / 2000) + (dir * .35) + ((totargetBack - totargetFront) / agg));
      }else{
        frontSpeed = speed * ((dir * .95) - ((totargetBack - totargetFront) / agg));
        backSpeed = speed * ((dir * .95) + ((totargetBack - totargetFront) / agg));
      }
    }else{
      agg = 1000;
      if(fabs(toTargetAvg) < adjDistance){
        frontSpeed = speed * ((toTargetAvg / 1800) + (dir * .25) - ((totargetBack - totargetFront) / agg));
        backSpeed = speed * ((toTargetAvg / 1800) + (dir * .25) + ((totargetBack - totargetFront) / agg));
      }else if(fabs(distance - toTargetAvg) < adjDistance){
        frontSpeed = speed * (((distance - toTargetAvg) / 2000) + (dir * .35) - ((totargetBack - totargetFront) / agg));
        backSpeed = speed * (((distance - toTargetAvg) / 2000) + (dir * .35) + ((totargetBack - totargetFront) / agg));
      }else{
        frontSpeed = speed * ((dir * .95) - ((totargetBack - totargetFront) / agg));
        backSpeed = speed * ((dir * .95) + ((totargetBack - totargetFront) / agg));
      }
    }
    
    leftF1.spin(vex::fwd, frontSpeed*.9, vex::rpm);
    leftF2.spin(vex::fwd, frontSpeed*.9, vex::rpm);
    leftB1.spin(vex::fwd, -backSpeed, vex::rpm);
    leftB2.spin(vex::fwd, -backSpeed, vex::rpm);
    rightF1.spin(vex::fwd, -frontSpeed*.9, vex::rpm);
    rightF2.spin(vex::fwd, -frontSpeed*.9, vex::rpm);
    rightB1.spin(vex::fwd, backSpeed, vex::rpm);
    rightB2.spin(vex::fwd, backSpeed, vex::rpm);
    vex::task::sleep(2);
    
  }

  leftF1.spin(vex::fwd, 0, vex::rpm);
  leftF2.spin(vex::fwd, 0, vex::rpm);
  leftB1.spin(vex::fwd, 0, vex::rpm);
  leftB2.spin(vex::fwd, 0, vex::rpm);
  rightF1.spin(vex::fwd, 0, vex::rpm);
  rightF2.spin(vex::fwd, 0, vex::rpm);
  rightB1.spin(vex::fwd, 0, vex::rpm);
  rightB2.spin(vex::fwd, 0, vex::rpm);
  vex::task::sleep(100);
}
void Robot::autoFire(){
    while(!Cbutton.pressing()){
      catapult.spin(vex::fwd, 100, vex::rpm);
      vex::task::sleep(10);
    }
    while(Cbutton.pressing()){
      catapult.spin(vex::fwd, 100, vex::rpm);
      vex::task::sleep(10);
    }
    vex::task::sleep(300);
    while(!Cbutton.pressing()){
      catapult.spin(vex::fwd, 100, vex::rpm);
      vex::task::sleep(10);
    }
    catapult.spin(vex::fwd, 0, vex::rpm);
}
void Robot::autonLift(int state){
  int distance = 0;
  double rotation = 0;
  if(state == 0){
    distance = 0;
  } else if(state == 1){
    distance = 1700;
  } else if(state == 2){
    distance = 1400;
  }
  else if(state > 10){
    distance = state;
  }
  liftR1.startRotateTo(distance, vex::deg, 200, vex::rpm);
  liftR2.startRotateTo(distance, vex::deg, 200, vex::rpm);
  liftL1.startRotateTo(distance, vex::deg, 200, vex::rpm);
  liftL2.startRotateTo(distance, vex::deg, 200, vex::rpm);
  while(fabs(distance - rotation) > 50){
    rotation = (liftL1.rotation(vex::deg) + liftR1.rotation(vex::deg))/2;
    vex::task::sleep(20);
  }
  vex::task::sleep(200);
}



void Robot::Yeet(){
  Yeeter.set(1);
  vex::task::sleep(250);
  Yeeter.set(0);
}

void Robot::Grab(bool Status){
  Grabber.set(Status);
}

void Robot::Yoink(bool Status){
  Yoinker.set(Status);
}

void resetScreen(){
    BiiiigBrain.Screen.clearScreen();
    BiiiigBrain.Screen.setCursor(1,1);
}
void nl(){
    BiiiigBrain.Screen.newLine();
}
int autoSelect(){
  int chose;
  resetScreen();
  for(int x = 0; x<4; x++){
    BiiiigBrain.Screen.drawRectangle((x*120)+1,(0*120)+1,118,118,vex::color::red);
  }
  for(int x = 0; x<3; x++){
    BiiiigBrain.Screen.drawRectangle((x*120)+1,(1*120)+1,118,118,vex::color::blue);
  }
  BiiiigBrain.Screen.setPenColor(vex::color::cyan);
  nl();
  nl();
  nl();
  BiiiigBrain.Screen.print("    Test    ");
  BiiiigBrain.Screen.print("   Red 1    ");
  BiiiigBrain.Screen.print("   Red 2    ");
  BiiiigBrain.Screen.print("   Blue 1   ");
  nl();
  nl();
  nl();
  nl();
  nl();
  BiiiigBrain.Screen.print("   Blue 2   ");
  BiiiigBrain.Screen.print("            ");
  BiiiigBrain.Screen.print("            ");
  BiiiigBrain.Screen.print("            ");
  int xSelect,ySelect;
  while(!BiiiigBrain.Screen.pressing()){
    Dru.rumble("..-..-");
  }
  xSelect = BiiiigBrain.Screen.xPosition();
  ySelect = BiiiigBrain.Screen.yPosition();
  while(BiiiigBrain.Screen.pressing()){}
  if(ySelect < 120){
    if(xSelect < 120){
      chose = 1;
    }else if(xSelect < 240){
      chose =  2;
    }else if(xSelect < 360){
      chose = 3;
    }else{
      chose = 4;
    }
  }else{
    if(xSelect < 120){
      chose = 5;
    }else if(xSelect < 240){
      chose = 6;
    }else if(xSelect < 360){
      chose = 7;
    }else{
      chose = 8;
    }
  }
  resetScreen();
  BiiiigBrain.Screen.print("Selected Option %d",chose);
  return chose;
}