# Metro M4 SAMD51J19 Basis

This is something I created mainly to learn bare metal programming on the SAMD51J19 but
also with the hopes it can be used in custom boards and with uC of the same family. It may not
grow to anything but down the road it may, who knows.

As the name implies this is intended to be a code basis for this uC family with project I program for myself
and this specific code is written for the SAMD51J19 for the Metro M4 board by adafruit which I'm developing on.

## License

Licensed Apache 2, do whatever as long as you credit me back. That's glossing over it though and
details are in the license file.


## Other Devices

An adafruit 7-segment display is wired directly to the board, https://www.adafruit.com/product/865. In the basis
It's being used as a clock display.

An LED and button are also wired to the board in conjunction with the 7-segment display. The LED lights up on 2 alarms
seperated by 12-hours triggered from the RTC and the button resets the LED until next alarm.

Code for control of those devices can be found in the `devices` folder. The 7-segment display code also serves as a basis
for generic code targetted for the display across the uC family.
