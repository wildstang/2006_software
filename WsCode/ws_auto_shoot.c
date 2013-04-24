#include <stdio.h>
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "user_routines.h"
#include "delays.h"       /*defined locally*/
#include "ws_io.h"

#include "ws_autonomous.h"
#include "ws_auto_drive.h"
#include "ws_general.h"
#include "ws_turret.h"
#include "ws_cc.h"
#include "ws_autonomous_tasks.h"
#include "ws_autonomous_prog.h"

#define CHECK_DEAD_ENCODER    1
#define DEAD_ENCODER_TICKS   40
#define DEAD_ENCODER_PAUSE   20

/*******************************************************************************
* FUNCTION NAME: auto_shoot()
* PURPOSE:       General routine to drive to a shooting position
*                and shoot if locked in
* ARGUMENTS:     drive_points - An array of drive points.  The last point must
*                               have a left and right tick of 0
* RETURNS:       none
*
*******************************************************************************/
UINT8 auto_shoot(AutoShootDriveType drive_points[])
{
  static UINT8 prog_state = AUTO_SHOOT_INIT;
  static UINT8 drive_index = 0;
  UINT8 prog_ret = AUTO_PROGRAM_NOT_DONE;
  UINT8 ret;
  UINT8 static pause_counter=0;
  UINT8 static delay_time = 0;
#if CHECK_DEAD_ENCODER
  UINT8 static dead_encoder_tick_count = 0;
#endif

  switch(prog_state)
  {
    case AUTO_SHOOT_INIT:
      // Turn shooter on
      shooter_set_state(SHOOTER_ON);
      if (g_cc_encoder_ret_val == CC_SUCCESS)
      {
        prog_state = AUTO_SHOOT_DELAY;
        pause_counter = 0;
        switch(g_starting_delay)
        {
          case STARTING_DELAY_ONE:
            delay_time = 40;
            break;
          case STARTING_DELAY_TWO:
            delay_time = 80;
            break;
          default:
            delay_time = 0;
            break;
        }
      }
      break;

    case AUTO_SHOOT_DELAY:
      if (pause_counter <= delay_time)
      {
        pause_counter++;
      }
      else
      {
        prog_state = AUTO_SHOOT_DRIVE;
        pause_counter = 0;
      }
      break;

    case AUTO_SHOOT_DRIVE:
      if(drive_points[drive_index].delay > 0)
      {
        if(pause_counter >=  drive_points[drive_index].delay)
        {
          pause_counter = 0;
          drive_index++;
        }
        else
        {
          printf("Drive delay: cur %3d  target  %3d\r",
                 drive_points[drive_index].delay, pause_counter);

          pause_counter++;
        }
        motor_vals.left_drive = 0;
        motor_vals.right_drive = 0;
        motor_vals.shifter_position = drive_points[drive_index].shifter;
      }
      else if(drive_points[drive_index].left_ticks == 0 &&
         drive_points[drive_index].right_ticks == 0)
      {
        /* We are at the last point
           turn the motors off and transition */
        motor_vals.left_drive = 0;
        motor_vals.right_drive = 0;
        motor_vals.shifter_position = drive_points[drive_index].shifter;
        prog_state = AUTO_SHOOT_STOP;
      }
      else
      {
#if CHECK_DEAD_ENCODER
        if(g_encoder_vals.left == 0)
        {
          dead_encoder_tick_count++;
        }
        else
        {
          dead_encoder_tick_count = 0;
        }
#endif
        /* We aren't at the last point in the list, so drive based on them */
        ret = auto_task_drive_dist_tank(drive_points[drive_index].left_ticks,
                                        drive_points[drive_index].y_stick,
                                        drive_points[drive_index].x_stick,
                                        drive_points[drive_index].shifter);
        if(ret == TASK_STATE_DONE)
        {
          /* We reached the point, go to the next */
          drive_index++;
          printf("INDEX: %d\r", drive_index);
        }
#if CHECK_DEAD_ENCODER
        else if(dead_encoder_tick_count == DEAD_ENCODER_TICKS)
        {
          prog_state = AUTO_SHOOT_DEAD_ENCODER;
          pause_counter = 0;
        }
#endif
      }
      break;

    case AUTO_SHOOT_DEAD_ENCODER:

      if (pause_counter >= DEAD_ENCODER_PAUSE)
      {
        prog_state = AUTO_SHOOT_STOP;
      }
      else
      {
        motor_vals.left_drive = 10;
        motor_vals.right_drive = 10;
        pause_counter++;
      }

      break;

    case AUTO_SHOOT_STOP:
      /* Turn the drive motors off */
      printf("STOP ");
      motor_vals.left_drive = 0;
      motor_vals.right_drive = 0;
      motor_vals.shifter_position = drive_points[drive_index].shifter;

      if(shooter_locked(TRUE) == TRUE)
      {
        printf(" SHOOT");
        /* Shooter is locked on, so shoot */
        /* Turn loader on */
        Oi_loader_manu = LOADER_SW_UP;

        /* Turn thumper on */
        Oi_sw_thumper = 1;
      }
      printf("\r");
      break;

    case AUTO_SHOOT_INVALID:
      /* We should never get here, but if we do, shut down */
      printf("INVALID STATE\r");
      set_motor_vals_off();
      shooter_set_state(SHOOTER_OFF);
      break;

    default:
      printf("GETTING TO DEFAULT: %d\r", prog_state);
      prog_state = AUTO_SHOOT_INVALID;

  }

  // Run the shooter feedback routine
  motor_vals.shooter = shooter_speed_feedback();

  // Run the turret feedback
  Oi_sw_turret_auto = 1;
  turret_oi_input();

  // Run the loader and thumper controls
  loader_control();
  thumper_control();

  return prog_ret;
}
