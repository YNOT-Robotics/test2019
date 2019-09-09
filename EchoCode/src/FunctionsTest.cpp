/*#include "vex.h"
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

double inch = 63;
double RadGyr = (1700/63)*(2/M_PI);
double turn90 = RadGyr*(M_PI/2)*inch;
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
        toggleWrist();
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
      
      while(fabs(distance - rotation) > 10){
        if(Dru.ButtonDown.pressing()){
          Broke = true;
          break;
        }
        rotation = (liftL1.rotation(vex::deg) + liftR1.rotation(vex::deg))/2;
        vex::task::sleep(20);
      }
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
      roller.spin(vex::fwd, 400, vex::rpm);
      uptake.spin(vex::fwd, 100, vex::rpm);
    } else if(isZero == -1){
      roller.spin(vex::fwd, -300, vex::rpm);
      uptake.spin(vex::fwd, -100, vex::rpm);
    } else if(isZero == 2){
      roller.spin(vex::fwd, 400, vex::rpm);
      uptake.spin(vex::fwd, 0, vex::rpm);

    }

  } else {
    if(Dru.ButtonL2.pressing() && go){
      roller.spin(vex::fwd, -300, vex::rpm);
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
      roller.spin(vex::fwd, 400, vex::rpm);
      uptake.spin(vex::fwd, 100, vex::rpm);
    }
    if(!Dru.ButtonDown.pressing()){
      xPressed = false;
    }
  }
}


void Robot::togglePusher(){
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
  int dir = angle / fabs(angle);
  double turn90 = 1700;
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
  //angle correction
  if(angle0 > 359){
    angle0 -= 360;
  }
  if(angle0 < 0){
    angle0 += 360;
  }
}

void Robot::move(double inch, double per){
  double distance = inch * 63;
  double maxSpeed = per*6;
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
  bool accel = true;
  bool deccel = false;
  double accelDis;
  double startSpeed = maxSpeed*.15;
  double speed = startSpeed;
  double rampSpeed = 10;
  double decMult = 2.5*per*.01;


  while(fabs(toTargetLeft) > 30 && fabs(toTargetRight) > 30){
    leftAvg = (leftF1.rotation(vex::deg) + leftB1.rotation(vex::deg)) / 2;
    rightAvg = (rightF1.rotation(vex::deg) + rightB1.rotation(vex::deg)) / 2;
    toTargetLeft = targetLeft - leftAvg;
    toTargetRight = targetRight - rightAvg;
    toTargetAvg = (toTargetLeft + toTargetRight) / 2;

    /////////////Do Not Change this////////////
    if(accel){
      if(speed<maxSpeed){
        speed += rampSpeed;
      }
      else{
        accel = false;
        accelDis = toTargetAvg;
      }
      if(toTargetAvg<distance/2){
        deccel = true;
        accelDis = toTargetAvg;
      };
    }
    else if(deccel){
      if(speed > .1*maxSpeed){
        speed -= rampSpeed*decMult;
      }
    }
    else{
      if(toTargetAvg>(accelDis)){
        speed = maxSpeed;
      }
      else{
        deccel = true;
      }
    }
    if(fabs(leftAvg) > fabs(rightAvg)+30){
      double difference = (leftSpeed-rightSpeed)/rightSpeed;
      leftSpeed = rightSpeed * (1-difference)*agg;
    }
    if(fabs(leftAvg) < fabs(rightAvg)+30){
      double difference = (rightSpeed-leftSpeed)/leftSpeed;
      leftSpeed = rightSpeed * (1-difference)*agg;
    }
    else{
      leftSpeed = speed;
      leftSpeed = speed;
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
  vex::task::sleep(500);

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
  vex::task::sleep(500);
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

void Robot::Track(double x, double y, double angApp){
  double angOpp;
  distx = x-xm0;
  disty = y-ym0;
  angEnd = (atan(disty/distx))*(180/M_PI);
  //Finds opposite turning angle
  if(angEnd < 0){
    angOpp = 90-angEnd;
  }
  else{
    angOpp = -90-angEnd;
  }
  //Turns a certain direction based off of input angle
  if(fabs(angApp-(angle0+angEnd))<fabs(angApp-(angle0+angOpp))){
    turn(angEnd,75);
  } else{
    turn(angOpp,75);
  }

  distx = x-xm0;
  disty = y-ym0;
  leng = pow((pow(distx, 2)+pow(disty, 2)), .5);

  move(leng,75);
}

*/