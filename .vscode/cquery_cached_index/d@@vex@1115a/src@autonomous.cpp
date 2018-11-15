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
int autoMode;
void autonomous() {

  pros::Motor driveL(DRIVE_LEFT);
	pros::Motor driveR(DRIVE_RIGHT);
	pros::Motor liftL(LIFT_LEFT);
	pros::Motor liftR(LIFT_RIGHT);
	pros::Motor puncherM1(PUNCHER_ONE);
	pros::Motor puncherM2(PUNCHER_TWO);
	pros::Motor ballInM(BALL_INTAKE);

  if(autoMode == 0) {
    ballInM.move(127);
    driveL.move_relative(100, 100);
    driveR.move_relative(100, 100);
    while(!(driveL.is_stopped() && driveR.is_stopped())) {
      pros::delay(2);
    }
  }
  if(autoMode == 1) {

  }
  if(autoMode == -1) {

  }
}



/*pros::lcd::set_text(1, "I'm sad :(");
pros::lcd::set_text(8, "And alone ;_;");*/
