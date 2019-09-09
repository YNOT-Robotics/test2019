//headers for the robot class
void autonomous();
void initialize(void);
void disabled(void);
void competition_initialize(void);
void opcontrol(void);
int driver(void);
int fire(void);
int down(void);
void nl();
void resetScreen();
int autoSelect();


class Robot{
  public:
    Robot(bool);
    void clearAll();
    void start();
    void liftCon();
    void WristCon();
    void toggleLift();
    void togglePusher();
    void intakeCon(int);
    void OPclear();
    void fire();
    void toggleWrist();
    void autonLift(int);
    void reset();
    void wait(int);
    void toggleMode();
    int liftState = 0;
    void CapDrop();

    //auton functions
    void autoInit();
    void autoFire();
    void move(double, double);
    void testMove(double,double);
    void turn(double,double);
    void trackingGrabber(double, double);
    void strafe(double, double);
    void endAuto();
    void Yeet();
    void Yoink(bool);
    void Grab(bool);
    void WristCheck();

    
    void auto1();
    void red1();
    void red2();
    void blue1();
    bool go = true;
    bool capSwitch = false;
  private:

    bool auton;
    bool driveOverride = false;
    bool state = false;
    bool catapultOn = true;
    bool r2Pressed = false;
    bool aPressed = false;
    int xPos = 0;
    int yPos = 0;
    int angle = 0;
    bool intakeOn = true;
    
    int distance = 0;
    double rotation = 0;
    bool Cap = false;
    bool bPress = false;
    int Broke = 0;
    bool Uppressed = false;
 };