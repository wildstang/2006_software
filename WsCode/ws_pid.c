/*******************************************************************************
* FILE NAME: ws_pid.c
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
#include "delays.h"

#include "ws_pid.h"
#include "ws_io.h"
#include "ws_general.h"

UINT8 show_pid = 0;
/*******************************************************************************
* FUNCTION NAME: pid
* PURPOSE:
* ARGUMENTS:
* RETURNS:
*
*******************************************************************************/
INT16 ws_pid(PidValsType *pid_vals, INT32 current, INT32 target)
{
  INT32 pid_val;
  INT32 error;
  INT32 prop_term;
  INT32 int_term;
  INT32 deriv_term;
  int i;

  /*
  printf("c %d t %d  ", (int)current, (int)target);
  */

  /* calculate the current error */
  error = target - current;

  /* calculate the proportional term of the PID equation */
  prop_term = pid_vals->prop_gain * error;

  if(error == 0)
  {
    /*
    pid_vals->integral *= .99;
    */
  }
  else
  {
    /* add the current error to the running integrated value */
    pid_vals->integral += error;
  }

  /* prevent integral wind-up */
  if (pid_vals->max_integral < pid_vals->integral)
  {
    pid_vals->integral = pid_vals->max_integral;
  }
  else if (-(pid_vals->max_integral) > pid_vals->integral)
  {
    pid_vals->integral = -(pid_vals->max_integral);
  }


  /* calculate the integral term using the integrated value & gain */
  int_term = pid_vals->int_gain * pid_vals->integral;

  /* calculate the differential term */
#if 0
  if (p4_wheel > 200)
  {
    printf("n");
#endif
    deriv_term = pid_vals->deriv_gain *
                 (error - pid_vals->last_error[LAST_ERROR_SIZE - 1]);
#if 0
  }
  else
  {
    printf("y");
    deriv_term = (pid_vals->deriv_gain *
                  (error - pid_vals->last_error[LAST_ERROR_SIZE - 1])) / error;
  }
#endif

  /* calculate the PID value using the previously calculate P, I, and D terms */
  pid_val = prop_term + int_term - deriv_term;

  pid_val = pid_val / pid_vals->scale_factor;

  /* limit PID value to max & min values */
  MIN(pid_val, pid_vals->min_val);
  MAX(pid_val, pid_vals->max_val);

  if (show_pid == 1)
  {
    printf("c %ld e %ld p %ld i %ld d %ld s %ld ", current,
        error, prop_term, int_term, deriv_term,
        pid_val);

    printf(" le ( ");
    for(i = 0; i < LAST_ERROR_SIZE; i++)
    {
      printf("%d ", pid_vals->last_error[i]);
    }
    printf(") ");

    printf("\r");
  }

  for(i = (LAST_ERROR_SIZE - 1); i > 0; i--)
  {
    pid_vals->last_error[i] = pid_vals->last_error[i-1];
  }
  pid_vals->last_error[0] = error;

  return (INT16)pid_val;
}


void pid_last_error_init( PidValsType *pid_vals)
{
  int i;
  for(i = 0; i < LAST_ERROR_SIZE; i++)
  {
    pid_vals->last_error[i] = 0;
  }
}
