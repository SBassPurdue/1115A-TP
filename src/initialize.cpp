#include "main.h"

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	int i = 7;
	char lyricsArray[57][40] = {"I hear the drums echoing tonight", "But she hears only whispers", "of some quiet conversation", "She's coming in, 12:30 flight", "The moonlit wings reflect the stars", "that guide me towards salvation", "I stopped an old man along the way", "Hoping to find some long forgotten", "words or ancient melodies", "He turned to me as if to say,", "\"Hurry boy, it's waiting there for you\"", "It\'s gonna take a lot to", "take me away from you", "There\'s nothing that a hundred", "men or more could ever do", "I bless the rains down in Africa", "Gonna take some time to do", "the things we never had", "The wild dogs cry out in the night", "As they grow restless,", "longing for some solitary company", "I know that I must do what's right", "As sure as Kilimanjaro rises like", "Olympus above the Serengeti", "I seek to cure what's deep inside,", "frightened of this thing", "that I\'ve become", "It\'s gonna take a lot to", "drag me away from you", "There\'s nothing that a hundred", "men or more could ever do", "I bless the rains down in Africa", "Gonna take some time to do", "the things we never had", "Hurry boy, she's waiting there for you", "It's gonna take a lot to", "drag me away from you", "There's nothing that a hundred", "men or more could ever do", "I bless the rains down in Africa", "I bless the rains down in Africa", "(I bless the rain)", "I bless the rains down in Africa", "(I bless the rain)", "I bless the rains down in Africa", "I bless the rains down in Africa", "(Ah, gonna take the time)", "Gonna take some time to do", "the things we never had"};
	while(i < 57) {
		pros::lcd::set_text(0, lyricsArray[i-7]);
		pros::lcd::set_text(1, lyricsArray[i-6]);
		pros::lcd::set_text(2, lyricsArray[i-5]);
		pros::lcd::set_text(3, lyricsArray[i-4]);
		pros::lcd::set_text(4, lyricsArray[i-3]);
		pros::lcd::set_text(5, lyricsArray[i-2]);
		pros::lcd::set_text(6, lyricsArray[i-1]);
		pros::lcd::set_text(7, lyricsArray[i]);
		pros::delay(20);
		i++;
	}
	pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
