/*******************************************************************************
* FILE NAME: ws_io.c
*
* DESCRIPTION:
*
* USAGE:
*
*******************************************************************************/

#include <stdio.h>
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "ws_io.h"

/*
#define MOTOR_TEST
 */

/*******************************************************************************
* FUNCTION NAME: assign_outputs_slow
* PURPOSE:       assign motor speeds to pwm outputs
* CALLED FROM:
* ARGUMENTS:
* RETURNS:       none
*******************************************************************************/
void assign_outputs_slow()
{

  /****************************************************************************
   *
   * This section assigns PWM outputs
   *
   ***************************************************************************/
  pwm01 = 127;
  pwm02 = 127;
  pwm03 = 127;
  pwm04 = 127;
  pwm05 = 127;
  pwm06 = 127;
  pwm07 = 127;
  pwm08 = 127;
  pwm09 = 127;
  pwm10 = 127;
  pwm11 = 127;
  pwm12 = 127;

#ifdef MOTOR_TEST
  if ((Oi_drive_y > (127 - SC_CALIB_STICK_DEADZONE)) &&
      (Oi_drive_y < (127 + SC_CALIB_STICK_DEADZONE)))
  {
    Oi_drive_y = 127;
  }

  /*
  Rc_analog_out_r_drive_1 = Oi_drive_y;
  Rc_analog_out_r_drive_2 = Oi_drive_y;
  Rc_analog_out_l_drive_1 = Oi_drive_y;
  Rc_analog_out_l_drive_2 = Oi_drive_y;
  Rc_analog_out_accumulator = Oi_drive_y;
  Rc_analog_out_loader = Oi_drive_y;
  Rc_analog_out_turret = Oi_drive_y;
  Rc_analog_out_shooter = Oi_drive_y;
  Rc_analog_out_shooter_tilt = Oi_drive_y;
  Rc_analog_out_cam_servo_pan = Oi_drive_y;
  Rc_analog_out_cam_servo_tilt = Oi_drive_y;
  */

  printf(" driveY=%d\r", (int)Oi_drive_y);

#else
  /*
  printf("left: %d right: %d\r", (int)motor_vals.left_drive,
         (int)motor_vals.right_drive);
  */
  /* drive motors */
  Rc_analog_out_l_drive_1 = 127 + (INT16)motor_vals.left_drive;
  Rc_analog_out_l_drive_2 = 127 + (INT16)motor_vals.left_drive;
  Rc_analog_out_r_drive_1 = 127 + (INT16)motor_vals.right_drive;
  Rc_analog_out_r_drive_2 = 127 + (INT16)motor_vals.right_drive;

  /* accumulator motor */
  if (motor_vals.accumulator == ACCUMULATOR_IN_FAST)
  {
    Rc_analog_out_accumulator = ACCUMULATOR_SPEED_FAST_IN;
  }
  else if (motor_vals.accumulator == ACCUMULATOR_OUT_FAST)
  {
    Rc_analog_out_accumulator = ACCUMULATOR_SPEED_FAST_OUT;
  }
  else if (motor_vals.accumulator == ACCUMULATOR_IN_SLOW)
  {
    Rc_analog_out_accumulator = ACCUMULATOR_SPEED_SLOW_IN;
  }
  else if (motor_vals.accumulator == ACCUMULATOR_OUT_SLOW)
  {
    Rc_analog_out_accumulator = ACCUMULATOR_SPEED_SLOW_OUT;
  }
  else
  {
    Rc_analog_out_accumulator = 127;
  }

  /*
  printf("Accumulator: %d\r", Rc_analog_out_accumulator);
  */

  /* Loader motor */
  if (motor_vals.loader == LOADER_UP)
  {
    Rc_analog_out_loader = LOADER_SPEED_UP;
  }
  else if (motor_vals.loader == LOADER_DOWN)
  {
    Rc_analog_out_loader = LOADER_SPEED_DOWN;
  }
  else
  {
    Rc_analog_out_loader = 127;
  }

  /*
  printf("Loader: %d\r", Rc_analog_out_loader);
  */

  /* turret motor */
  Rc_analog_out_turret = 127 + (INT16)motor_vals.turret;
  /*
   printf("Turret: %3d RLS: %d LLS: %d\r", Rc_analog_out_turret,
       Dig_in_turret_sw_r, Dig_in_turret_sw_l);
   */

  /* shooter tilt motor */
  Rc_analog_out_shooter_tilt = 127 + (INT16)motor_vals.shooter_tilt;
  /*
  printf("Tilt: %3d\r", Rc_analog_out_shooter_tilt);
  */

  /* shooter motor */
  Rc_analog_out_shooter = 127 + (INT16)motor_vals.shooter;
  /*
  printf("Shooter: %3d\r", Rc_analog_out_shooter);
  */


  /* Pan & Tilt servos for camera */
  Rc_analog_out_cam_servo_pan = motor_vals.cam_servo_pan;
  Rc_analog_out_cam_servo_tilt = motor_vals.cam_servo_tilt;

  /* Set thumper positions */
  if(motor_vals.thumper == THUMPER_UP)
  {
    Rc_analog_out_thumper = 254;
  }
  else
  {
    Rc_analog_out_thumper = 127;
  }

  /*
  printf("thumper %d\r", Rc_analog_out_thumper);
  */

#endif


  /****************************************************************************
   *
   * This section assigns relay outputs
   *
   ***************************************************************************/
  relay1_fwd = 0;
  relay1_rev = 0;
  relay2_fwd = 0;
  relay2_rev = 0;
  relay3_fwd = 0;
  relay3_rev = 0;
  relay4_fwd = 0;
  relay4_rev = 0;
  relay5_fwd = 0;
  relay5_rev = 0;
  relay6_fwd = 0;
  relay6_rev = 0;
  relay7_fwd = 0;
  relay7_rev = 0;
  relay8_fwd = 0;
  relay8_rev = 0;

#ifdef MOTOR_TEST

#else

  if(motor_vals.shifter_position == SHIFTER_HIGH)
  {
    Rc_relay_shifter_1 = 0;
    Rc_relay_shifter_2 = 0;
  }
  else
  {
    Rc_relay_shifter_1 = 1;
    Rc_relay_shifter_2 = 0;
  }

  if (motor_vals.pump == PUMP_ON)
  {
    Rc_relay_pump_on = 1;
  }
  else
  {
    Rc_relay_pump_on = 0;
  }

  /* Set bumper positions */
  if(motor_vals.bumper_position == BUMPER_OUT)
  {
    Rc_relay_bumper_1 = 1;
    Rc_relay_bumper_2 = 1;
  }
  else if(motor_vals.bumper_position == BUMPER_HALF_OUT)
  {
    Rc_relay_bumper_1 = 1;
    Rc_relay_bumper_2 = 0;
  }
  else
  {
    Rc_relay_bumper_1 = 0;
    Rc_relay_bumper_2 = 0;
  }
  /*
  printf("B1: %d  B2: %d\r", Rc_relay_bumper_1, Rc_relay_bumper_2);
  */

  /* Set ball accelerator */
  if(motor_vals.ball_accel == BALL_ACCEL_FWD)
  {
    Rc_relay_ball_accel_fwd = 1;
    Rc_relay_ball_accel_rev = 0;
  }
  else if(motor_vals.ball_accel == BALL_ACCEL_REV)
  {
    Rc_relay_ball_accel_fwd = 0;
    Rc_relay_ball_accel_rev = 1;
  }
  else
  {
    Rc_relay_ball_accel_fwd = 0;
    Rc_relay_ball_accel_rev = 0;
  }

#endif

  /****************************************************************************
   *
   * This section assigns digital outputs
   *
   ***************************************************************************/
  if (motor_vals.brake_mode == BRAKE_MODE_ON)
  {
    Dig_out_brake_mode = 0;
  }
  else
  {
    Dig_out_brake_mode = 1;
  }

  return;
}


/*******************************************************************************
* FUNCTION NAME: assign_outputs_fast
* PURPOSE:       assign motor speeds to pwm outputs
* CALLED FROM:
* ARGUMENTS:
* RETURNS:       none
*******************************************************************************/
void assign_outputs_fast()
{
  pwm13 = 127;
  pwm14 = 127;
  pwm15 = 127;
  pwm16 = 127;

#ifdef MOTOR_TEST
  if ((Oi_drive_y > (127 - SC_CALIB_STICK_DEADZONE)) &&
      (Oi_drive_y < (127 + SC_CALIB_STICK_DEADZONE)))
  {
    Oi_drive_y = 127;
  }

  /*
  Rc_analog_out_pwm13 = Oi_drive_y;

  printf(" driveY %d\r", (int)Oi_drive_y);
  */

#else


#endif

  return;
}



/*******************************************************************************
* FUNCTION NAME: joystick_scaling
* PURPOSE:       Scale joystick so the x & y ranges are 0-254
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
UINT8 joystick_scaling(UINT8 joystick_val, UINT8 stick_min_val,
                       UINT8 stick_mid_val, UINT8 stick_max_val)
{
  int tmp_val;
  INT16 new_joystick_val = 127;

  /************************************************************
  Since no two joysticks are the same, we want to make sure that
  all joysticks give a consistent 0-254 input to the rest of
  the code.

  This is done by scaling the joystick so that the values map to
  the following range
        Input          Output
     stick_min_val ==>   0
     stick_mid_val ==>  127
     stick_max_val ==>  254

  The scaled joystick value is stored back into joystick_val

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  It is important to make sure that the return value is never
  less than 0 or greater than 254
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ************************************************************/

  /*
  printf("in %d ", (int)joystick_val);
  */

  if (joystick_val > stick_mid_val)
  {
    new_joystick_val = 127 +
                       ((((INT16)joystick_val - (INT16)stick_mid_val) * 127) /
                        ((INT16)stick_max_val - (INT16)stick_mid_val));
  }
  else if (joystick_val < stick_mid_val)
  {
    new_joystick_val = 127 -
                       ((((INT16)stick_mid_val - (INT16)joystick_val) * 127) /
                        ((INT16)stick_mid_val - (INT16)stick_min_val));
  }

  MIN_MAX(new_joystick_val, 0, 254);

  /*
  printf("out %d\r", (int)new_joystick_val);
  */

  return new_joystick_val;
}



void io_print_oi_inputs()
{
  /*
  printf(" Drive x %d y %d ", (int)Oi_drive_x, (int)Oi_drive_y);
  printf("\r");
  */

  /*
  printf("p1 x %3d y %3d wheel %3d aux %3d top %d trig %d aux1 %d aux2 %d\r",
         (int)p1_x, (int)p1_y, (int)p1_wheel, (int)p1_aux,
         (int)p1_sw_top, (int)p1_sw_trig, (int)p1_sw_aux1, (int)p1_sw_aux2);
  printf("p2 x %3d y %3d wheel %3d aux %3d top %d trig %d aux1 %d aux2 %d\r",
         (int)p2_x, (int)p2_y, (int)p2_wheel, (int)p2_aux,
         (int)p2_sw_top, (int)p2_sw_trig, (int)p2_sw_aux1, (int)p2_sw_aux2);
  printf("p3 x %3d y %3d wheel %3d aux %3d top %d trig %d aux1 %d aux2 %d\r",
         (int)p3_x, (int)p3_y, (int)p3_wheel, (int)p3_aux,
         (int)p3_sw_top, (int)p3_sw_trig, (int)p3_sw_aux1, (int)p3_sw_aux2);
  printf("p4 x %3d y %3d wheel %3d aux %3d top %d trig %d aux1 %d aux2 %d\r",
         (int)p4_x, (int)p4_y, (int)p4_wheel, (int)p4_aux,
         (int)p4_sw_top, (int)p4_sw_trig, (int)p4_sw_aux1, (int)p4_sw_aux2);
  */

  return;
}


void io_print_rc_inputs()
{
  /*
  printf("01 %d 02 %d 03 %d 04 %d 05 %d 06 %d 07 %d 08 %d 09 %d\r",
          rc_dig_in01, rc_dig_in02, rc_dig_in03, rc_dig_in04, 
          rc_dig_in05, rc_dig_in06, rc_dig_in07, rc_dig_in08, 
          rc_dig_in09);
  printf("10 %d 11 %d 12 %d 13 %d 14 %d 15 %d 16 %d 17 %d 18 %d\r",
          rc_dig_in10, rc_dig_in11, rc_dig_in12, 
          rc_dig_in13, rc_dig_in14, rc_dig_in15, rc_dig_in16, 
          rc_dig_in17, rc_dig_in18);
  printf("\r");
  */
  return;
}


/*******************************************************************************
* FUNCTION NAME: display_oi_data
* PURPOSE:       Displays data on OI user display when in user mode
* ARGUMENTS:     print_data - data to print
*                type
* RETURNS:       none
*******************************************************************************/
void display_oi_data(UINT8 print_data, DisplayDataType type)
{

  if ((Oi_calibrate < OI_CALIBRATE_ENCODERS) ||
      (Oi_calibrate > OI_CALIBRATE_JOYSTICKS))
  {
    /* in pot/encoder calibration mode, only display pot/encoder data */
    if (type == DISPLAY_DATA_CALIBRATE)
    {
      User_Mode_byte = print_data;
    }
  }
  else
  {
    /* not in calibrate mode */
    if (disabled_mode == ROBOT_DISABLED)
    {
      /* show auto lock-in data */
      if (type == DISPLAY_DATA_AUTO)
      {
        User_Mode_byte = print_data;
      }
    }
    else
    {
      /* show turret feedback data */
      if (type == DISPLAY_DATA_TURRET)
      {
        User_Mode_byte = print_data;
      }
    }
  }

  return;
}


