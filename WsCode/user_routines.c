/*******************************************************************************
* FILE NAME: user_routines.c <FRC VERSION>
*
* DESCRIPTION:
*  This file contains the default mappings of inputs  
*  (like switches, joysticks, and buttons) to outputs on the RC.  
*
* USAGE:
*  You can either modify this file to fit your needs, or remove it from your 
*  project and replace it with a modified copy. 
*
*******************************************************************************/

#include <stdio.h>

#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "eeprom.h"
#include "ws_includes.h"
#include "ws_io.h"
#include "user_routines.h"
#include "serial_ports.h"
#include "ws_calibrate.h"
#include "ws_drive_input.h"
#include "ws_general.h"
#include "ws_turret.h"
#include "ws_cc.h"


/*******************************************************************************
* FUNCTION NAME: User_Initialization
* PURPOSE:       This routine is called first (and only once) in the Main
*                function.  You may modify and add to this function.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void User_Initialization (void)
{
  Set_Number_of_Analog_Channels(SIXTEEN_ANALOG);    /* DO NOT CHANGE! */

  /* FIRST: Set up the I/O pins you want to use as digital INPUTS. */
  digital_io_01 = digital_io_02 = digital_io_03 = INPUT;
  digital_io_05 = digital_io_06 = digital_io_07 = digital_io_08 = INPUT;
  digital_io_09 = INPUT;
  digital_io_11 = digital_io_12 = INPUT;
  digital_io_13 = digital_io_14 = digital_io_15 = digital_io_16 = INPUT;
  digital_io_17 = digital_io_18 = INPUT;

  digital_io_04 = OUTPUT;
  digital_io_10 = OUTPUT;
    /* 
     Note: digital_io_01 = digital_io_02 = ... digital_io_04 = INPUT; 
           is the same as the following:

           digital_io_01 = INPUT;
           digital_io_02 = INPUT;
           ...
           digital_io_04 = INPUT;
    */

  /* SECOND: Set up the I/O pins you want to use as digital OUTPUTS. */

  /* THIRD: Initialize the values on the digital outputs. */

  /* FOURTH: Set your initial PWM values.  Neutral is 127. */
  pwm01 = pwm02 = pwm03 = pwm04 = pwm05 = pwm06 = pwm07 = pwm08 = 127;
  pwm09 = pwm10 = pwm11 = pwm12 = pwm13 = pwm14 = pwm15 = pwm16 = 127;

  /* FIFTH: Set your PWM output types for PWM OUTPUTS 13-16. */
  /*   Choose from these parameters for PWM 13-16 respectively:               */
  /*     IFI_PWM  - Standard IFI PWM output generated with Generate_Pwms(...) */
  /*     USER_CCP - User can use PWM pin as digital I/O or CCP pin.           */
  Setup_PWM_Output_Type(IFI_PWM, IFI_PWM, IFI_PWM, IFI_PWM);

  /* Example: The following would generate a 40KHz PWM with a 50% duty cycle
     on the CCP2 pin:

         CCP2CON = 0x3C;
         PR2 = 0xF9;
         CCPR2L = 0x7F;
         T2CON = 0;
         T2CONbits.TMR2ON = 1;

         Setup_PWM_Output_Type(USER_CCP,IFI_PWM,IFI_PWM,IFI_PWM);
  */

  /* Add any other initialization code here. */

  Init_Serial_Port_One();
  Init_Serial_Port_Two();
  stdout_serial_port = SERIAL_PORT_ONE;

  Putdata(&txdata);             /* DO NOT CHANGE! */

  printf("IFI 2006 User Processor Initialized ...\r");

  User_Proc_Is_Ready(); /* DO NOT CHANGE! - last line of User_Initialization */
}

/*******************************************************************************
* FUNCTION NAME: Process_Data_From_Master_uP
* PURPOSE:       Executes every 26.2ms when it gets new data from the master 
*                microprocessor.
* CALLED FROM:   main.c
* ARGUMENTS:     none
* RETURNS:       void
*******************************************************************************/
void Process_Data_From_Master_uP(void)
{
  UINT8        ret_val;
  DistHdgType  curr_dist_hdg;

  Getdata(&rxdata);   /* Get fresh data from the master microprocessor. */

  io_print_oi_inputs();
  io_print_rc_inputs();


  if (autonomous_mode == AUTO_DISABLED)
  {
    if ((Oi_calibrate < OI_CALIBRATE_ENCODERS) && (user_display_mode == 0))
    {
      /* Calibrate encoders & not user display mode: turn all motors off so
         the robot will NOT move in this state */
      set_motor_vals_off();

      /* flash the lights on the OI */
      if ((rxdata.packet_num % 8) < 4)
      {
        display_oi_data(255, DISPLAY_DATA_CALIBRATE);
      }
      else
      {
        display_oi_data(0, DISPLAY_DATA_CALIBRATE);
      }

      /* debug buttons to talk to CC */
      if (Oi_sw_get_dst_hdg == 1)
      {
        ret_val = cc_get_dist_hdg(&curr_dist_hdg);

        if (ret_val == CC_SUCCESS)
        {
          printf("stat %d h %3d d %3u\r", curr_dist_hdg.status,
                 curr_dist_hdg.hdg, curr_dist_hdg.dist);
        }
      }

      if (Oi_sw_encoder_debug == 1)
      {
        if (g_cc_encoder_ret_val == CC_SUCCESS)
        {
          printf("Left: %04d Right: %04d Orient %03d\r",
                 g_encoder_vals.left, g_encoder_vals.right,
                 g_encoder_vals.orient);
        }
      }

      if ((Oi_sw_center_camera == 1) && (Oi_sw_center_camera_prev == 0))
      {
        cc_set_camera_pos(CC_CMD_SET_CAMERA_CENTER);
      }

      if ((Oi_sw_9090_camera == 1) && (Oi_sw_9090_camera_prev == 0))
      {
        cc_set_camera_pos(CC_CMD_SET_CAMERA_9090);
      }

      if ((Oi_sw_cc_center_n_look == 1) && (Oi_sw_cc_center_n_look_prev == 0))
      {
        ret_val = cc_center_and_look(LOOK_LOW);
        printf("CnL ret: %d\r", ret_val);
      }

      if ((Oi_sw_cc_start_moving == 1) && (Oi_sw_cc_start_moving_prev == 0))
      {
        ret_val = cc_start_moving();
        printf("Move ret: %d\r", ret_val);
      }
    }
    else
    {
      if (Oi_calibrate < OI_CALIBRATE_ENCODERS)
      {
        /* Pot calibration */
        EEPROM_Write_Handler();
        calibrate_pots();
        display_calibration();

        if (EEPROM_Queue_Free_Space() == EEPROM_QUEUE_SIZE)
        {
          /* queue is empty so it's safe to read the calibration values */
          retrieve_calibration();
        }
      }
      else if (Oi_calibrate > OI_CALIBRATE_JOYSTICKS)
      {
        display_calibration();
      }

      /* scale joysticks */
      Oi_drive_y = joystick_scaling(Oi_drive_y, DRIVE_STICK_Y_MIN,
                                    DRIVE_STICK_Y_MIDDLE, DRIVE_STICK_Y_MAX);
      Oi_drive_x = joystick_scaling(Oi_drive_x, DRIVE_STICK_X_MIN,
                                    DRIVE_STICK_X_MIDDLE, DRIVE_STICK_X_MAX);
      Oi_turret_stick = joystick_scaling(Oi_turret_stick,
                                         MANUAL_TURRET_STICK_MIN,
                                         MANUAL_TURRET_STICK_MIDDLE,
                                         MANUAL_TURRET_STICK_MAX);
      Oi_shooter_tilt = joystick_scaling(Oi_shooter_tilt,
                                         MANUAL_SHOOTER_TILT_STICK_MIN,
                                         MANUAL_SHOOTER_TILT_STICK_MIDDLE,
                                         MANUAL_SHOOTER_TILT_STICK_MAX);

      drive_stick_input(TRUE);
      accumulator_control();
      loader_control();
      pump_control();
      thumper_control();
      bumper_control();
      turret_oi_input();
      shooter_oi_input();
      motor_vals.shooter = shooter_speed_feedback();
    }

    if (disabled_mode == ROBOT_DISABLED)
    {
      /* reset motors to off when robot is disabled */
      shooter_set_state(SHOOTER_OFF);
    }

    assign_outputs_fast();
    Generate_Pwms(pwm13,pwm14,pwm15,pwm16);

    assign_outputs_slow();
    Putdata(&txdata);             /* DO NOT CHANGE! */
  }
}


