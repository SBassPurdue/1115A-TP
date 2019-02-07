#include "main.h"
#include "autohook.h"
#include "mathfunct.h"
#include "display/lvgl.h"
#include "display/lv_misc/lv_color.h"


int i = 0;
int autoMode = 1;

//Auton Name Array
char autonsColor[4][20] = {"6-Pointer (Close)", "4-Pointer (Far)", "YEET", "Placeholder"};
int len = sizeof(autonsColor)/sizeof(autonsColor[0]);
int actId = 0;
//String to send to the roller UI element
char rollerInStr[2048] = "";

//UI Interactivity functions
static lv_res_t rollerSetR(lv_obj_t *roller) {
	autoMode = (lv_roller_get_selected(roller) + 1);
}

static lv_res_t rollerSetB(lv_obj_t *roller) {
	autoMode = (-1 * (lv_roller_get_selected(roller) + 1));
}

void scrollFnR(int dir, lv_obj_t *roller) {
	lv_roller_set_selected(roller, lv_roller_get_selected(roller) - dir, true);
	actId = 0;
	rollerSetR(roller);
}

void scrollFnB(int dir, lv_obj_t *roller) {
	lv_roller_set_selected(roller, lv_roller_get_selected(roller) - dir, true);
	actId = 0;
	rollerSetB(roller);
}



static lv_res_t scrollRU(lv_obj_t *btn)
{
	actId = 1;
	return LV_RES_OK;
}

static lv_res_t scrollRD(lv_obj_t *btn)
{
	actId = -1;
	return LV_RES_OK;
}

static lv_res_t scrollBU(lv_obj_t *btn)
{
	actId = 2;
	return LV_RES_OK;
}

static lv_res_t scrollBD(lv_obj_t *btn)
{
	actId = -2;
	return LV_RES_OK;
}

void autoPickFn(void* param) {

while (i < len) {strcat(rollerInStr, autonsColor[i]); i++; if(i != len) {strcat(rollerInStr, "\n");}}
	/*----UI Object Inits----*/
	//Tab View
	lv_obj_t *tabObj;
	tabObj = lv_tabview_create(lv_scr_act(), NULL);
		lv_obj_t *redTab = lv_tabview_add_tab(tabObj, "Red");
			lv_page_set_sb_mode(redTab, LV_SB_MODE_OFF);
		lv_obj_t *blueTab = lv_tabview_add_tab(tabObj, "Blue");
			lv_page_set_sb_mode(blueTab, LV_SB_MODE_OFF);
		lv_obj_t *skillsTab = lv_tabview_add_tab(tabObj, "Skills");
			lv_page_set_sb_mode(skillsTab, LV_SB_MODE_OFF);

	/*---Tab Contents---*/
	//Red Tab
	lv_obj_t *checkContRed = lv_cont_create(redTab, NULL);
		lv_obj_align(checkContRed, redTab, LV_ALIGN_IN_TOP_LEFT, 10, 10);
		lv_obj_set_size(checkContRed, 250, 150);

	lv_obj_t *redRoller = lv_roller_create(checkContRed, NULL);
		lv_roller_set_options(redRoller, rollerInStr);
		lv_obj_align(redRoller, NULL, LV_ALIGN_IN_LEFT_MID, 5, 0);
		lv_roller_set_action(redRoller, rollerSetR);

	lv_obj_t *btnContRed = lv_cont_create(checkContRed, NULL);
		lv_obj_align(btnContRed, NULL, LV_ALIGN_IN_TOP_RIGHT, 60, 15);
		lv_obj_set_size(btnContRed, 60, 120);

	lv_obj_t *redBtnUp = lv_btn_create(btnContRed, NULL);
		lv_obj_set_size(redBtnUp, 50, 50);
		lv_obj_align(redBtnUp, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
		lv_btn_set_action(redBtnUp, LV_BTN_ACTION_PR, scrollRU);

	lv_obj_t *redBtnDown = lv_btn_create(btnContRed, NULL);
		lv_obj_set_size(redBtnDown, 50, 50);
		lv_obj_align(redBtnDown, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -5);
		lv_btn_set_action(redBtnDown, LV_BTN_ACTION_PR, scrollRD);

	//Blue Tab
	lv_obj_t *checkContBlue = lv_cont_create(blueTab, NULL);
		lv_obj_align(checkContBlue, blueTab, LV_ALIGN_IN_TOP_LEFT, 10, 10);
		lv_obj_set_size(checkContBlue, 250, 150);

	lv_obj_t *blueRoller = lv_roller_create(checkContBlue, NULL);
		lv_roller_set_options(blueRoller, rollerInStr);
		lv_obj_align(blueRoller, NULL, LV_ALIGN_IN_LEFT_MID, 5, 0);
		lv_roller_set_action(blueRoller, rollerSetB);

	lv_obj_t *btnContBlue = lv_cont_create(checkContBlue, NULL);
		lv_obj_align(btnContBlue, NULL, LV_ALIGN_IN_TOP_RIGHT, 60, 15);
		lv_obj_set_size(btnContBlue, 60, 120);

	lv_obj_t *blueBtnUp = lv_btn_create(btnContBlue, NULL);
		lv_obj_set_size(blueBtnUp, 50, 50);
		lv_obj_align(blueBtnUp, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);
		lv_btn_set_action(blueBtnUp, LV_BTN_ACTION_PR, scrollBU);

	lv_obj_t *blueBtnDown = lv_btn_create(btnContBlue, NULL);
		lv_obj_set_size(blueBtnDown, 50, 50);
		lv_obj_align(blueBtnDown, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, -5);
		lv_btn_set_action(blueBtnDown, LV_BTN_ACTION_PR, scrollBD);

	//Skills Tab

	//Main Loop
	while (true) {
		while (actId != 0) {
			if(actId == 1 || actId == -1) {
				scrollFnR(actId, redRoller);
			}
			if(actId == 2 || actId == -2) {
				scrollFnB(actId > 0 ? 1 : -1, blueRoller);
			}
		}
		lv_btn_set_state(redBtnUp, LV_BTN_STATE_REL);
		lv_btn_set_state(redBtnDown, LV_BTN_STATE_REL);
		lv_btn_set_state(blueBtnUp, LV_BTN_STATE_REL);
		lv_btn_set_state(blueBtnDown, LV_BTN_STATE_REL);
		lv_roller_get_selected(redRoller) == 0 ? lv_btn_set_state(redBtnUp, LV_BTN_STATE_INA) : lv_roller_get_selected(redRoller) == len - 1 ? lv_btn_set_state(redBtnDown, LV_BTN_STATE_INA) : pros::delay(1);
		lv_roller_get_selected(blueRoller) == 0 ? lv_btn_set_state(blueBtnUp, LV_BTN_STATE_INA) : lv_roller_get_selected(blueRoller) == len - 1 ? lv_btn_set_state(blueBtnDown, LV_BTN_STATE_INA) : pros::delay(1);

		lv_tick_inc(20);
		pros::delay(20);
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
	pros::ADIAnalogIn ptmr (PTMR_PORT);


	/*int i = 7;
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
	}*/

	pros::Task autoPick(autoPickFn, 0, TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT, "Autonomous Picker");
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
