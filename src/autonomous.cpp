#include <math.h>
#include "main.h"
#include "autohook.h"
#include "autofunct.h"

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
//1686
int count = 0;
static int catIncrement = -1900;

pros::Motor driveLF(DRIVE_LEFT_FRONT);
pros::Motor driveLB(DRIVE_LEFT_BACK);
pros::Motor driveRF(DRIVE_RIGHT_FRONT);
pros::Motor driveRB(DRIVE_RIGHT_BACK);
pros::Motor pultM(CATAPULT_MOTOR);
pros::Motor ballInM(BALL_INTAKE);
pros::Motor capFlipM(CAP_FLIPPER);
pros::ADIPotentiometer ptmr(PTMR_PORT);

//How tf does this work again?
void settleDrive(int t) {
  bool tgz = (t > 0);
  pros::delay(500);
  while(count < 100) {
    if ((abs(driveLF.get_target_position()-driveLF.get_position()) < 1000 && abs(driveRF.get_target_position()-driveRF.get_position()) < 1000) && ((!(abs(driveLF.get_actual_velocity()) < 3 || abs(driveRF.get_actual_velocity()) < 3)))) {
      count = count + 1;
    } else {
      count = 0;
    }
    if (tgz) {
      t = t - 2;
      if (t < 1) {
        break;
      }
    }
    pros::delay(2);
  }
  count = 0;
}

void drive(int distance, int speed) {
  driveLF.move_relative(distance, speed);
  driveLB.move_relative(distance, speed);
  driveRF.move_relative(-distance, speed);
  driveRB.move_relative(-distance, speed);
}

void turn(float angle, int speed) {
  angle = angle * 32 / 4;
  int useAngle = floor(angle);
  driveLF.move_relative(-useAngle, speed);
  driveLB.move_relative(-useAngle, speed);
  driveRF.move_relative(-useAngle, speed);
  driveRB.move_relative(-useAngle, speed);
}

void wait(int time) {
  while (time > 0) {
    driveLF.move_relative(0, 0);
    driveLB.move_relative(0, 0);
    driveRF.move_relative(-0, 0);
    driveRB.move_relative(-0, 0);
    time = time - 20;
    pros::delay(20);
  }
}

void fireCatapult() {
  pultM.move_relative(catIncrement, 100);
}

void autonomous() {

  if(autoMode == 1) {
    //Move to the cap
    drive(-3000, 110);
    pros::delay(500);
    settleDrive(-1);
    drive(-1250, 50);
    //Succ the ball
    ballInM.move(70);
    settleDrive(-1);
    pros::delay(2500);
    //Go home
    drive(4000, 150);
    pros::delay(400);
    settleDrive(-1);
    drive(1000, 50);
    pros::delay(500);
    //Scoot Forward
    drive(-480, 100);
    settleDrive(750);
    //Turn and fire
    turn(-90, 80);
    pros::delay(1500);
    drive(750, 50);
    settleDrive(500);
    wait(500);
    fireCatapult();
    pros::delay(500);
    //Go for bottom flag
    pros::delay(250);
    drive(2750, 100);
    pros::delay(4000);
  }

  if(autoMode == -1) {
    //Move to the cap
    drive(-3000, 110);
    pros::delay(500);
    settleDrive(-1);
    drive(-1250, 50);
    //Succ the ball
    ballInM.move(70);
    settleDrive(-1);
    pros::delay(2500);
    //Go home
    drive(4000, 150);
    pros::delay(400);
    settleDrive(-1);
    drive(1000, 50);
    pros::delay(500);
    //Scoot Forward
    drive(-480, 100);
    settleDrive(750);
    //Turn and fire
    turn(90, 80);
    pros::delay(1500);
    drive(750, 50);
    settleDrive(500);
    wait(500);
    fireCatapult();
    pros::delay(500);
    //Go for bottom flag
    pros::delay(250);
    drive(2750, 100);
    pros::delay(4000);
  }

  if(autoMode == 2) {
    //Move to the cap
    drive(-3000, 110);
    pros::delay(500);
    settleDrive(-1);
    drive(-1250, 50);
    //Succ the ball
    ballInM.move(70);
    settleDrive(-1);
    pros::delay(2500);
    //Platform align
    drive(750, 110);
    capFlipM.move_relative(-750, 50);
    settleDrive(1000);
    turn(-90, 80);
    pros::delay(1500);
    //Just fucking send it
    drive(2250, 200);
    settleDrive(2000);
  }

  if(autoMode == -2) {
    //Move to the cap
    drive(-3000, 110);
    pros::delay(500);
    settleDrive(-1);
    drive(-1250, 50);
    //Succ the ball
    ballInM.move(70);
    settleDrive(-1);
    pros::delay(2500);
    //Platform align
    drive(750, 110);
    capFlipM.move_relative(-750, 50);
    settleDrive(1000);
    turn(90, 80);
    pros::delay(1500);
    drive(2250, 200);
    settleDrive(2000);
  }

  if(autoMode == 3 || autoMode == -3) {
    fireCatapult();
    pros::delay(2000);
  }

/*
  //Six Points CLose Red Square
  if(autoMode == 2) {
    zeroDrive();
    //Move to the cap
    driveL.move_relative(-3000, 110);
    driveR.move_relative(3000, 110);
    settleDrive(-1);
    driveL.move_relative(-1250, 50);
    driveR.move_relative(1250, 50);
    //Succ the ball
    ballInM.move(-70);
    pros::delay(2500);
    ballInM.move(0);
    //Go home
    driveL.move_relative(4000, 150);
    driveR.move_relative(-4000, 150);
    pros::delay(400);
    ballInM.move(-50);
    settleDrive(-1);
    ballInM.move_relative(500, 90);
    driveL.move(60);
    driveR.move(-60);
    pros::delay(1000);
    driveL.move(0);
    driveR.move(0);
    //Scoot forward
    driveL.move_relative(-380, 100);
    driveR.move_relative(380, 100);
    settleDrive(750);
    //Turn to hit high flag
    driveL.move_relative(-780, 100);
    driveR.move_relative(-780, 100);
    settleDrive(750);
    driveL.move_relative(-100, 80);
    driveR.move_relative(100, 80);
    shoot();
    //Position for shot #2 (mid flag)
    driveL.move_relative(-1600, 80);
    driveR.move_relative(1600, 80);
    ballInM.move(-90);
    pros::delay(750);
    ballInM.move(0);
    settleDrive(750);
    shoot();
    //Turn and hit low flag
    driveL.move_relative(-250, 100);
    pros::delay(500);
    driveL.move_relative(-2500, 200);
    driveR.move_relative(2500, 200);
    settleDrive(-1);
  }

  if(autoMode == -2) {
    zeroDrive();
    //Move to the cap
    driveL.move_relative(-3000, 110);
    driveR.move_relative(3000, 110);
    settleDrive(-1);
    driveL.move_relative(-1250, 50);
    driveR.move_relative(1250, 50);
    //Succ the ball
    ballInM.move(-70);
    pros::delay(2500);
    ballInM.move(0);
    //Go home
    driveL.move_relative(4000, 150);
    driveR.move_relative(-4000, 150);
    pros::delay(400);
    ballInM.move(-50);
    settleDrive(-1);
    ballInM.move_relative(500, 90);
    driveL.move(60);
    driveR.move(-60);
    pros::delay(1000);
    driveL.move(0);
    driveR.move(0);
    //Scoot forward
    driveL.move_relative(-380, 100);
    driveR.move_relative(380, 100);
    settleDrive(750);
    //Turn to hit high flag
    driveL.move_relative(780, 100);
    driveR.move_relative(780, 100);
    settleDrive(750);
    driveL.move_relative(-100, 80);
    driveR.move_relative(100, 80);
    shoot();
    //Position for shot #2 (mid flag)
    driveL.move_relative(-1600, 80);
    driveR.move_relative(1600, 80);
    ballInM.move(-90);
    pros::delay(750);
    ballInM.move(0);
    settleDrive(750);
    shoot();
    //Turn and hit low flag
    driveR.move_relative(250, 100);
    pros::delay(500);
    driveL.move_relative(-2500, 200);
    driveR.move_relative(2500, 200);
    settleDrive(-1);
  }

  if (autoMode == 3) {
    deploy();
    //Move to the cap
    driveL.move_relative(-3000, 110);
    driveR.move_relative(3000, 110);
    pros::delay(250);
    puncherM.move_relative(-1000, 100);
    settleDrive(-1);
    driveL.move_relative(-1250, 50);
    driveR.move_relative(1250, 50);
    //Succ the ball
    ballInM.move(-70);
    pros::delay(2500);
    //Go back
    driveL.move_relative(2000, 100);
    driveR.move_relative(-2000, 100);
    settleDrive(-1);
    ballInM.move(0);
    //Turn to face Cap #2
    driveL.move_relative(-1620, 50);
    driveR.move_relative(-1620, 50);
    settleDrive(-1);
    //Get to Cap # 2
    capRotateM.move_relative(450, 200);
    liftR.move_relative(-75, 50);
    liftL.move_relative(75, 50);
    pros::delay(250);
    driveL.move_relative(1020, 100);
    driveR.move_relative(-1020, 100);
    settleDrive(-1);
    //Yeet that boi & go back
    capRotateM.move_relative(450, 150);
    pros::delay(100);
    driveL.move_relative(-650, 75);
    driveR.move_relative(650, 75);
    settleDrive(500);
    driveL.move_relative(250, 75);
    driveR.move_relative(250, 75);
    settleDrive(500);
    //Line up with the platform
    driveL.move(-70);
    driveR.move(70);
    pros::delay(1000);
    driveL.move_relative(500, 75);
    driveR.move_relative(-500, 75);
    settleDrive(750);
    driveL.move(-127);
    driveR.move(127);
    pros::delay(1000);
    driveL.move_relative(-1500, 250);
    driveR.move_relative(1500, 250);
    settleDrive(-1);



    //Move to skewer the first cap
    driveL.move_relative(3000, 110);
    driveR.move_relative(-3000, 110);
    pros::delay(500);
    capRotateM.move_relative(450, 100);
    settleDrive(-1);
    puncherM.move(0);
    driveL.move_relative(1500, 70);
    driveR.move_relative(-1500, 70);
    settleDrive(-1);
    driveL.move_relative(-2000, 110);
    driveR.move_relative(2000, 110);
    settleDrive(-1);
    driveL.move_relative(390, 100);
    driveR.move_relative(390, 100);
    settleDrive(750);
    driveL.move_relative(2000, 110);
    driveR.move_relative(-2000, 110);
    settleDrive(-1);

    capRotateM.set_zero_position(capRotateM.get_position());
  }

  if (autoMode == -3) {
    //Guess I'll die
  }

  if (autoMode == 4) {
    liftR.move_relative(-75, 50);
    liftL.move_relative(75, 50);
    pros::delay(500);
  } */
}


//pros::lcd::set_text(1, "I'm sad :(");
//pros::lcd::set_text(8, "And alone ;_;");
