/*
 * Pokémon Brilliant Diamond and Shining Pearl automation
 */

#include <util/delay.h>

#include "automation-utils.h"
#include "user-io.h"

/* Static functions */
static void temporary_control(void);
static void repeat_shaymin_encounter(void);
static void repeat_darkrai_encounter(void);
static void repeat_heatran_encounter(void);


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
				repeat_shaymin_encounter();
				break;
			case 3:
				repeat_darkrai_encounter();
				break;
			case 4:
				repeat_heatran_encounter();
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
void repeat_shaymin_encounter(void)
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
void repeat_darkrai_encounter(void)
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
 */
void repeat_heatran_encounter(void)
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

