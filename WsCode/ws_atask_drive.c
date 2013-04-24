#include <stdio.h>
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "user_routines.h"
#include "delays.h"       /*defined locally*/
#include "ws_cc.h"
#include "ws_pid.h"
#include "ws_io.h"

#include "ws_autonomous.h"
#include "ws_autonomous_tasks.h"
#include "ws_auto_drive.h"
#include "ws_drive_input.h"

extern PidValsType g_drive_speed_pid_vals;

AutoTaskState auto_task_drive_time(UINT16 time,INT8 l_speed, INT8 r_speed, UINT8 gear)
{
  static DriveTaskState task_state = DRIVE_TASK_INIT;
  static UINT16 drive_counter = 0;
  AutoTaskState ret_state = TASK_STATE_PROCESSING;

  switch (task_state)
  {
    case DRIVE_TASK_INIT:
      drive_counter = 0;
      task_state = DRIVE_TASK_DRIVE;
      ret_state = TASK_STATE_PROCESSING;
      break;

    case DRIVE_TASK_DRIVE:
      drive_counter++;
      motor_vals.left_drive = l_speed;
      motor_vals.right_drive = r_speed;
      motor_vals.shifter_position = gear;
      if(drive_counter >= time)
      {
        task_state = DRIVE_TASK_DONE;
      }
      ret_state = TASK_STATE_PROCESSING;
      break;

    case DRIVE_TASK_DONE:
      motor_vals.left_drive = 0;
      motor_vals.right_drive = 0;
      task_state = DRIVE_TASK_INIT;
      ret_state = TASK_STATE_DONE;
      break;

    default:
      task_state = DRIVE_TASK_INIT;
      ret_state = TASK_STATE_DONE;
      break;
  }
  return ret_state;
}


AutoTaskState auto_task_drive_dist(INT16 target_ticks, INT8 speed, UINT8 gear)
{
  return auto_task_drive_dist_tank(target_ticks, speed, speed, gear);
}

AutoTaskState auto_task_drive_dist_tank(INT16 target_ticks, UINT8 y_stick, UINT8 x_stick, UINT8 shifter_pos)
{
  static DriveTaskState task_state = DRIVE_TASK_INIT;
  static UINT16 drive_counter = 0;
  AutoTaskState ret_state = TASK_STATE_PROCESSING;

  switch (task_state)
  {
    case DRIVE_TASK_INIT:
      drive_counter = 0;
      task_state = DRIVE_TASK_DRIVE;
      ret_state = TASK_STATE_PROCESSING;
      break;

    case DRIVE_TASK_DRIVE:
      if(g_cc_encoder_ret_val == CC_SUCCESS)
      {
        drive_counter += g_encoder_vals.left;
        Oi_drive_y = y_stick;
        Oi_drive_x = x_stick;
        Oi_sw_shifter = shifter_pos;
        Oi_sw_turbo = 1;
        drive_stick_input(FALSE);

        if((y_stick < 127) &&
           (drive_counter <= (target_ticks * (-1))))
        {
          task_state = DRIVE_TASK_DONE;
        }
        else if((y_stick >= 127) &&
                (drive_counter >= target_ticks))
        {
          task_state = DRIVE_TASK_DONE;
        }
        ret_state = TASK_STATE_PROCESSING;
      }
      else
      {
        Oi_drive_y = 127;
        Oi_drive_x = 127;
        Oi_sw_shifter = shifter_pos;
        drive_stick_input(FALSE);
      }
      printf("cur: %3d target: %3d x: %3d y: %3d s: %d t: %d\r", 
              drive_counter, target_ticks, Oi_drive_x, Oi_drive_y,
              Oi_sw_shifter, Oi_sw_turbo);
      break;

    case DRIVE_TASK_DONE:
      printf("DRIVE DONE\r");
      Oi_drive_y = 127;
      Oi_drive_x = 127;
      Oi_sw_shifter = shifter_pos;
      drive_stick_input(FALSE);
      task_state = DRIVE_TASK_INIT;
      ret_state = TASK_STATE_DONE;
      break;

    default:
      task_state = DRIVE_TASK_INIT;
      ret_state = TASK_STATE_DONE;
      break;
  }

  return ret_state;
}

AutoTaskState auto_task_rotate(INT8 angle, INT8 speed)
{
  UINT8 ret = AUTO_PROGRAM_DONE;
  return ret;
}

