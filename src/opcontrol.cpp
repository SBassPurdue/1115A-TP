#include "main.h"
#include <math.h>
#include "autohook.h"

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
	/* --- Parameter Definitions ---*/
	static int redOffset = 140;
	static int blueOffset = 140;
	/* ---Variable Inits and Definitions --- */
	bool releaseA = true;
	bool releaseB = true;
	bool releaseY = true;
	bool releaseX = true;
	bool releaseSide = true;
	bool angleRelease = true;
	bool revToggle = false;
	bool targetBlue = false;
	//Drive Power
	int lPow = 0;
	int rPow = 0;
	int timeSinceB = 4000;

	double lfTarg = 0;
	double lbTarg = 0;
	double rfTarg = 0;
	double rbTarg = 0;
	int angleTarg = 0;

	//Controller Inits
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Controller partner(pros::E_CONTROLLER_PARTNER);

	//Motor Inits
	pros::Motor driveLF(DRIVE_LEFT_FRONT);
	pros::Motor driveLB(DRIVE_LEFT_BACK);
	pros::Motor driveRF(DRIVE_RIGHT_FRONT);
	pros::Motor driveRB(DRIVE_RIGHT_BACK);
	pros::Motor puncherM(PUNCHER_MOTOR);
	pros::Motor puncherAngleM(PUNCHER_ANGLE);
	pros::Motor ballInM(BALL_INTAKE);
	pros::Motor capArmM(CAP_ARM);

	//Camera Init
	pros::Vision camera(VISION_PORT);


	driveLF.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	driveLB.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	driveRF.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	driveRB.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	puncherAngleM.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	capArmM.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

	master.print(0, 4, "Aiming at %s", (targetBlue ? "Blue" : "Red "));

	//Main Loop
	while (true) {
		//Biggest detected color signature
		pros::vision_object_s_t largestRedFlag = camera.get_by_sig(0, 2);
		pros::vision_object_s_t largestBlueFlag = camera.get_by_sig(0, 3);

		/* ---Drive Controls (Voltage)--- */
		lPow =  master.get_analog(ANALOG_LEFT_Y);
		rPow =  -master.get_analog(ANALOG_RIGHT_Y);

		driveLF.move(revToggle ? rPow : lPow);
		driveLB.move(revToggle ? rPow : lPow);
		driveRF.move(revToggle ? lPow : rPow);
		driveRB.move(revToggle ? lPow : rPow);

		/* ---Misc. Button Controls--- */

		//Cap Flipper
		capArmM.move(60 * (master.get_digital(DIGITAL_UP) - master.get_digital(DIGITAL_DOWN)));

		//Puncher
		puncherM.move(-127 * master.get_digital(DIGITAL_A));
		if (master.get_digital(DIGITAL_A)) {releaseA = false;}
		if (!master.get_digital(DIGITAL_A) && !releaseA) {
			releaseA = true;
			angleRelease = true;
		}

		//Angle Changer
		if (releaseX) {
			if (master.get_digital(DIGITAL_R1)) {angleTarg = -350; angleRelease = false;}
			if (master.get_digital(DIGITAL_R2)) {angleTarg = 0; angleRelease = false;}
		}
		puncherAngleM.move_absolute(angleTarg, 50);

		/*
		if (master.get_digital(DIGITAL_UP) || master.get_digital(DIGITAL_DOWN)) {
			puncherAngleM.move(-60 * (master.get_digital(DIGITAL_UP) - master.get_digital(DIGITAL_DOWN)));
			angleTarg = puncherAngleM.get_position();
		}
		*/
		//Manually reset the encoder
		if (master.get_digital(DIGITAL_LEFT)) {
			puncherAngleM.move(0);
			puncherAngleM.set_zero_position(puncherAngleM.get_position());
		}

		//Ball Intake
		ballInM.move(-110 * (master.get_digital(DIGITAL_L2)-master.get_digital(DIGITAL_L1)));

		//Drive Direction Toggle
		/*
		if (master.get_digital(DIGITAL_Y) && releaseY) {
			releaseY = false;
			revToggle = !revToggle;
		}
		*/
		//right --> positive

		if (master.get_digital(DIGITAL_Y)) {
			if (targetBlue && largestBlueFlag.width > 5) {
				driveRF.move_velocity((-0.5 * (blueOffset - largestBlueFlag.x_middle_coord)) - master.get_analog(ANALOG_LEFT_Y));
				driveRB.move_velocity((-0.5 * (blueOffset - largestBlueFlag.x_middle_coord)) - master.get_analog(ANALOG_LEFT_Y));
				driveLF.move_velocity((-0.5 * (blueOffset - largestBlueFlag.x_middle_coord)) + master.get_analog(ANALOG_LEFT_Y));
				driveLB.move_velocity((-0.5 * (blueOffset - largestBlueFlag.x_middle_coord)) + master.get_analog(ANALOG_LEFT_Y));
			}
			if (!targetBlue && largestRedFlag.width > 5) {
				driveRF.move_velocity((-0.5 * (redOffset - largestRedFlag.x_middle_coord)) - master.get_analog(ANALOG_LEFT_Y));
				driveRB.move_velocity((-0.5 * (redOffset - largestRedFlag.x_middle_coord)) - master.get_analog(ANALOG_LEFT_Y));
				driveLF.move_velocity((-0.5 * (redOffset - largestRedFlag.x_middle_coord)) + master.get_analog(ANALOG_LEFT_Y));
				driveLB.move_velocity((-0.5 * (redOffset - largestRedFlag.x_middle_coord)) + master.get_analog(ANALOG_LEFT_Y));
			}
		}

		if (!master.get_digital(DIGITAL_Y)) {releaseY = true;}

		//Brake Button
		if (master.get_digital(DIGITAL_X) && releaseX) {
			releaseX = false;
			lfTarg = driveLF.get_position();
			lbTarg = driveLB.get_position();
			rfTarg = driveRF.get_position();
			rbTarg = driveRB.get_position();
		} else if(master.get_digital(DIGITAL_X)) {
			driveLF.move_absolute(lfTarg, 130);
			driveLB.move_absolute(lbTarg, 130);
			driveRF.move_absolute(rfTarg, 130);
			driveRB.move_absolute(rbTarg, 130);
		}
		if (!master.get_digital(DIGITAL_X)) {releaseX = true;}

		//Big block of logic for double fire button
		if (master.get_digital(DIGITAL_B) && releaseB) {
			releaseB = false;
			angleTarg == 0 ? timeSinceB = 0 : timeSinceB = -400;
			angleTarg = 0;
		}

		if (!releaseB) {
			if (timeSinceB > 0 && timeSinceB <= 1000) {puncherM.move(-127);} else {puncherM.move(0);}
			if (1000 < timeSinceB && timeSinceB < 1400) {
				puncherM.move(0);
				angleTarg = -350;
			}
			if (1400 < timeSinceB){ballInM.move(110);}
			if (1900 < timeSinceB) {puncherM.move(-127);}
		}

		if (!master.get_digital(DIGITAL_B)) {releaseB = true;}

		//Update target color for vision aiming
		if (releaseSide && (master.get_digital(DIGITAL_LEFT) || master.get_digital(DIGITAL_RIGHT))) {
			releaseSide = false;
			targetBlue = !targetBlue;
			master.print(0, 4, "Aiming at %s", (targetBlue ? "Blue" : "Red "));
		}
		if (!(master.get_digital(DIGITAL_LEFT) || master.get_digital(DIGITAL_RIGHT))) {
			releaseSide = true;
		}

		//Auton call (used in testing & debugging autons w/o competition switch)
		if(master.get_digital(DIGITAL_LEFT) && master.get_digital(DIGITAL_RIGHT)) {
			autonomous();
		}

		timeSinceB = timeSinceB + 20;
		pros::delay(20);
	}
}
