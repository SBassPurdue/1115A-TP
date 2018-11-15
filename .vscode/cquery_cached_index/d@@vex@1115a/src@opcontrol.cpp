#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void capSpinTaskFn(void* param) {
	bool release = true;
	double changeFactor = 0;
	double motorOld = 0;
	double target = 0;
	pros::Motor capRotateM(CAP_ROTATOR);
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Controller partner(pros::E_CONTROLLER_PARTNER);

	while (true) {

		if ((master.get_digital(DIGITAL_UP) || master.get_digital(DIGITAL_DOWN) || master.get_digital(DIGITAL_LEFT) || master.get_digital(DIGITAL_RIGHT)) && release) {
			release = false;
			changeFactor = ((450 * (master.get_digital(DIGITAL_UP)-master.get_digital(DIGITAL_DOWN))) + (25 * (master.get_digital(DIGITAL_LEFT) - master.get_digital(DIGITAL_RIGHT))));
		}
		if (!(master.get_digital(DIGITAL_UP) || master.get_digital(DIGITAL_DOWN) || master.get_digital(DIGITAL_LEFT) || master.get_digital(DIGITAL_RIGHT))) {release = true;}


		target = target + changeFactor;
		capRotateM.move_absolute(target, 100);
		changeFactor = 0;
		pros::delay(10);
	}
}
void opcontrol() {

	bool release = true;
	bool revToggle = false;
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Controller partner(pros::E_CONTROLLER_PARTNER);
	pros::Task capSpin(capSpinTaskFn, 0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Cap Spin Task");


	pros::Motor driveL(DRIVE_LEFT);
	pros::Motor driveR(DRIVE_RIGHT);
	pros::Motor liftL(LIFT_LEFT);
	pros::Motor liftR(LIFT_RIGHT);
	pros::Motor puncherM1(PUNCHER_ONE);
	pros::Motor puncherM2(PUNCHER_TWO);
	pros::Motor ballInM(BALL_INTAKE);

	while (true) {

		//Drive Controls (Voltage)
		driveL.move(revToggle ? 1 * master.get_analog(ANALOG_LEFT_Y) : -1 * master.get_analog(ANALOG_RIGHT_Y));
		driveR.move(revToggle ? -1 * master.get_analog(ANALOG_RIGHT_Y) : 1 * master.get_analog(ANALOG_LEFT_Y));

		//Lift Controls
		liftR.move(-127 * (master.get_digital(DIGITAL_R1)-master.get_digital(DIGITAL_R2)));
		liftL.move(127 * (master.get_digital(DIGITAL_R1)-master.get_digital(DIGITAL_R2)));

		/*Misc. Button Controls*/
		//Puncher
		puncherM1.move(127 * (master.get_digital(DIGITAL_A)));
		puncherM2.move(-127 * (master.get_digital(DIGITAL_A)));

		//Cap Rotation
		//capRotateM.move(127 * (master.get_digital(DIGITAL_LEFT)-master.get_digital(DIGITAL_RIGHT)));

		//Ball Intake
		ballInM.move(70 * (master.get_digital(DIGITAL_L2)-master.get_digital(DIGITAL_L1)));

		//Drive Direction Toggle
		if (master.get_digital(DIGITAL_Y) && release) {
			release = false;
			revToggle = !revToggle;
		}
		if (!master.get_digital(DIGITAL_Y)) {release = true;}

		if(master.get_digital(DIGITAL_X) && master.get_digital(DIGITAL_A)) {
			autonomous();
		}

		pros::delay(20);
	}
}
