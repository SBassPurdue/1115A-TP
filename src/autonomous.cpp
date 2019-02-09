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

pros::Motor driveLF(DRIVE_LEFT_FRONT);
pros::Motor driveLB(DRIVE_LEFT_BACK);
pros::Motor driveRF(DRIVE_RIGHT_FRONT);
pros::Motor driveRB(DRIVE_RIGHT_BACK);
pros::Motor ballInM(BALL_INTAKE);
pros::Motor puncherM(PUNCHER_MOTOR);
pros::Motor puncherAngleM(PUNCHER_ANGLE);


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
  angle = angle * 9;
  int useAngle = floor(angle);
  driveLF.move_relative(-useAngle, speed);
  driveLB.move_relative(-useAngle, speed);
  driveRF.move_relative(-useAngle, speed);
  driveRB.move_relative(-useAngle, speed);
}

void firePuncher() {
  int t = 900;
  while(t > 0) {
    puncherM.move(-127);
    t = t - 20;
    pros::delay(20);
  }
  puncherM.move(0);
}

void firstCapRoutine() {
  //Move to the cap
  drive(3000, 110);
  pros::delay(500);
  settleDrive(-1);
  drive(1250, 50);
  //Succ the ball
  ballInM.move(70);
  settleDrive(-1);
  pros::delay(2500);
  ballInM.move_relative(-250, 50);
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

void autonomous() {

  if(autoMode == 1) {
    firstCapRoutine();
    //Go Home
    drive(-4000, 110);
    settleDrive(-1);
    //Touch base
    drive(-500, 50);
    pros::delay(1000);
    drive(500, 50);
    settleDrive(-1);
    turn(90, 80);
    pros::delay(1500);
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-500, 100);
    pros::delay(500);
    firePuncher();
    turn(10, 80);
    puncherAngleM.move(0);
    pros::delay(500);
    drive(4000, 100);
    settleDrive(-1);
  }

  if(autoMode == -1) {
    firstCapRoutine();
    //Go Home
    drive(-4000, 110);
    settleDrive(-1);
    //Touch base
    drive(-500, 50);
    pros::delay(1000);
    drive(500, 50);
    settleDrive(-1);
    turn(-90, 80);
    pros::delay(1500);
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-500, 100);
    pros::delay(500);
    firePuncher();
    turn(-10, 80);
    puncherAngleM.move(0);
    pros::delay(500);
    drive(4000, 100);
    settleDrive(-1);
  }

  if(autoMode == 2) {
    firstCapRoutine();
    //Platform align
    drive(-700, 110);
    settleDrive(1000);
    turn(-90, 80);
    pros::delay(1500);
    //Just fucking send it
    drive(-2250, 200);
    settleDrive(2000);
  }

  if(autoMode == -2) {
    firstCapRoutine();
    //Platform align
    drive(-700, 110);
    settleDrive(1000);
    turn(90, 80);
    pros::delay(1500);
    drive(-2250, 200);
    settleDrive(2000);
  }

  if(autoMode == 3 || autoMode == -3) {
    //Move to the cap
    drive(4250, 110);
    pros::delay(500);
    settleDrive(-1);
    //Succ the ball
    ballInM.move(70);
    settleDrive(-1);
    pros::delay(2500);
    ballInM.move_relative(-250, 50);
    //Go Home
    drive(-4000, 110);
    settleDrive(-1);
    //Touch base
    drive(-500, 50);
    pros::delay(1000);
    drive(500, 50);
    settleDrive(-1);
    turn(-90, 80);
    pros::delay(1500);
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-500, 100);
    pros::delay(500);
    firePuncher();
    turn(-10, 80);
    puncherAngleM.move(0);
    pros::delay(500);
    drive(4000, 100);
    settleDrive(-1);
  }
}


//pros::lcd::set_text(1, "I'm sad :(");
//pros::lcd::set_text(8, "And alone ;_;");
