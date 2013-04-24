/*******************************************************************************
* FILE NAME: ws_autonomous.c
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
#include "user_routines.h"
#include "delays.h"       /*defined locally*/
#include "ws_io.h"
#include "ws_cc.h"
#include "ws_general.h"

#include "ws_autonomous.h"

UINT8 g_starting_pos = STARTING_POS_UNINIT;
UINT8 g_starting_delay = STARTING_POS_UNINIT;

static UINT8 s_prog_num = 0;
static UINT8 s_auto_locked_in = FALSE;

/*******************************************************************************
* FUNCTION NAME: autonomous_init
* PURPOSE:       Initialize the autonomous system...called only once
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
void autonomous_init(void)
{
  return;
}


/*******************************************************************************
* FUNCTION NAME: auto_lock_in
* PURPOSE:       Lock in autonomous program & pass info to CC
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
void auto_lock_in(void)
{
  if (Oi_sw_auto_lockin == 1)
  {
    s_auto_locked_in = TRUE;
    /* Tell the cc that we're locked in */
  }
  else
  {
    /* lock in program selector & robot position selectors */
    auto_chooser();
    s_auto_locked_in = FALSE;
  }

  return;
}


/*******************************************************************************
* FUNCTION NAME: auto_chooser
* PURPOSE:       set autonomous program, side & starting position;
*                lock in forced auto
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void auto_chooser(void)
{
  /*
  printf("auto_prog %d, auto_pos %d ", (int)Oi_auto_prog_select,
         (int)Oi_auto_pos_select);
  */

  /* read autonomous program dial */
  /* Set s_prog_num based on where Oi_auto_prog_select is */
  /* Be sure to check that the switch is +- AUTO_PROG_OI_DIFF from the value */
  if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL0 - AUTO_PROG_OI_DIFF)) &&
     (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL0 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 1;
  }
  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL1 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL1 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 2;
  }
  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL2 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL2 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 3;
  }

  /* Only allow the user to select a program that has been written.  Uncomment
   * the programs below as they're written */

  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL3 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL3 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 4;
  }
  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL4 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL4 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 5;
  }
  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL5 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL5 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 6;
  }
  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL6 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL6 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 7;
  }
  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL7 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL7 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 8;
  }
  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL8 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL8 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 9;
  }
  /*
  else if((Oi_auto_prog_select >= (AUTO_PROG_OI_SEL9 - AUTO_PROG_OI_DIFF)) &&
          (Oi_auto_prog_select <= (AUTO_PROG_OI_SEL9 + AUTO_PROG_OI_DIFF)))
  {
    s_prog_num = 10;
  }
  */
  else
  {
    /* default to dead auto program */
    s_prog_num = 1;
  }

  /* read starting position dial */
  /* Set g_starting_pos based on where Oi_auto_pos_select is */
  /* Be sure to check that the switch is +- AUTO_POS_OI_DIFF from the value */
  if((Oi_auto_pos_select >= (AUTO_POS_OI_FAR - AUTO_POS_OI_DIFF)) &&
     (Oi_auto_pos_select <= (AUTO_POS_OI_FAR + AUTO_POS_OI_DIFF)))
  {
    g_starting_pos = STARTING_POS_FAR;
  }
  else if((Oi_auto_pos_select >= (AUTO_POS_OI_CENTER - AUTO_POS_OI_DIFF)) &&
          (Oi_auto_pos_select <= (AUTO_POS_OI_CENTER + AUTO_POS_OI_DIFF)))
  {
    g_starting_pos = STARTING_POS_CENTER;
  }
  else if((Oi_auto_pos_select >= (AUTO_POS_OI_NEAR - AUTO_POS_OI_DIFF)) &&
          (Oi_auto_pos_select <= (AUTO_POS_OI_NEAR + AUTO_POS_OI_DIFF)))
  {
    g_starting_pos = STARTING_POS_NEAR;
  }
  else
  {
    /* default to uninitialized */
    g_starting_pos = STARTING_POS_UNINIT;
  }

  /* read starting delay switch */
  /* Set g_starting_delay based on where Oi_auto_delay_select is */
  if ((Oi_auto_delay_select >= (AUTO_DELAY_OI_TWO - AUTO_DELAY_OI_DIFF)) &&
      (Oi_auto_delay_select <= (AUTO_DELAY_OI_TWO + AUTO_DELAY_OI_DIFF)))
  {
    g_starting_delay = STARTING_DELAY_TWO;
  }
  else if ((Oi_auto_delay_select >= (AUTO_DELAY_OI_ONE - AUTO_DELAY_OI_DIFF)) &&
           (Oi_auto_delay_select <= (AUTO_DELAY_OI_ONE + AUTO_DELAY_OI_DIFF)))
  {
    g_starting_delay = STARTING_DELAY_ONE;
  }
  else if ((Oi_auto_delay_select >=
            (AUTO_DELAY_OI_NONE - AUTO_DELAY_OI_DIFF)) &&
           (Oi_auto_delay_select <=
            (AUTO_DELAY_OI_NONE + AUTO_DELAY_OI_DIFF)))
  {
    g_starting_delay = STARTING_DELAY_NONE;
  }
  else
  {
    /* default to no delay */
    g_starting_delay = STARTING_DELAY_NONE;
  }

  /*
  printf("prog %d, pos %d delay %d\r", s_prog_num, g_starting_pos,
         g_starting_delay);
  */

  return;
}

/*******************************************************************************
* FUNCTION NAME: auto_main
* PURPOSE:       main loop for autonomous
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void auto_main()
{
  UINT8 ret = AUTO_PROGRAM_NOT_DONE;
  auto_output_off();

  printf("Run auto program %d\r",(int)s_prog_num);

  while (autonomous_mode == AUTO_ENABLED)
  {
    if (statusflag.NEW_SPI_DATA)      /* 26.2ms loop area */
    {
      Getdata(&rxdata); /* DO NOT DELETE, or it will be stuck here forever */

      /* Get a new encoder reading 
       * This is done in the slow loop, but that doesn't get called here
       * because of the local while loop*/
      g_cc_encoder_ret_val = cc_get_encoder_vals(&g_encoder_vals);

      /*
      printf("cc ret %d; enc l %d r %d\r", g_cc_encoder_ret_val,
             g_encoder_vals.left, g_encoder_vals.right);
      */

      /* Wildstang autonomous code */
      ret = auto_run_program(s_auto_locked_in, s_prog_num);

      if(ret == AUTO_PROGRAM_DONE)
      {
        auto_output_off();
      }

      assign_outputs_slow();
      Putdata(&txdata);   /* DO NOT DELETE, or you will get no PWM outputs! */
    }

    assign_outputs_fast();
    //Generate_Pwms(pwm13,pwm14,pwm15,pwm16);

    display_auto_data();
  }

  return;
}

/*******************************************************************************
* FUNCTION NAME: display_auto_data
* PURPOSE:       display autonomous data on the OI user display
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void display_auto_data(void)
{
  static UINT8 num_blinks = 0;
  static UINT16 num_cycles = 0;
  static UINT8 blink_state = BLINK_ON;
  //Default LEDs to off
  UINT8 do_led_on = 0;

  if ((Oi_calibrate < OI_CALIBRATE_JOYSTICKS) &&
      (Oi_calibrate > OI_CALIBRATE_ENCODERS))
  {
    /* only display auto data when not in joystick or encoder calibrate mode */
    NEAR_POS_LED = 0;
    CENTER_POS_LED = 0;
    FAR_POS_LED = 0;

    if(disabled_mode == ROBOT_DISABLED)
    {
      if(Oi_sw_auto_lockin == 1)
      {
        do_led_on = 1;
      }
      else
      {
        //  We are disabled and not locked in
        //  blink the program number (1-based)
        switch(blink_state)
        {
          case BLINK_ON:
            if(num_cycles == AUTO_LED_BLINK_END)
            {
              // We've been in the ON part of a blink
              // long enough
              // Move to off state, reset cycle count
              blink_state = BLINK_OFF;
              num_cycles = 0;
            }
            else
            {
              // Turn the LEDs on
              do_led_on = 1;
            }
            break;
          case BLINK_OFF:
            if(num_cycles == AUTO_LED_BLINK_END)
            {
              // We've been in the OFF part of a blink
              // long enough

              if(++num_blinks < (s_prog_num))
              {
                // We haven't blinked the number of times
                // for the current program
                // Reset the cycle count and start another blink
                num_cycles = 0;
                blink_state = BLINK_ON;
                do_led_on = 1;
              }
              else
              {
                // We've blinked the number of times for
                // the current program
                // Reset the number of blinks and move
                // to the delay state
                num_blinks = 0;
                blink_state = BLINK_DELAY;
              }
            }
            break;
          case BLINK_DELAY:
            if(num_cycles == AUTO_LED_BLINK_DELAY_END)
            {
              // We've delayed long enough
              // Start a new blink sequence
              num_cycles = 0;
              blink_state = BLINK_ON;
              do_led_on = 1;
            }
            break;
          default:
            break;
        }
        // Increment the cycle count
        num_cycles++;
      }
    }
    else if(autonomous_mode == AUTO_ENABLED)
    {
      do_led_on = 1;
    }

    if (do_led_on)
    {
      /* turn LEDs on when locked in or in auto; blink LEDs otherwise */
      switch (g_starting_pos)
      {
        case STARTING_POS_FAR:
          FAR_POS_LED = 1;
          break;
        case STARTING_POS_CENTER:
          CENTER_POS_LED = 1;
          break;
        case STARTING_POS_NEAR:
          NEAR_POS_LED = 1;
          break;
        case STARTING_POS_UNINIT:
          NEAR_POS_LED = 1;
          CENTER_POS_LED = 1;
          FAR_POS_LED = 1;
          break;
        default:
          break;
      }
    }
  }
  /* print the autonomous program info to the dashboard */
  /* Auto program # is the top 4 bits
     Starting position is bits 3 and 2
     Auto lock-in is bit 1, bit 0 is empty */

  User_Byte4 = s_prog_num << 4;
  User_Byte4 |= (g_starting_pos << 2);
  User_Byte4 |= (s_auto_locked_in << 1);
  /* Upper 2 bits of user byte1 should show delay */
  User_Byte1 = g_starting_delay << 6;
}



/*******************************************************************************
* FUNCTION NAME: auto_output_off
* PURPOSE:       Sets the outputs to a safe state.  Used for initialization
*                as well as at the end of autonomous
* ARGUMENTS:
* RETURNS:       none
*******************************************************************************/
void auto_output_off()
{
  set_motor_vals_off();
}

