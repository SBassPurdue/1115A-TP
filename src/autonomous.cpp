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
pros::ADIDigitalIn intakeLimit(LIMITSWITCH_PORT);


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

void align(int power, int time) {
  while(time > 0) {
    driveLF.move(power);
    driveLB.move(power);
    driveRF.move(-power);
    driveRB.move(-power);
    time = time - 20;
    pros::delay(20);
  }
}

void turn(float angle, int speed) {
  angle = angle * 9;
  int useAngle = floor(angle);
  driveLF.move_relative(-useAngle, speed);
  driveLB.move_relative(-useAngle, speed);
  driveRF.move_relative(-useAngle, speed);
  driveRB.move_relative(-useAngle, speed);
}

void armPuncher() {
  puncherM.move_relative(-900, 100);
}

void firePuncher() {
  puncherM.move_relative(-1800, 100);
  pros::delay(500);
}

void intakeUntilBall() {
  int timeout = 5000;
  ballInM.move(100);
  while(!pros::c::adi_digital_read(LIMITSWITCH_PORT) && timeout > 0) {timeout = timeout - 20; pros::delay(20);}
  ballInM.move(0);
}

void firstCapRoutine() {
  //Move to the cap
  drive(3000, 160);
  pros::delay(500);
  settleDrive(-1);
  drive(750, 50);
  //Succ the ball
  ballInM.move(100);
  settleDrive(-1);
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
  //Red CLose Tile
  if(autoMode == 1) {
    firstCapRoutine();
    pros::delay(500);
    //Go Home
    drive(-3500, 200);
    settleDrive(-1);
    //Touch base
    align(-30, 250);
    armPuncher();
    drive(500, 50);
    settleDrive(750);
    turn(90, 80);
    pros::delay(1000);
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-350, 100);
    while(puncherAngleM.get_position() > -340) {pros::delay(20);}
    firePuncher();
    puncherAngleM.move(0);
    turn(5, 80);
    pros::delay(500);
    drive(4000, 200);
    settleDrive(-1);
    align(60, 500);
    drive(-2000, 200);
    settleDrive(500);
    turn(-90, 80);
    pros::delay(750);
    ballInM.move(-100);
    drive(3250, 200);
    settleDrive(-1);
    turn(60, 80);
    pros::delay(500);
    firePuncher();
    turn(15, 80);
    pros::delay(250);
    align(100, 1000);
  }
  //Blue Close Tile
  if(autoMode == -1) {
    firstCapRoutine();
    pros::delay(500);
    //Go Home
    drive(-3500, 200);
    settleDrive(-1);
    //Touch base
    align(-30, 250);
    armPuncher();
    drive(500, 50);
    settleDrive(750);
    turn(-90, 80);
    pros::delay(1000);
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-350, 100);
    while(puncherAngleM.get_position() > -340) {pros::delay(20);}
    firePuncher();
    puncherAngleM.move(0);
    turn(-5, 80);
    pros::delay(500);
    drive(4000, 200);
    settleDrive(-1);
    align(60, 500);
    drive(-2000, 200);
    settleDrive(500);
    turn(90, 80);
    pros::delay(750);
    ballInM.move(-100);
    drive(3250, 200);
    settleDrive(-1);
    turn(-60, 80);
    pros::delay(500);
    firePuncher();
    turn(-15, 80);
    pros::delay(250);
    align(100, 1000);
  }

  //Red Far Tile
  if(autoMode == 2) {
    firstCapRoutine();
    pros::delay(500);
    ballInM.move_relative(-200, 50);
    //Line up to shoot
    drive(-1750, 110);
    settleDrive(-1);
    turn(80, 80);
    pros::delay(750);
    puncherAngleM.move_absolute(-170, 100);
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-270, 100);
    pros::delay(500);
    firePuncher();
    //Realign with the platform
    turn(-70, 80);
    pros::delay(750);
    drive(700, 50);
    settleDrive(500);
    turn(-90, 80);
    settleDrive(750);
    align(-20, 750);
    //Just fucking send it
    drive(-1600, 200);
    settleDrive(2000);
  }
  //Blue Far Tile
  if(autoMode == -2) {
    firstCapRoutine();
    pros::delay(500);
    ballInM.move_relative(-200, 50);
    //Line up to shoot
    drive(-1750, 110);
    settleDrive(-1);
    turn(-80, 80);
    pros::delay(750);
    puncherAngleM.move_absolute(-170, 100);
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-270, 100);
    pros::delay(500);
    firePuncher();
    //Realign with the platform
    turn(70, 80);
    pros::delay(750);
    drive(700, 50);
    settleDrive(500);
    turn(90, 80);
    settleDrive(750);
    align(-20, 750);
    //Just fucking send it
    drive(-1600, 200);
    settleDrive(2000);
  }
  //Skills
  if(autoMode == 3 || autoMode == -3) {
    //Inital align to zero things out
    align(-20, 500);
    //Move to the cap
    drive(4200, 100);
    pros::delay(500);
    ballInM.move(-60);
    settleDrive(-1);
    //Succ the ball
    drive(250, 75);
    intakeUntilBall();
    //Go align with the flags
    drive(-3000, 110);
    settleDrive(-1);
    turn(-90, 80);
    pros::delay(950);
    drive(3750, 110);
    settleDrive(-1);
    turn(90, 80);
    pros::delay(950);
    drive(-1500, 50);
    pros::delay(1000);
    align(-30, 750);
    drive(2100, 90);
    settleDrive(-1);
    turn(-90, 80);
    pros::delay(950);
    //Double fire on the right flags
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-300, 100);
    pros::delay(500);
    firePuncher();
    puncherAngleM.move_absolute(0, 100);
    //Go for the second cap & ball
    pros::delay(100);
    turn(90, 80);
    pros::delay(950);
    ballInM.move(-60);
    drive(2400, 110);
    settleDrive(-1);
    //Succ the ball
    drive(250, 75);
    intakeUntilBall();
    //Finish taking in the ball from earlier (no. 2)
    ballInM.move(100);
    drive(-750, 110);
    settleDrive(300);
    ballInM.move(0);
    //Move towards the middle bottom flag
    turn(-45, 80);
    pros::delay(950);
    drive(4200, 110);
    settleDrive(-1);
    //Face mid bot flag
    turn(-50, 80);
    pros::delay(950);
    //Hit mid bot flag
    drive(2000, 80);
    settleDrive(-1);
    align(30, 500);
    //Back up and move toward 3rd ball & cap
    drive(-2000, 110);
    settleDrive(-1);
    turn(-45, 80);
    pros::delay(950);
    drive(-3400, 110);
    settleDrive(-1);
    turn(-50, 80);
    pros::delay(950);
    //Get 3rd ball and cap
    ballInM.move(-60);
    drive(2000, 110);
    settleDrive(-1);
    //Succ the ball
    drive(250, 75);
    intakeUntilBall();
    drive(-3300, 110);
    settleDrive(-1);
    align(-30, 1500);
    //Front auton's shoot from wall align
    drive(500, 50);
    settleDrive(750);
    turn(90, 80);
    pros::delay(1000);
    turn(-5, 80);
    firePuncher();
    ballInM.move(110);
    puncherAngleM.move_absolute(-300, 100);
    pros::delay(500);
    firePuncher();
    puncherAngleM.move(0);
    turn(10, 80);
    ballInM.move(0);
    pros::delay(500);
    drive(4000, 110);
    settleDrive(-1);
    align(30, 1000);
    drive(-5850, 110);
    settleDrive(-1);
    turn(90, 80);
    pros::delay(950);
    align(30, 1000);
    drive(-1500, 80);
    settleDrive(750);
    align(-30, 1000);
    //Just fucking send it
    drive(-2250, 200);
    settleDrive(2000);
    align(-30, 500);
    //Just fucking send it 2: electric boogaloo
    drive(-1600, 200);
    settleDrive(2000);
  }
}


//pros::lcd::set_text(1, "I'm sad :(");
//pros::lcd::set_text(8, "And alone ;_;");
