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

void opcontrol() {
	/* ---Variable Inits and Definintions --- */
	bool release = true;
	bool triggerRelease = true;
	double liftTarg = 0;
	bool revToggle = false;
	//Drive Power
	int lPow = 0;
	int rPow = 0;

	int t = 0;
	double pTarget = 0;
	int capTarget = 0;
	static int targIncrement = -CATAPULT_REV;

	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Controller partner(pros::E_CONTROLLER_PARTNER);

	//Motor Inits

	pros::Motor driveLF(DRIVE_LEFT_FRONT);
	pros::Motor driveLB(DRIVE_LEFT_BACK);
	pros::Motor driveRF(DRIVE_RIGHT_FRONT);
	pros::Motor driveRB(DRIVE_RIGHT_BACK);
	pros::Motor pultM(CATAPULT_MOTOR);
	pros::Motor ballInM(BALL_INTAKE);
	pros::Motor capFlipM(CAP_FLIPPER);
	pros::ADIPotentiometer ptmr(PTMR_PORT);

	capFlipM.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	/*Ptmr Values
	* Full Release: 1035
	* Load: 2155
	* Release: >2200
	*/


	//Main Loop
	while (true) {

		//Drive Controls (Voltage)
		//Flip Magnitude when Reversed
		lPow = master.get_analog(ANALOG_LEFT_Y);
		rPow = -master.get_analog(ANALOG_RIGHT_Y);

		//Flip Side when Reversed
		driveLF.move(revToggle ? lPow : rPow);
		driveLB.move(revToggle ? lPow : rPow);
		driveRF.move(revToggle ? rPow : lPow);
		driveRB.move(revToggle ? rPow : lPow);


		/*
		//Old Method
		lPow = revToggle ? 1 * master.get_analog(ANALOG_LEFT_Y) : -1 * master.get_analog(ANALOG_RIGHT_Y)
		rPow = revToggle ? -1 * master.get_analog(ANALOG_RIGHT_Y) : 1 * master.get_analog(ANALOG_LEFT_Y)


		*/

		/* ---Misc. Button Controls--- */
		//Catapult
		//pultM.move_absolute(-pTarget, 50);
		if (master.get_digital(DIGITAL_A) && release) {
			release = false;
			pultM.move_relative(targIncrement, 100);
			t = 2000;
			//pTarget = pTarget + targIncrement;
		}

		if (t <= 0) {
			pultM.move(0);
		}


		/* Logic Brainstorming
		* Use ptmr to detect when cat set, base pTarget on given value ()
		*
		*
		*/

		//Cap Flipper
		capFlipM.move(127 * (master.get_digital(DIGITAL_R2)-master.get_digital(DIGITAL_R1)));

		/* //Busted AF
		if (master.get_digital(DIGITAL_R2) || master.get_digital(DIGITAL_R1)) {
			capFlipM.move(127 * (master.get_digital(DIGITAL_R2)-master.get_digital(DIGITAL_R1)));
		} else {
			capFlipM.move_absolute(liftTarg, 10);
		}
		if (master.get_digital(DIGITAL_R2) || master.get_digital(DIGITAL_R1)) {triggerRelease = false;}

		if (!master.get_digital(DIGITAL_R2) && !master.get_digital(DIGITAL_R1) && triggerRelease == false) {
			liftTarg = capFlipM.get_position();
			triggerRelease = true;
		}*/



		//Ball Intake
		ballInM.move(-90 * (master.get_digital(DIGITAL_L2)-master.get_digital(DIGITAL_L1)));

		//Drive Direction Toggle
		if (master.get_digital(DIGITAL_Y) && release) {
			release = false;
			revToggle = !revToggle;
		}

		if (!master.get_digital(DIGITAL_A) && !master.get_digital(DIGITAL_Y)) {release = true;}

		//TODO: Brake Logic

		//Auton call (used in testing & debugging autons w/o competition switch)
		if(master.get_digital(DIGITAL_UP) && master.get_digital(DIGITAL_RIGHT)) {
			autonomous();
		}

		t = t - 20;
		pros::delay(20);
	}
}
