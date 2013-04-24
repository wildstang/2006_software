/*******************************************************************************
* FILE NAME: ws_turret.c
*
* DESCRIPTION: Control the turret and the ball shooter
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
#include "ws_turret.h"
#include "ws_cc.h"
#include "ws_pid.h"
#include "ws_drive_input.h"

#define DEBUG_TURRET_STATE_MACHINE 0
#define TURRET_SWITCH_ACTIVATED 0
#define CALIBRATE_SHOOTER  0

static UINT8 g_turret_leds = 0;
static UINT8 g_turret_locked_in = FALSE;
static UINT8 g_tilt_locked_in = FALSE;
static UINT8 g_shooter_speed_locked_in = FALSE;

static PidValsType turret_pid_vals;
static PidValsType shooter_tilt_pid_vals;
static PidValsType shooter_speed_pid_vals;

static UINT8 turret_feedback_state = FDBK_DISABLED;
static INT8  turret_desired_pos = 0;
static INT8  turret_speed_prev = 0;
static UINT8 shooter_feedback_state = FDBK_DISABLED;
static UINT16 shooter_desired_pos = 0;
static UINT8 shooter_state = SHOOTER_OFF;

static TurretLatchStateType g_turret_latch = LATCH_NONE;

#ifdef PROTO_ROBOT
/* proto */
const rom UINT16 tilt_lookup[] = 
/*          0    1    2    3    4    5    6    7    8    9 */
/* 0 */  { 16,  16,  16,  16,  16,  16,  16,  16,  16,  16, 
/* 1 */    16,  16,  16,  16,  15,  15,  15,  15,  15,  15, 
/* 2 */    15,  15,  15,  15,  15,  15,  16,  16,  16,  16, 
/* 3 */    16,  16,  17,  17,  18,  18,  19,  20,  20,  21, 
/* 4 */    22,  22,  23,  23,  24,  25,  25,  26,  27,  27, 
/* 5 */    27,  27,  27,  27,  27,  27,  27,  27,  27,  27, 
/* 6 */    28,  28,  29,  30,  30,  31,  32,  32,  33,  33, 
/* 7 */    34,  35,  35,  36,  37};
#endif

#ifdef REAL_ROBOT
/* Real robot at Milwaukee */
#if 0
/* Wisconsin numbers */
const rom UINT16 tilt_lookup[] = 
/*          0    1    2    3    4    5    6    7    8    9 */
/* 0 */  { 14,  14,  14,  14,  14,  14,  14,  14,  14,  14, 
/* 1 */    14,  14,  14,  14,  14,  14,  14,  14,  15,  15, 
/* 2 */    15,  16,  16,  16,  17,  17,  17,  18,  18,  19, 
/* 3 */    19,  19,  20,  20,  21,  21,  22,  22,  23,  23, 
/* 4 */    24,  24,  25,  25,  26,  26,  26,  26,  26,  27, 
/* 5 */    27,  27,  27,  28,  28,  29,  30,  30,  31,  32, 
/* 6 */    32,  33,  34,  34,  35,  36,  36,  37,  38,  38, 
/* 7 */    39,  40,  41};
#endif
#if 0
/* truncating */
const rom UINT16 tilt_lookup[] = 
/*          0    1    2    3    4    5    6    7    8    9 */
/* 0 */  { 13,  13,  13,  13,  13,  13,  13,  13,  13,  13, 
/* 1 */    13,  14,  15,  16,  17,  19,  20,  21,  22,  24, 
/* 2 */    24,  24,  25,  25,  26,  26,  26,  27,  27,  28, 
/* 3 */    28,  28,  29,  29,  30,  30,  31,  32,  33,  35, 
/* 4 */    35,  35,  36,  36,  37,  37,  38,  38,  39,  39, 
/* 5 */    40,  40,  41,  41,  41,  42,  42,  43,  43,  44, 
/* 6 */    44,  45,  45,  46,  46,  47};
#endif

/* rounding */
const rom UINT16 tilt_lookup[] = 
/*          0    1    2    3    4    5    6    7    8    9 */
/* 0 */  { 13,  13,  13,  13,  13,  13,  13,  13,  13,  13, 
/* 1 */    13,  14,  15,  17,  18,  19,  20,  22,  23,  24, 
/* 2 */    24,  25,  25,  26,  26,  26,  27,  27,  28,  28, 
/* 3 */    29,  29,  29,  30,  30,  31,  31,  33,  35,  37, 
/* 4 */    37,  38,  38,  39,  39,  39,  40,  40,  40,  41, 
/* 5 */    41,  42,  42,  42,  43,  43,  44,  44,  44,  45, 
/* 6 */    45,  45,  46,  46,  47,  47};


#endif



const rom INT8 turret_speed_lookup[] =
{  0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 
   0,   0,   1,   1,   1,   1,   2,   2,   2,   2, 
   3,   3,   3,   4,   4,   4,   5,   5,   6,   6, 
   7,   7,   8,   8,   9,   9,  10,  10,  11,  11, 
  12,  13,  13,  14,  15,  15,  16,  17,  18,  18, 
  19,  20,  21,  22,  22,  23,  24,  25,  26,  27, 
  28,  29,  30,  31,  32,  33,  34,  35,  36,  37, 
  38,  39,  40,  41,  43,  44,  45,  46,  47,  49, 
  50,  51,  52,  54,  55,  56,  58,  59,  60,  62, 
  63,  65,  66,  68,  69,  71,  72,  74,  75,  77, 
  78,  80,  81,  83,  85,  86,  88,  90,  91,  93, 
  95,  97,  98, 100, 102, 104, 105, 107, 109, 111, 
 113, 115, 117, 119, 121, 123, 125, 127};



/*******************************************************************************
* FUNCTION NAME: turret_init()
* PURPOSE:       Initializes PID constants
* ARGUMENTS:     none
* RETURNS:       none
* DOL:
*******************************************************************************/
void turret_init()
{
// Dave's tuning
#if 0
  turret_pid_vals.scale_factor = 1000;
  turret_pid_vals.prop_gain = 750;
  turret_pid_vals.int_gain = 0;
  turret_pid_vals.deriv_gain = -1600;
  turret_pid_vals.max_integral = 1000;
  turret_pid_vals.min_val = -127;
  turret_pid_vals.max_val = 127;
  pid_last_error_init(&turret_pid_vals);
#endif


#ifdef SHIPPED_TUNING
  turret_pid_vals.scale_factor = 1000;
  turret_pid_vals.prop_gain = 350;
  turret_pid_vals.int_gain = 30;
  turret_pid_vals.deriv_gain = -100;
  turret_pid_vals.max_integral = 250;
  turret_pid_vals.min_val = -127;
  turret_pid_vals.max_val = 127;
  pid_last_error_init(&turret_pid_vals);
#endif

  turret_pid_vals.scale_factor = 1000;
  turret_pid_vals.prop_gain = 2000;
  turret_pid_vals.int_gain = 250;
  turret_pid_vals.deriv_gain = 100;
  turret_pid_vals.max_integral = 50 * turret_pid_vals.scale_factor;
  turret_pid_vals.min_val = -127;
  turret_pid_vals.max_val = 127;
  pid_last_error_init(&turret_pid_vals);

/* numbers for division by error in deriv term */
#if 0
  turret_pid_vals.scale_factor = 1000;
  // Ultimate gain == 120
  // Ultimate period == 14/40
  turret_pid_vals.prop_gain = 1200;
  turret_pid_vals.int_gain = 0;
  turret_pid_vals.deriv_gain = 3000;
  // turret_pid_vals.int_gain = 571;
  // turret_pid_vals.deriv_gain = -4;
  turret_pid_vals.max_integral = 200;
  turret_pid_vals.min_val = -127;
  turret_pid_vals.max_val = 127;
  pid_last_error_init(&turret_pid_vals);
#endif


}


/*******************************************************************************
* FUNCTION NAME: shooter_init()
* PURPOSE:       Initializes PID constants
* ARGUMENTS:     none
* RETURNS:       none
* DOL:
*******************************************************************************/
void shooter_init()
{
#ifdef ORIG_VALUES
  /* PID vals for shooter tilt */
  shooter_tilt_pid_vals.scale_factor = 10;
  shooter_tilt_pid_vals.prop_gain = 8 * shooter_tilt_pid_vals.scale_factor;
  shooter_tilt_pid_vals.int_gain = 4;
  shooter_tilt_pid_vals.deriv_gain = -170;
  shooter_tilt_pid_vals.max_integral = 60;
  shooter_tilt_pid_vals.min_val = -127;
  shooter_tilt_pid_vals.max_val = 127;
  pid_last_error_init(&shooter_tilt_pid_vals);
#endif
  /* PID vals for shooter tilt */
  shooter_tilt_pid_vals.scale_factor = 100;
  shooter_tilt_pid_vals.prop_gain = 3 * shooter_tilt_pid_vals.scale_factor;
  shooter_tilt_pid_vals.int_gain = 0;
//shooter_tilt_pid_vals.int_gain = 50;
  shooter_tilt_pid_vals.deriv_gain = -0;
  shooter_tilt_pid_vals.max_integral = 60 * shooter_tilt_pid_vals.scale_factor;
  shooter_tilt_pid_vals.min_val = -127;
  shooter_tilt_pid_vals.max_val = 127;
  pid_last_error_init(&shooter_tilt_pid_vals);


  /* PID vals for shooter speed */
  shooter_speed_pid_vals.scale_factor = 20;
  shooter_speed_pid_vals.prop_gain = 1 * shooter_speed_pid_vals.scale_factor;
  shooter_speed_pid_vals.int_gain = 1;
  shooter_speed_pid_vals.deriv_gain = 0;
  shooter_speed_pid_vals.max_integral = 127 * shooter_speed_pid_vals.scale_factor;
  shooter_speed_pid_vals.min_val = -127;
  shooter_speed_pid_vals.max_val = 127;
  pid_last_error_init(&shooter_speed_pid_vals);
}


/*******************************************************************************
* FUNCTION NAME: turret_oi_input()
* PURPOSE:       Handles driver input from OI for turret
* ARGUMENTS:     OIData
* RETURNS:       none
*
*******************************************************************************/
void turret_oi_input(void)
{
  static TurretStateType auto_turret_state = TURRET_MANUAL;
  static TurretStateType auto_turret_state_prev = TURRET_INVALID;
  static UINT8 cam_centered = FALSE;
  static UINT8 cam_moving = FALSE;
  static UINT8 lost_loop_count;
#if CALIBRATE_SHOOTER
  UINT16  tilt_pot = 0;
#endif

  TurretStateType auto_turret_state_next = auto_turret_state;
  DistHdgType curr_dist_hdg;
  UINT8 cc_ret_status;
  UINT16 tilt_pos;

#if CALIBRATE_SHOOTER
  if(cam_centered == FALSE && cam_moving == FALSE)
  {
    cc_ret_status = cc_center_and_look(LOOK_LOW);
    if(cc_ret_status == CC_SUCCESS)
    {
      printf("LOOK!!!\r");
      cam_centered = TRUE;
      cam_moving = FALSE;
    }
  }
  else
  {
    if(cam_moving == FALSE)
    {
      cc_ret_status =  cc_start_moving();
      if(cc_ret_status == CC_SUCCESS)
      {
        printf("MOVE!!!\r");
        cam_moving = TRUE;
        cam_centered = FALSE;
      }
    }
  }

  if(cam_moving == TRUE)
  {
    cc_ret_status = cc_get_dist_hdg(&curr_dist_hdg);
    if(cc_ret_status == CC_SUCCESS)
    {
      if(curr_dist_hdg.valid_data == CC_VALID_DATA)
      {
        tilt_pot = get_shooter_tilt_pot();
        printf(" Cam: %d Tilt: %d\r", curr_dist_hdg.dist, tilt_pot);

        set_led_state(LED_OFF, TURRET_AUTO_LED_MASK);
        set_led_state(LED_OFF, TURRET_FIRE_READY_LED_MASK);

        if(curr_dist_hdg.status == STATE_SEE_GREEN)
        {
          set_led_state(LED_ON, TURRET_SEE_GREEN_LED_MASK);
        }
        else
        {
          set_led_state(LED_OFF, TURRET_SEE_GREEN_LED_MASK);
        }
      }
    }

    display_turret_leds();
    process_turret_sensors();
    turret_manual_control();
    shooter_manual_control();

  }
  return;
#endif // END SHOOTER CALIBRATION


#if 0
  if(Oi_sw_turbo == 1 && Oi_sw_turbo_prev == 0)
  {
    turret_pid_vals.prop_gain += 1;
  }
  if(p4_sw_top == 1 && p4_sw_top_prev == 0)
  {
    turret_pid_vals.prop_gain -= 1;
  }
#endif
  //printf("P: %d ", (int) turret_pid_vals.prop_gain);



  process_turret_sensors();

  switch(auto_turret_state)
  {
    case TURRET_MANUAL:
#if DEBUG_TURRET_STATE_MACHINE
      printf("State-Manual ");
#endif
      /* First time through
       *    - Center camera (also tells cc to look for green)
       */
      if ((cam_centered == FALSE) ||
          ((auto_turret_state_prev != auto_turret_state) &&
           (auto_turret_state_prev != TURRET_MANUAL_LOCK)) ||
          (Oi_sw_camera_up != Oi_sw_camera_up_prev))
      {
#if DEBUG_TURRET_STATE_MACHINE
        /*
        printf("Call cc_center_and_look()");
        */
#endif
        if (Oi_sw_camera_up == 1)
        {
          cc_ret_status = cc_center_and_look(LOOK_HIGH);
        }
        else
        {
          cc_ret_status = cc_center_and_look(LOOK_LOW);
        }

        if(cc_ret_status == CC_SUCCESS)
        {
          cam_centered = TRUE;
          cam_moving = FALSE;
        }
        else
        {
          cam_centered = FALSE;
        }

        turret_set_pos(FDBK_DISABLED, 0);
        shooter_set_pos(FDBK_DISABLED, 0);
      }

      if(cam_centered == TRUE)
      {
        /* Request dist/hdg */
        cc_ret_status = cc_get_dist_hdg(&curr_dist_hdg);

        if(cc_ret_status == CC_SUCCESS)
        {
#if DEBUG_TURRET_STATE_MACHINE
          printf("s %d h %3d d %3u p %3d t %3d", curr_dist_hdg.status,
                 curr_dist_hdg.hdg, curr_dist_hdg.dist, 
                 curr_dist_hdg.pan_pwm, curr_dist_hdg.tilt_pwm);
#endif
          /* Found green *
           * Transition to TURRET_MANUAL_LOCK*/
          if(curr_dist_hdg.status ==  STATE_SEE_GREEN)
          {
            auto_turret_state_next = TURRET_MANUAL_LOCK;
          }
        }
      }
      /* Process manual joystick control */
      turret_manual_control();
      shooter_manual_control();

      /* Set OI LEDs */
      set_led_state(LED_OFF, TURRET_SEE_GREEN_LED_MASK);
      set_led_state(LED_OFF, TURRET_AUTO_LED_MASK);
      set_led_state(LED_OFF, TURRET_FIRE_READY_LED_MASK);

      break;

    case TURRET_MANUAL_LOCK:
#if DEBUG_TURRET_STATE_MACHINE
      printf("State-ManualLock ");
#endif


      /* need to toggle the camera tilt when it sees green as well */
      if (Oi_sw_camera_up != Oi_sw_camera_up_prev)
      {
        if (Oi_sw_camera_up == 1)
        {
          cc_ret_status = cc_center_and_look(LOOK_HIGH);
        }
        else
        {
          cc_ret_status = cc_center_and_look(LOOK_LOW);
        }
      }

      /* Request dist/hdg */
      cc_ret_status = cc_get_dist_hdg(&curr_dist_hdg);

      if(cc_ret_status == CC_SUCCESS)
      {
#if DEBUG_TURRET_STATE_MACHINE
         printf("s %d h %3d d %3u p %3d t %3d", curr_dist_hdg.status,
                curr_dist_hdg.hdg, curr_dist_hdg.dist, 
                curr_dist_hdg.pan_pwm, curr_dist_hdg.tilt_pwm);
#endif
         /* Didn't find green
          * Transition to TURRET_MANUAL
          */
         if ((curr_dist_hdg.status == STATE_NO_GREEN) ||
             (curr_dist_hdg.status == STATE_NO_GREEN_LOST_LEFT) ||
             (curr_dist_hdg.status == STATE_NO_GREEN_LOST_RIGHT))
         {
           auto_turret_state_next = TURRET_MANUAL;
         }
      }

      /* Button Tapped
       * Transition to TURRET_AUTO
       */
      if (Oi_sw_turret_auto == 1)
      {
        if(disabled_mode == ROBOT_ENABLED)
        {
         /* Transition to TURRET_AUTO */
          auto_turret_state_next = TURRET_AUTO;
        }
      }

      /* Process manual joystick control */
      turret_manual_control();
      shooter_manual_control();

      /* Set OI LEDs */
      set_led_state(LED_ON, TURRET_SEE_GREEN_LED_MASK);
      set_led_state(LED_OFF, TURRET_AUTO_LED_MASK);
      set_led_state(LED_OFF, TURRET_FIRE_READY_LED_MASK);

      break;

    case TURRET_AUTO:
#if DEBUG_TURRET_STATE_MACHINE
      printf("State-Auto ");
#endif
      /* First time through
       *    - Allow camera to start moving
       */
      if ((cam_moving == FALSE) ||
          ((auto_turret_state_prev != auto_turret_state) &&
           (auto_turret_state_prev != TURRET_AUTO_LOST)))
      {
#if DEBUG_TURRET_STATE_MACHINE
        /*
         printf("Call cc_start_moving()\r");
         */
#endif
        cc_ret_status =  cc_start_moving();
        if(cc_ret_status == CC_SUCCESS)
        {
          cam_moving = TRUE;
          cam_centered = FALSE;
        }
        else
        {
          cam_moving = FALSE;
        }
      }

      /* Request dist/hdg */
      cc_ret_status = cc_get_dist_hdg(&curr_dist_hdg);

      if(cc_ret_status == CC_SUCCESS)
      {
#if DEBUG_TURRET_STATE_MACHINE
         printf("s %d h %3d d %3u  ", curr_dist_hdg.status, curr_dist_hdg.hdg,
                curr_dist_hdg.dist);
#endif
         if(curr_dist_hdg.status ==  STATE_SEE_GREEN)
         {
          /************
           * FOUND GREEN
           ***********/
           /* Turret feedback */
           turret_set_pos(FDBK_ENABLED, curr_dist_hdg.hdg);
           motor_vals.turret = turret_pos_feedback();

           if(curr_dist_hdg.valid_data == CC_VALID_DATA)
           {
             /* Shooter angle feedback */
             tilt_pos = lookup_shooter_tilt(curr_dist_hdg.dist);
             shooter_set_pos(FDBK_ENABLED, tilt_pos);
             motor_vals.shooter_tilt = shooter_pos_feedback();
             /*
             */
           }
         }
         else
         {
           /*
            * Didn't find green
            *     Transition to TURRET_AUTO_LOST */
           auto_turret_state_next = TURRET_AUTO_LOST;
         }
      }

      /* Set OI LEDs */
      set_led_state(LED_ON, TURRET_SEE_GREEN_LED_MASK);
      set_led_state(LED_ON, TURRET_AUTO_LED_MASK);

      if (shooter_locked(FALSE) == TRUE)
      {
        set_led_state(LED_ON, TURRET_FIRE_READY_LED_MASK);
      }
      else
      {
        set_led_state(LED_OFF, TURRET_FIRE_READY_LED_MASK);
      }

      if (((Oi_sw_turret_manu_move == 1) ||
           (Oi_sw_shooter_tilt_manu_move == 1)) &&
          (Oi_sw_turret_auto == 0))
      {
       /* Transition to TURRET_MANUAL if either manual move button is pressed,
          but not if the auto enable button is pressed */
        auto_turret_state_next = TURRET_MANUAL;
      }

      if(disabled_mode == ROBOT_DISABLED)
      {
       /* Transition to TURRET_MANUAL */
        auto_turret_state_next = TURRET_MANUAL;
      }

      break;

    case TURRET_AUTO_LOST:
#if DEBUG_TURRET_STATE_MACHINE
      printf("State-AutoLost ");
#endif
      /* First time through
       *      Initialize loop counter
       */
      if(auto_turret_state_prev != auto_turret_state)
      {
        lost_loop_count = 0;
      }

      /* Request dist/hdg */
      cc_ret_status = cc_get_dist_hdg(&curr_dist_hdg);

      if (cc_ret_status != CC_SUCCESS)
      {
        /* if CC times-out, set green state to 'no green' */
        curr_dist_hdg.status = STATE_NO_GREEN;
      }

#if DEBUG_TURRET_STATE_MACHINE
      /*
      */
      printf("cnt %d  s %d  ", lost_loop_count, curr_dist_hdg.status);
#endif

      /* Set OI LEDs */
      set_led_state(LED_OFF, TURRET_SEE_GREEN_LED_MASK);
      set_led_state(LED_ON, TURRET_AUTO_LED_MASK);
      set_led_state(LED_OFF, TURRET_FIRE_READY_LED_MASK);

      if (curr_dist_hdg.status == STATE_SEE_GREEN)
      {
        /* Found green, transition to TURRET_AUTO */
        auto_turret_state_next = TURRET_AUTO;
      }
      else if (curr_dist_hdg.status == STATE_NO_GREEN)
      {
        /* lost green but unsure of which direction, just stop */
        /* Turret Motor Off */
        motor_vals.turret = 0;
        /* Turn turret feedback Off */
        turret_set_pos(FDBK_DISABLED, 0);
      }
      else if (curr_dist_hdg.status == STATE_NO_GREEN_LOST_LEFT)
      {
        /* lost green to the left, run turret left */
        if (g_turret_latch == LATCH_NONE)
        {
          motor_vals.turret = 127;
        }
        else
        {
          motor_vals.turret = 0;
        }
      }
      else if (curr_dist_hdg.status == STATE_NO_GREEN_LOST_RIGHT)
      {
        /* lost green to the right, run turret right */
        if (g_turret_latch == LATCH_NONE)
        {
          motor_vals.turret = -127;
        }
        else
        {
          motor_vals.turret = 0;
        }
      }

      /* Shooter Aim Motor Off */
      motor_vals.shooter_tilt = 0;

      /* Turn tilt feedback Off */
      shooter_set_pos(FDBK_DISABLED, 0);

      /* Increment loop counter */
      lost_loop_count = lost_loop_count + 1;

      if (((Oi_sw_turret_manu_move == 1) ||
           (Oi_sw_shooter_tilt_manu_move == 1)) &&
          (Oi_sw_turret_auto == 0))
      {
       /* Transition to TURRET_MANUAL if either manual move button is pressed,
          but not if the auto enable button is pressed */
        auto_turret_state_next = TURRET_MANUAL;
      }

#if USE_TURRET_LOST_LOOP_LIMIT
      if(lost_loop_count >= TURRET_LOST_LOOP_LIMIT)
      {
       /* Transition to TURRET_MANUAL */
        auto_turret_state_next = TURRET_MANUAL;
      }
#endif

      if(disabled_mode == ROBOT_DISABLED)
      {
       /* Transition to TURRET_MANUAL */
        auto_turret_state_next = TURRET_MANUAL;
      }

      break;

    default:
      /* Transition to TURRET_MANUAL */
      auto_turret_state_next = TURRET_MANUAL;
      break;
  }

#if DEBUG_TURRET_STATE_MACHINE
  printf("\r");
#endif

  display_turret_leds();

  /* Save off current state as previous */
  auto_turret_state_prev = auto_turret_state;
  /* Save off next state */
  auto_turret_state = auto_turret_state_next;
#if TURRET_PID_TUNE_CODE
    turret_pid_tune();
#endif


}

/*******************************************************************************
* FUNCTION NAME: process_turret_sensors
* PURPOSE:       Looks at the sensors on the turret to determine
*                if we're at the limit
* ARGUMENTS:     none
* RETURNS:       none
*
*******************************************************************************/
void process_turret_sensors()
{
  /* We want to latch the first sensor to become activated
   * The latched sensor is stored in global variable */


  if(g_turret_latch == LATCH_NONE)
  {
    if((Dig_in_turret_sw_l == TURRET_SWITCH_ACTIVATED) &&
       (Dig_in_turret_sw_r == TURRET_SWITCH_ACTIVATED))
    {
      g_turret_latch = LATCH_NONE;
    }
    else if(Dig_in_turret_sw_l == TURRET_SWITCH_ACTIVATED)
    {
      g_turret_latch = LATCH_LEFT;
    }
    else if(Dig_in_turret_sw_r == TURRET_SWITCH_ACTIVATED)
    {
      g_turret_latch = LATCH_RIGHT;
    }
  }
  /* We unlatch when neitherof the switches is activated */
  else if((Dig_in_turret_sw_r != TURRET_SWITCH_ACTIVATED) &&
          (Dig_in_turret_sw_l != TURRET_SWITCH_ACTIVATED))
  {
    g_turret_latch = LATCH_NONE;
  }
}

#if TURRET_PID_TUNE_CODE
INT8 turret_pid_tune()
{
  static INT8 turret_speed = 0;

  /*******************************************
  *  Use this to determine the minimum speed
  *  that it takes to move the turret
  ********************************************/
#if 0
  if(Oi_sw_turret_manu_move == 1 &&
     Oi_sw_turret_manu_move_prev == 0)
  {
    if(Oi_turret_stick < 87)
    {
      turret_speed--;
    }
    else if(Oi_turret_stick > 167)
    {
      turret_speed++;
    }
  }
#endif


#if 0
  /****************************************
   * Use this to determine if the turret will
   * move at a given speed
   ****************************************/
  if(Oi_sw_turret_manu_move == 1)
  {
    if(Oi_turret_stick < 87)
    {
      turret_speed = -8;
    }
    else if(Oi_turret_stick > 167)
    {
      turret_speed = 8;
    }
  }
  else
  {
    turret_speed = 0;
  }
#endif


  MIN(turret_speed, -127);
  MAX(turret_speed, 127);

  motor_vals.turret = turret_speed;
  printf("TUNE: TURRET SPEED %03d\r", turret_speed);

}
#endif





/*******************************************************************************
* FUNCTION NAME: turret_manual_control()
* PURPOSE:       Handles driver input from OI for turret
* ARGUMENTS:     OIData
* RETURNS:       none
*
*******************************************************************************/
INT8 turret_manual_control()
{
  /* create deadzone in manual turret stick */
  DEADZONE(Oi_turret_stick, MANUAL_TURRET_DEADZONE);

  if(Oi_sw_turret_manu_move == 1)
  {
#if 1
    /* Assign the value of the manual turret stick to turret_speed */
    if (Oi_turret_stick >= 127)
    {
      motor_vals.turret = turret_speed_lookup[Oi_turret_stick - 127];

      /* slow turret down by 50% when at light sensor limit */
      if(g_turret_latch == LATCH_LEFT)
      {
        motor_vals.turret /= 2;
      }
    }
    else
    {
      motor_vals.turret = 0 - turret_speed_lookup[127 - Oi_turret_stick];

      /* slow turret down by 50% when at light sensor limit */
      if(g_turret_latch == LATCH_RIGHT)
      {
        motor_vals.turret /= 2;
      }
    }

#else
    /* Assign the value of the manual turret stick to turret_speed */
    motor_vals.turret = Oi_turret_stick - 127;
#endif
  }
  else
  {
    motor_vals.turret = 0;
  }

#if DEBUG_TURRET_STATE_MACHINE
  printf("  Turret manu J %d L %d SL %d SR %d M %d ",
         Oi_turret_stick, g_turret_latch, 
         Dig_in_turret_sw_l, Dig_in_turret_sw_r,
         motor_vals.turret);
  /*
  */
#endif
}


/*******************************************************************************
* FUNCTION NAME: turret_set_pos()
* PURPOSE:       set heading of camera relative to the turret
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
void turret_set_pos(UINT8 feedback_state, INT8 cam_hdg)
{
  turret_feedback_state = feedback_state;
  turret_desired_pos = cam_hdg;
}


/*******************************************************************************
* FUNCTION NAME: turret_pos_feedback()
* PURPOSE:       feedback loop for turret position
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
INT8 turret_pos_feedback(void)
{
  INT16 turret_speed = 0;
  INT16 kd;
  INT8 lock_window;

  if (turret_feedback_state == FDBK_ENABLED)
  {

    if (disabled_mode == ROBOT_ENABLED)
    {
#if 1
      if((g_turret_latch == LATCH_RIGHT) &&
         (turret_desired_pos > TURRET_CENTER))
      {
        turret_speed = 0;
      }
      else if((g_turret_latch == LATCH_LEFT) &&
              (turret_desired_pos < TURRET_CENTER))
      {
        turret_speed = 0;
      }
      else
      {
        show_pid = 0;

        if((TURRET_CENTER - turret_desired_pos) != turret_pid_vals.last_error[0])
        {
            turret_pid_vals.integral = 0;
        }
        show_pid = 0;
        turret_speed = ws_pid(&turret_pid_vals, turret_desired_pos,
                              TURRET_CENTER);

        /*
        printf("error %d speed %d\r", turret_pid_vals.last_error[0],
               turret_speed);
        */
        if (autonomous_mode == AUTO_ENABLED)
        {
          lock_window = 6;
        }
        else
        {
          lock_window = 2;
        }
        if ((turret_pid_vals.last_error[0] <= lock_window) &&
            (turret_pid_vals.last_error[0] >= (0 - lock_window)) &&
            (turret_speed <= 20))
        {
          g_turret_locked_in = TRUE;
        }
        else
        {
          g_turret_locked_in = FALSE;
        }
      }
#else
      show_pid = 0;
      //kd = 0 - (INT16) p4_wheel;
      turret_speed = ws_pid(&turret_pid_vals, turret_desired_pos,
                            TURRET_CENTER);
      if(turret_speed != turret_speed_prev)
      {
        //turret_pid_vals->integral = 0;
      }
#endif
      turret_speed_prev = turret_speed;
    }
  }

#if DEBUG_TURRET_STATE_MACHINE
  printf("Turret auto %d", turret_speed);
  /*
  */
#endif

  return (INT8)turret_speed;
}


/*******************************************************************************
* FUNCTION NAME: shooter_manual_control()
* PURPOSE:       Handles driver input from OI for shooter
* ARGUMENTS:     OIData
* RETURNS:       none
*
*******************************************************************************/
void shooter_manual_control()
{
  UINT16  tilt_pot = 0;

  /* create deadzone in manual shooter tilt stick */
  DEADZONE(Oi_shooter_tilt, MANUAL_SHOOTER_TILT_DEADZONE);
  tilt_pot = get_shooter_tilt_pot();

  if(Oi_sw_shooter_tilt_manu_move == 1)
  {
    if (Oi_calibrate < OI_CALIBRATE_ENCODERS)
    {
      /* in pot calibrate mode, don't enforce limits */
      motor_vals.shooter_tilt = 127 - Oi_shooter_tilt;
    }
    else
    {
      /* non-pot calibrate mode, enforce limits */
      if((tilt_pot <= calibration_vals.shooter_top) && (Oi_shooter_tilt > 127))
      {
        motor_vals.shooter_tilt = 0;
      }
      else if((tilt_pot >= calibration_vals.shooter_bot) && (Oi_shooter_tilt < 127))
      {
        motor_vals.shooter_tilt = 0;
      }
      else
      {
        /* Assign the value of joystick to motor_vals.shooter_tilt*/
        motor_vals.shooter_tilt = 127 - Oi_shooter_tilt;
      }
    }
  }
  else
  {
    motor_vals.shooter_tilt = 0;
  }

#if 0
  if (Oi_sw_turbo == 1)
  {
    shooter_set_pos(FDBK_ENABLED, 451);
    motor_vals.shooter_tilt = shooter_pos_feedback();
  }
  else
  {
#endif
  /*
    printf("Shooter manu %d", motor_vals.shooter_tilt);
    printf(" pos %d\r", tilt_pot);
  */
#if 0
  }
#endif


  return;
}


/*******************************************************************************
* FUNCTION NAME: shooter_set_pos()
* PURPOSE:       set desired shooter position for feedback routine
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
void shooter_set_pos(UINT8 feedback_state, UINT16 desired_pos)
{
  shooter_feedback_state = feedback_state;
  shooter_desired_pos = desired_pos;
  /*
  printf("state %d pos %d ", shooter_feedback_state, shooter_desired_pos);
  */
}


/*******************************************************************************
* FUNCTION NAME: shooter_pos_feedback()
* PURPOSE:       feedback loop for shooter position
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
INT8 shooter_pos_feedback(void)
{
  INT16  shooter_pos_speed = 0;
  INT16  shooter_pos_speed_prev = 0;
  UINT16  shooter_pos;
  INT8  lock_window;

  if (shooter_feedback_state == FDBK_ENABLED)
  {
    /* get current shooter position from pot */
    shooter_pos = 0;

    if (disabled_mode == ROBOT_ENABLED)
    {
      shooter_pos = get_shooter_tilt_pot();

      /*
      printf("tgt %d curr %d ", shooter_desired_pos, shooter_pos);
      */

      if((shooter_pos <= calibration_vals.shooter_top) &&
         (shooter_desired_pos < calibration_vals.shooter_top))
      {
        /*
        printf(" TOP %d %d %d\r", shooter_pos, 
                                  shooter_desired_pos, 
                                  calibration_vals.shooter_top);
        */
        motor_vals.shooter_tilt = 0;
      }
      else if((shooter_pos >= calibration_vals.shooter_bot) &&
              (shooter_desired_pos > calibration_vals.shooter_bot))
      {
        /*
        printf(" BOT %d %d %d\r", shooter_pos, 
                                  shooter_desired_pos, 
                                  calibration_vals.shooter_bot);
        */
        motor_vals.shooter_tilt = 0;
      }
      else
      {
        show_pid = 0;
        shooter_pos_speed = ws_pid(&shooter_tilt_pid_vals, shooter_pos,
                               shooter_desired_pos);
        if(shooter_pos == shooter_desired_pos)
        {
          shooter_tilt_pid_vals.integral = 0;
        }

        /*
        printf("error %d speed %d\r", turret_pid_vals.last_error[0],
               shooter_pos_speed);
        */
        if (autonomous_mode == AUTO_ENABLED)
        {
          lock_window = 6;
        }
        else
        {
          lock_window = 2;
        }
        if ((shooter_tilt_pid_vals.last_error[0] <= lock_window) &&
            (shooter_tilt_pid_vals.last_error[0] >= (0 -lock_window)) &&
            (shooter_pos_speed <= 20))
        {
          g_tilt_locked_in = TRUE;
        }
        else
        {
          g_tilt_locked_in = FALSE;
        }
      }
    }
  }

  /*
  printf("Shooter auto %d\r", shooter_pos_speed);
  */

  shooter_pos_speed_prev = shooter_pos_speed;
  return (INT8)shooter_pos_speed;
}


/*******************************************************************************
* FUNCTION NAME: shooter_oi_input()
* PURPOSE:       Handles driver input from OI for shooter
* ARGUMENTS:     OIData
* RETURNS:       none
*
*******************************************************************************/
void shooter_oi_input()
{
  if (HAT_RANGE_CHECK(Oi_shooter, SHOOTER_SW_ON))
  {
    shooter_state = SHOOTER_ON;
  }
  else if (HAT_RANGE_CHECK(Oi_shooter, SHOOTER_SW_OFF))
  {
    shooter_state = SHOOTER_OFF;
  }
  return;
}


/*******************************************************************************
* FUNCTION NAME: shooter_set_state()
* PURPOSE:       Turn shooter on / off (only use in autonomous, or to turn off
*                shooter when robot is disabled)
* ARGUMENTS:     OIData
* RETURNS:       none
*
*******************************************************************************/
void shooter_set_state(ShooterType state)
{
  shooter_state = state;

  return;
}


/*******************************************************************************
* FUNCTION NAME: shooter_speed_feedback()
* PURPOSE:       Sets shooter motor speed
* ARGUMENTS:     OIData
* RETURNS:       none
*
*******************************************************************************/
INT8 shooter_speed_feedback()
{
  INT16 shooter_motor = 0;
  UINT16 shooter_speed;
  UINT8 cc_ret_status;

  if ((shooter_state == SHOOTER_ON) && (disabled_mode == ROBOT_ENABLED))
  {
    cc_ret_status = cc_get_shooter_speed(&shooter_speed);

    if (cc_ret_status != CC_SUCCESS)
    {
      shooter_speed = SHOOTER_DESIRED_SPEED;
    }

    /*
    printf("shooter speed %u ", shooter_speed);
    */
    show_pid = 0;
    shooter_motor = SHOOTER_BASE_PWM +
                    ws_pid(&shooter_speed_pid_vals, shooter_speed,
                           SHOOTER_DESIRED_SPEED);

    if ((shooter_speed > (SHOOTER_DESIRED_SPEED - SHOOTER_SPEED_DIFF)) &&
        (shooter_speed < (SHOOTER_DESIRED_SPEED + SHOOTER_SPEED_DIFF)))
    {
      g_shooter_speed_locked_in = TRUE;
    }
    else
    {
      g_shooter_speed_locked_in = FALSE;
    }

    MIN(shooter_motor, 0);
    MAX(shooter_motor, 127);
  }
  else
  {
    g_shooter_speed_locked_in = FALSE;
  }

  return (INT8)shooter_motor;
}


/*******************************************************************************
* FUNCTION NAME: get_shooter_tilt_pot()
* PURPOSE:       Returns the value of the shooter tilt pot
* ARGUMENTS:
* RETURNS:
*
*******************************************************************************/
UINT16 get_shooter_tilt_pot()
{
  return (Get_Analog_Value(Analog_in_tilt_pot) >> 1);
}

/*******************************************************************************
* FUNCTION NAME: lookup_shooter_tilt()
* PURPOSE:       Looks up the tilt pos based on the camera tilt servo
* ARGUMENTS:     pos - camera tilt servo value to be mapped
* RETURNS:       new_pos - tilt position that maps to the input pos
*
*******************************************************************************/
UINT16 lookup_shooter_tilt(UINT8 pos)
{
  UINT16 new_pos;
  UINT8 index;

  /* Range check pos so that it is in a valid range */
  MIN(pos, CC_TILT_RET_LOW);
  MAX(pos, CC_TILT_RET_HIGH);

  /* Map the input to the output */
  index = pos - CC_TILT_RET_LOW;
  new_pos = tilt_lookup[index];
  new_pos += calibration_vals.shooter_top;

  /*
  printf(" LUP %03d -> %03d -> %03d\r", pos, index, new_pos);
  */

  return new_pos;
}


/*******************************************************************************
* FUNCTION NAME: set_led_state()
* PURPOSE:       sets the LED state variable
* ARGUMENTS:     pos - state to set the LED to (on or off)
*                mask - which LED to set
* RETURNS:       none
*******************************************************************************/
void set_led_state(LedState state, UINT8 mask)
{
  if (state == LED_ON)
  {
    g_turret_leds |= mask;
  }
  else if (state == LED_OFF)
  {
    g_turret_leds &= ~mask;
  }
}

/*******************************************************************************
* FUNCTION NAME: shooter_locked()
* PURPOSE:       Determine wheter the shooter is locked in and ready to fire
* ARGUMENTS:     none
* RETURNS:       Boolean value
*******************************************************************************/
UINT8 shooter_locked(UINT8 ignore_speed)
{
  UINT8 ret = FALSE;
  static UINT8 prev_speed_locked = FALSE;
  if ((ignore_speed == TRUE) && (prev_speed_locked == TRUE))
  {
    ret = ((g_turret_locked_in == TRUE) &&
           (g_tilt_locked_in == TRUE));
  }
  else
  {
    ret = ((g_turret_locked_in == TRUE) &&
           (g_tilt_locked_in == TRUE) &&
           (g_shooter_speed_locked_in));
  }

  if (g_shooter_speed_locked_in == TRUE)
  {
    prev_speed_locked = TRUE;
  }
  return ret;
}

/*******************************************************************************
* FUNCTION NAME: display_turret_leds()
* PURPOSE:       output the LED state variable to the OI
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void display_turret_leds()
{
  display_oi_data(g_turret_leds, DISPLAY_DATA_TURRET);
}


