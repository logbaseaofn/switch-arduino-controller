/*
 * Pokémon Brilliant Diamond and Shining Pearl automation
 */

#include <util/delay.h>

#include "automation-utils.h"
#include "user-io.h"

/* Static functions */
static void temporary_control(void);
static void shaymin_encounter(void);
static void darkrai_encounter(void);
static void heatran_encounter(void);
static void long_encounter(void);
static void ramanas_encounter_with_time_reset(void);

// TODO Define the button sequences
// - generic (heatran, giratina, ramanas park)
// - darkrai (longer animation than generic)
// - shaymin (totally different reset method)

int main(void)
{
	init_automation();
	init_led_button();

	/* Initial beep to confirm that the buzzer works */
	beep();

	/* Wait for the user to press the button (should be on the Switch main menu) */
	count_button_presses(100, 100);

	/* Set the virtual controller as controller 1 */
	switch_controller(REAL_TO_VIRT);

	for (;;) {
		/* Set the LEDs, and make sure automation is paused while in the
		   menu */
		set_leds(BOTH_LEDS);
		pause_automation();

		/* Feature selection menu */
		uint8_t count = count_button_presses(100, 900);

		for (uint8_t i = 0 ; i < count ; i += 1) {
			beep();
			_delay_ms(100);
		}

		switch (count) {
			case 1:
				temporary_control();
				break;
			case 2:
				shaymin_encounter();
				break;
			case 3:
				darkrai_encounter();
				break;
			case 4:
				heatran_encounter();
				break;
			case 5:
				long_encounter();
				break;


			default:
				/* Wrong selection */
				delay(100, 200, 1500);
				break;
		}
	}
}

/*
 * Temporary gives back control to the user by performing controller switch.
 */
void temporary_control(void)
{
	set_leds(NO_LEDS);

	/* Allow the user to connect their controller back as controller 1 */
	switch_controller(VIRT_TO_REAL);

	/* Wait for the user to press the button (should be on the Switch main menu) */
	count_button_presses(100, 100);

	/* Set the virtual controller as controller 1 */
	switch_controller(REAL_TO_VIRT);
}

/*
 * Macro for encountering Shaymin
 */
void shaymin_encounter(void)
{
	setup_button_automation_interrupt();

	for (;;) {
		SEND_BUTTON_SEQUENCE(
			{ BT_NONE, DP_TOP, SEQ_HOLD, 2 }, /* Navigate to "run" */
			{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Select "run" */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 132 }, /* Wait for scene change */
			{ BT_A, DP_NEUTRAL, SEQ_HOLD, 8 }, /* Close "Shaymin disappeared dialog "*/
			{ BT_P, DP_NEUTRAL, SEQ_HOLD, 8 }, /* Get on bike */
			{ BT_NONE, DP_BOTTOM, SEQ_HOLD, 64 }, /* Ride down out of area */
			{ BT_NONE, DP_TOP, SEQ_HOLD, 65 }, /* Ride back to Shaymin */
			{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Talk to Shaymin */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 64 }, /* Wait for Shaymin cry to finish */
			{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Start Shaymin encounter */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 512 } /* Wait for menu to pop up again */
		);

		if (interrupted_by_button()) {
			return;
		}
	}
}

/*
 * Macro for encountering Darkrai
 */
void darkrai_encounter(void)
{
	setup_button_automation_interrupt();

	for (;;) {
		SEND_BUTTON_SEQUENCE(
			{ BT_H, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Home menu */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 24 }, /* Wait for main menu to load */
			{ BT_X, DP_NEUTRAL, SEQ_HOLD, 16 }, /* Close software */
			{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Confirm close software */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 64 }, /* Wait for software to close */
			{ BT_A, DP_NEUTRAL, SEQ_MASH, 544 }, /* Start software all the way
																							through encounter, I think only
																							updates where mashed is counted
																							in the timer */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 272 } /* Wait until just after we get a
																								good look at Darkrai */
		);

		if (interrupted_by_button()) {
			return;
		}
	}
}

/*
 * Macro for encountering Heatran
 *
 * This timing also works well for Giratina
 */
void heatran_encounter(void)
{
	setup_button_automation_interrupt();

	for (;;) {
		SEND_BUTTON_SEQUENCE(
			{ BT_H, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Home menu */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 24 }, /* Wait for main menu to load */
			{ BT_X, DP_NEUTRAL, SEQ_HOLD, 16 }, /* Close software */
			{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Confirm close software */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 64 }, /* Wait for software to close */
			{ BT_A, DP_NEUTRAL, SEQ_MASH, 544 }, /* Start software all the way
																							through encounter, I think only
																							updates where mashed is counted
																							in the timer */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 230 } /* Wait until just after we get a
																								good look at Heatran */
		);

		if (interrupted_by_button()) {
			return;
		}
	}
}

/*
 * Same as heatran, but it will wait until the pokemon is visible on screen
 * given the pokemon sent out has no attachment to the player. Intended for Regice.
 *
 * This works for all ramanas park pokemon given your lead does not have an
 * ability. It even works with the long ability animations of Kyogre and
 * Groudon as well as Regigigas
 */
void long_encounter(void)
{
	setup_button_automation_interrupt();

	for (;;) {
		SEND_BUTTON_SEQUENCE(
			{ BT_H, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Home menu */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 24 }, /* Wait for main menu to load */
			{ BT_X, DP_NEUTRAL, SEQ_HOLD, 16 }, /* Close software */
			{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Confirm close software */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 64 }, /* Wait for software to close */
			{ BT_A, DP_NEUTRAL, SEQ_MASH, 544 }, /* Start software all the way
																							through encounter, I think only
																							updates where mashed is counted
																							in the timer */
			{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 444 } /* Wait until after battle menu
																								pops up, with buffering for
																								possible shiny animation */
		);

		if (interrupted_by_button()) {
			return;
		}
	}
}

/**
 * This function encounters pokemon in Ramanas Park and rolls the time back an
 * hour approximately every hour. This is to hopefully make colors of
 * non-shinies consistent
 */
// TODO It looks like ramanas always looks like the same background, but the
// time thing works and would be useful for Shaymin (and maybe Arceus?) going
// to keep it here for now
void ramanas_encounter_with_time_reset(void)
{
	setup_button_automation_interrupt();

	for (;;) {
		// The main button sequence takes approximately 57 seconds. For an entire
		// hour of button sequences, the macro will execute 63.157 times. To avoid
		// resetting the time such that time runs backwards, it will be 65 times.
		// This the overshooting, for time to jump 5 hours (enough for the daylight
		// to change if started at 10AM) this macro must run for at least a week
		// straight. There is a 7% chance that a shiny won't have been found by
		// then (and even more unlikely that the user hasn't restarted the
		// macro/reset the time manually) at this point so I will not be making the
		// requirements even tighter.
		for (int i = 0; i < 65; i++) {
			//SEND_BUTTON_SEQUENCE(
			//	{ BT_H, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Home menu */
			//	{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 24 }, /* Wait for main menu to load */
			//	{ BT_X, DP_NEUTRAL, SEQ_HOLD, 16 }, /* Close software */
			//	{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Confirm close software */
			//	{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 64 }, /* Wait for software to close */
			//	{ BT_A, DP_NEUTRAL, SEQ_MASH, 544 }, /* Start software all the way
			//																					through encounter, I think only
			//																					updates where mashed is counted
			//																					in the timer */
			//	{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 230 } /* Wait until just after we get a
			//																						good look at Heatran */
			//);

			SEND_BUTTON_SEQUENCE(
				{ BT_H, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Home menu */
				{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 24 }, /* Wait for main menu to load */
				{ BT_NONE, DP_BOTTOM, SEQ_HOLD, 2 }, /* Navigate to bubble buttons */
				{ BT_NONE, DP_RIGHT, SEQ_HOLD, 24 }, /* Navigate to "sleep mode" */
				{ BT_NONE, DP_LEFT, SEQ_HOLD, 2 }, /* Navigate to "settings" */
				{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Enter "settings" */
				{ BT_NONE, DP_BOTTOM, SEQ_HOLD, 64 }, /* Navigate to "System" */
				{ BT_NONE, DP_RIGHT, SEQ_HOLD, 2 }, /* Make pane active */
				{ BT_NONE, DP_BOTTOM, SEQ_HOLD, 18 }, /* Navigate to "Date and Time" */
				{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Enter time and date settings */
				{ BT_NONE, DP_BOTTOM, SEQ_HOLD, 32 }, /* Navigate to "Date and Time" */
				{ BT_A, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Enter "Date and Time"*/
				{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 8 }, /* Wait for date-time form to popup */
				{ BT_NONE, DP_RIGHT, SEQ_MASH, 3 }, /* Navigate to "Hour" */
				{ BT_NONE, DP_BOTTOM, SEQ_MASH, 1 }, /* Back an hour */
				{ BT_A, DP_NEUTRAL, SEQ_MASH, 4 }, /* Accept */
				{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 8 }, /* Wait */
				{ BT_H, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Navigate to home menu */
				{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 32 }, /* Wait for home to load */
				{ BT_H, DP_NEUTRAL, SEQ_HOLD, 2 }, /* Enter game again */
				{ BT_NONE, DP_NEUTRAL, SEQ_HOLD, 64 } /* Wait for game to load */
			);

			if (interrupted_by_button()) {
				return;
			}
		}
	}
}
