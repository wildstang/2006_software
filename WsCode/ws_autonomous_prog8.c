#include <stdio.h>
#include "ifi_aliases.h"
#include "ifi_default.h"
#include "ifi_utilities.h"
#include "ws_includes.h"
#include "user_routines.h"
#include "delays.h"       /*defined locally*/
#include "ws_io.h"

#include "ws_autonomous.h"
#include "ws_autonomous_tasks.h"
#include "ws_autonomous_prog.h"

/* Auto Program 8
 *
 * Short Fly - drive straight and shoot with a delay
 * 
 */

AutoShootDriveType drive_points_far_8[] = 
{
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 100,    0,      0},
  {  200,     127,     SHIFT_SWITCH_HIGH,  60,    0,      0},
  {  127,     127,     SHIFT_SWITCH_HIGH,  0,     0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      40},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
};

AutoShootDriveType drive_points_center_8[] = 
{
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 260,    0,      0},
  {  127,     127,     SHIFT_SWITCH_HIGH,  0,     0,      50},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      40},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
};


AutoShootDriveType drive_points_near_8[] = 
{
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 470,    0,      0},
  {  127,     127,     SHIFT_SWITCH_HIGH,  0,     0,      65},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      40},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
}; 
UINT8 auto_prog8(void)
{
  UINT8 prog_ret = AUTO_PROGRAM_NOT_DONE;

  if(g_starting_pos == STARTING_POS_CENTER)
  {
    auto_shoot(drive_points_center_8);
  }
  else if(g_starting_pos == STARTING_POS_FAR)
  {
    auto_shoot(drive_points_far_8);
  }
  else if(g_starting_pos == STARTING_POS_NEAR)
  {
    auto_shoot(drive_points_near_8);
  }
  else
  {
    prog_ret = AUTO_PROGRAM_DONE;
  }


  return prog_ret;
}

