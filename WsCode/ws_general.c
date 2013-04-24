/*******************************************************************************
* FILE NAME: ws_general.c
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
#include "delays.h"       /*defined locally*/
#include "ws_io.h"
#include "ws_general.h"
#include "ws_turret.h"


/*******************************************************************************
* FUNCTION NAME: accumulator_control()
* PURPOSE:       Handles driver input from OI for accumulator
* ARGUMENTS:     OIData
* RETURNS:       none
*
*******************************************************************************/
void accumulator_control()
{
  static UINT8 accumulator_loop_num_in = 0;
  static UINT8 accumulator_loop_num_out = 0;

  if (Oi_accumulator > ACCUMULATOR_SW_IN)
  {
    /* run accumulator in */
    accumulator_loop_num_out = 0;
    if(accumulator_loop_num_in <= ACCUMULATOR_SLOW_LOOPS)
    {
      /* go slow */
      motor_vals.accumulator = ACCUMULATOR_IN_SLOW;
      accumulator_loop_num_in = accumulator_loop_num_in + 1;
    }
    else
    {
      /* go fast for rest */
      motor_vals.accumulator = ACCUMULATOR_IN_FAST;
    }
  }
  else if (Oi_accumulator < ACCUMULATOR_SW_OUT)
  {
    /* run accumulator out */
    accumulator_loop_num_in = 0;
    if(accumulator_loop_num_out <= ACCUMULATOR_SLOW_LOOPS)
    {
      /* go slow */
      motor_vals.accumulator = ACCUMULATOR_OUT_SLOW;
      accumulator_loop_num_out = accumulator_loop_num_out + 1;
    }
    else
    {
      /* go fast for rest */
      motor_vals.accumulator = ACCUMULATOR_OUT_FAST;
    }
  }
  else
  {
    motor_vals.accumulator = ACCUMULATOR_OFF;
    accumulator_loop_num_in = 0;
    accumulator_loop_num_out = 0;
  }

  /*
  printf("acc Oi %d ln in %d out %d s = %d\r", Oi_accumulator,
         accumulator_loop_num_in, accumulator_loop_num_out,
         motor_vals.accumulator);
  */

  return;
}


/*******************************************************************************
* FUNCTION NAME: loader_control()
* PURPOSE:       Handles driver input from OI for loader
* ARGUMENTS:     OIData
* RETURNS:       none
*
*******************************************************************************/
void loader_control()
{
  if (Oi_loader_manu <= LOADER_SW_UP)
  {
    motor_vals.loader = LOADER_UP;
    motor_vals.ball_accel = BALL_ACCEL_FWD;
  }
  else if(Oi_loader_manu >= LOADER_SW_DOWN)
  {
    motor_vals.loader = LOADER_DOWN;
    motor_vals.ball_accel = BALL_ACCEL_REV;
  }
  else
  {
    motor_vals.loader = LOADER_OFF;
    motor_vals.ball_accel = BALL_ACCEL_OFF;
  }

  /*
  printf("ldr speed %d\r", motor_vals.loader);
  */
}

/*******************************************************************************
* FUNCTION NAME: pump_control()
* PURPOSE:       Turns pump on and off based on pressure sensor
* ARGUMENTS:     none
* RETURNS:       none
*
*******************************************************************************/
void pump_control(void)
{
  /* This variable represents the number of cycles the pump is on after the
     switch tells us that we're at 120 PSI */
  static int cycles_after_sensor = 0;

  /* Check to see if pressure switch is off
     This means that we are less than 120 PSI.
     In this case we want the pump to turn on.

     If the pressure switch indicates that we are
     greater than 120PSI, we want to run for PUMP_TOP_OFF
     cycles before it turns off

     Input variables:
     -------------------
     Dig_in_pressure - The variable containing the state of the pressure switch
                       0 = Less than or equal to 120PSI
                       1 = Greater than 120PSI

     competition_mode - The variable containing the state of the robot.  This
                        will have one of the following values.
                        ROBOT_ENABLED
                        ROBOT_DISABLED

     Output variables:
     -------------------
     motor_vals.pump - The output variable that will control
                       the pump.  It can be set to the following
                       values - PUMP_ON, PUMP_OFF
  */

  if (Dig_in_pressure == PRESSURE_BELOW_120)
  {
    /* if pressure sensor is < 120, set counter to 0 */
    cycles_after_sensor = 0;
  }
  else if ((motor_vals.pump == PUMP_ON) && (disabled_mode == ROBOT_ENABLED))
  {
    /* else pump is on & robot is enabled increment counter */
    cycles_after_sensor++;
  }


  if (cycles_after_sensor < 300)
  {
    /* if counter is < 300 turn pump on */
    motor_vals.pump = PUMP_ON;
  }
  else
  {
    /* else turn pump off */
    motor_vals.pump = PUMP_OFF;
  }
}

/*******************************************************************************
* FUNCTION NAME: thumper_control()
* PURPOSE:       Processes thumper control
* ARGUMENTS:     None
* RETURNS:       none
*
*******************************************************************************/
void thumper_control()
{
  static UINT8 thumper_count = 0;

  if(Oi_sw_thumper == 1)
  {
    thumper_count++;
    if(thumper_count >= THUMPER_CYCLE_LOOPS)
    {
      if(motor_vals.thumper == THUMPER_UP)
      {
        motor_vals.thumper = THUMPER_DOWN;
      }
      else
      {
        motor_vals.thumper = THUMPER_UP;
      }
      /* reset counter upon expiration */
      thumper_count = 0;
    }
  }
  else
  {
    motor_vals.thumper = THUMPER_DOWN;
  }

  /*
  printf("Thumper SW: %d CNT %03d MV %03d\r", 
          Oi_sw_thumper, thumper_count, motor_vals.thumper);
    */
}


/*******************************************************************************
* FUNCTION NAME: bumper_control()
* PURPOSE:       Processes bumper control
* ARGUMENTS:     None
* RETURNS:       none
*
*******************************************************************************/
void bumper_control()
{
  /*
  printf("bumper in %d out %d ", Oi_sw_bumper_in, Oi_sw_bumper_out);
  */

  if ((Oi_sw_bumper_in == 1) && (Oi_sw_bumper_in_prev == 0))
  {
    if(motor_vals.bumper_position == BUMPER_HALF_OUT)
    {
      motor_vals.bumper_position = BUMPER_IN;
    }
    else if(motor_vals.bumper_position == BUMPER_OUT)
    {
      motor_vals.bumper_position = BUMPER_HALF_OUT;
    }
  }
  else if ((Oi_sw_bumper_out == 1) && (Oi_sw_bumper_out_prev == 0))
  {
    if(motor_vals.bumper_position == BUMPER_IN)
    {
      motor_vals.bumper_position = BUMPER_HALF_OUT;
    }
    else if(motor_vals.bumper_position == BUMPER_HALF_OUT)
    {
      motor_vals.bumper_position = BUMPER_OUT;
    }
  }


  if(disabled_mode == ROBOT_DISABLED)
  {
    motor_vals.bumper_position = BUMPER_IN;
  }

  /*
  printf("pos %d\r", motor_vals.bumper_position);
  */

}
/*******************************************************************************
* FUNCTION NAME: set_motor_vals_off()
* PURPOSE:       Turns all motors off in motor_vals struct
* ARGUMENTS:     none
* RETURNS:       none
*
*******************************************************************************/
void set_motor_vals_off(void)
{
  motor_vals.left_drive = 0;
  motor_vals.right_drive = 0;
  motor_vals.shifter_position = SHIFTER_HIGH;
  motor_vals.accumulator = ACCUMULATOR_OFF;
  motor_vals.loader = LOADER_OFF;
  motor_vals.ball_accel = BALL_ACCEL_OFF;
  motor_vals.turret = 0;
  motor_vals.shooter_tilt = 0;
  motor_vals.bumper_position = BUMPER_IN;
  motor_vals.thumper = THUMPER_DOWN;
  shooter_set_state(SHOOTER_OFF);
  motor_vals.shooter = shooter_speed_feedback();
  motor_vals.brake_mode = BRAKE_MODE_OFF;
}


