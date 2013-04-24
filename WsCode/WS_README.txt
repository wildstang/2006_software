2006 Wildstang Functionality Guide

=-=-=-=-=-=-=-=-=-=-=-=-
Driver
=-=-=-=-=-=-=-=-=-=-=-=-
* Overview
  - Tank drive  - Drive stick X/Y
  - Shifter     - OI shifter switch
  - Turbo       - Drive stick trigger
  - Bumper      - Drive stick R-Top & L-Top
  - Accumulator - OI accumulate button


* Single Stick Tank Drive
  - Drive stick X/Y - Single stick differential drive

* Shifter

* Turbo
  - The drive stick trigger provides momentary control of turbo.  When not
    pressed, the maximum speed that the robot is allowed to drive at is scaled
    down by 2/3.  When the button is pressed, the maximum speed is allowed.
    Anti-turbo is only active in high gear, when in low gear the robot is
    allowed to drive at maximum speed independent of the turbo button state.

* Bumper

* Accumulator

* Dynamic braking
  - This feature will allow the robot to set the drive speed controllers to
    brake mode while driving at slow speeds based on wheel encoder counts.
    This will provide additional resistance within the drivetrain when the
    driver's intention is to not move.

=-=-=-=-=-=-=-=-=-=-=-=-
Manipulator
=-=-=-=-=-=-=-=-=-=-=-=-
* Overview
  - Turret  - Manipulator stick Trigger + X-axis
  - Tilt    - Manipulator stick L-Top + Y-axis
  - Shooter - Manipulator stick hat Up & Down
  - Loader  - OI loader switch
  - Thumper - OI thumper button

* Turret

* Shooter

* Loader

* Thumper


=-=-=-=-=-=-=-=-=-=-=-=-
Common
=-=-=-=-=-=-=-=-=-=-=-=-
* Joystick calibration
  - When the OI debug switch is put in joystick mode, the output of the
    joysticks can be read on the OI 7-seg display when the OI is in user mode
    (press select until the first character is a 'u')  This is useful for
    debugging joystick input
  - To activate for an analog stick, move the stick all the way to either the
    top or left depending on which axis is being looked at.

* Pot calibration
  - When the OI debug switch is put in pot mode, there are two modes of
    operation
    - Debug - Displays the values of various pots on the OI 7-seg display
    - Calibrate - Allows the shooter tilt to be calibrated and the calibration
      values to be stored in EEPROM.
  - Debugging
    - Shooter tilt - Move the manipulator stick all the way to the top.  The
      value displayed at the OI is the current value of the pot.
  - Calibration
    - Shooter tilt
      - There are two calibration values associated with the shooter tilt pot:
        top and bottom.
      - Position the shooter tilt so that it is full up or down.  Move the
        manipulator stick full forward or full backward (respectively) and
        press the manipulator stick trigger.

