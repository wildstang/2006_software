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
#include "ws_autonomous_tasks.h"
#include "ws_autonomous_prog.h"

/* Auto Program 9
 *
 * Crosser - Cross the field and shoot
 * 
 */
UINT8 auto_prog9(void)
{
  UINT8 prog_ret = AUTO_PROGRAM_NOT_DONE;

  AutoShootDriveType drive_points_far[] = 
  {
    /**************************************************
     *                                              NOT_USED
     * y_stick, x_stick, shifter,           l_tick, r_tick, delay
     *************************************************/
    {  250,     254,     SHIFT_SWITCH_HIGH, 45,    0,      0},
    /* original value
    {  254,     127,     SHIFT_SWITCH_HIGH, 480,    0,      0},
    */
    {  254,     127,     SHIFT_SWITCH_HIGH, 530,    0,      0},
    /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
    {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      60},
    {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      20},
    {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
  };

  AutoShootDriveType drive_points_center[] = 
  {
    /**************************************************
     *                                              NOT_USED
     * y_stick, x_stick, shifter,           l_tick, r_tick, delay
     *************************************************/
    /* Original value *
     {  254,     100,    SHIFT_SWITCH_HIGH,  1100,    0,      0},
     */
    {  254,     90,    SHIFT_SWITCH_HIGH,  1050,    0,      0},
    {  127,     127,     SHIFT_SWITCH_HIGH,  0,     0,      60},
    /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
    {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,     0},
  };

  AutoShootDriveType drive_points_near[] = 
  {
    /**************************************************
     *                                              NOT_USED
     * y_stick, x_stick, shifter,           l_tick, r_tick, delay
     *************************************************/
    {  254,     100,    SHIFT_SWITCH_HIGH,  1400,    0,      0},
    {  127,     127,     SHIFT_SWITCH_HIGH,  0,     0,      60},
    /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
    {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,     0},
  };

  if(g_starting_pos == STARTING_POS_CENTER)
  {
    auto_shoot(drive_points_center);
  }
  else if(g_starting_pos == STARTING_POS_FAR)
  {
    auto_shoot(drive_points_far);
  }
  else if(g_starting_pos == STARTING_POS_NEAR)
  {
    auto_shoot(drive_points_near);
  }
  else
  {
    prog_ret = AUTO_PROGRAM_DONE;
  }

  return prog_ret;
}
