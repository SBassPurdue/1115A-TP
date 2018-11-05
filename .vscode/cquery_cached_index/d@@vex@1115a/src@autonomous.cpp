#include "main.h"

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
void autonomous() {
  int autoMode = 0;
  pros::Motor driveL(DRIVE_LEFT);
	pros::Motor driveR(DRIVE_RIGHT);
	pros::Motor puncherM1(PUNCHER_ONE);
	pros::Motor puncherM2(PUNCHER_TWO);

  if(autoMode == 0) {
    puncherM1.move(127);
    puncherM2.move(-127);
    pros::delay(500);
    puncherM1.move(0);
    puncherM2.move(0);
    driveL.move(-90);
    driveR.move(90);
    pros::delay(3000);
  }
  if(autoMode == 1) {

  }
  if(autoMode == -1) {
    pros::lcd::set_text(1, "I'm sad :(");
  }
}
