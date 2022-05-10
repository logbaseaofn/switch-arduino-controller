Pokémon Sword/Shield automation
===============================

This module automates some tasks in Pokémon Brilliant Diamond and Shining
Pearl.

Requirements
------------

You will need an Arduino UNO R3, an external Arduino programmer, and a
pushbutton inserted in the Arduino board, between pins 13 and GND (on the top
row).

You can additionally install a buzzer between pins 2 and GND.

See [the main README](../../README.md#required-hardware) for details.

Installation
------------

Use `make` to build the `usb-iface.hex` and `bdsp.hex` files. Flash
`usb-iface.hex` to the USB interface microcontroller (ATmega16U2), and
`bdsp.hex` to the main microcontroller (ATmega328P).

See the main README for the
[required software](../../README.md#required-software), the
[build procedure](../../README.md#building), and the
[programming procedure](../../README.md#programming).

Usage
-----

Plug the Arduino to the Switch; the L LED on the Arduino board should start
blinking rapidly, and the TX/RX LEDs should be off.

To start the automation process, start Pokémon Brilliant Diamond/Shining Pearl
(if it is not already), and put the game in the required state (which depends
on the task to be automated; see below for details).

Press Home to get to the Switch main menu (the selection should be on
the game icon) and press the pushbutton on the Arduino board. The emulated
controller will get auto-registered as controller 2, and then will access the
controller settings to register as controller 1. It will then get back to the
game, ready to control it.

Once it’s done, the Arduino L LED will blink once per second, and both the
RX and TX LEDs will be lit up. You are in the “main menu”, which allows you
to select which automation feature to perform. Press the pushbutton on the
board once to activate feature 1; twice to activate feature 2; etc.

The different automation features are described below.

### Shiny Shaymin Hunt [Feature 1 — one button press]

**Pre-requisites:** Have a Shaymin encounter ready with cursor over "Battle".

This macro takes into account party pokemon with high friendship. It is
suggested to have your leak pokmon hold a Smoke Ball or have another way of to
guarantee a successful flee.

To end this macro pull down the reset pin.

