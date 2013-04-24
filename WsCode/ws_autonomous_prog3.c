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

/* Auto Program 3
 *
 * Thriller - Post pattern and shoot
 * 
 */
AutoShootDriveType drive_points_far_3[] = 
{
#if HIGH_GEAR_TRACTION_FRONT
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 200,    0,      0},
  {  240,     200,     SHIFT_SWITCH_HIGH, 370,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#else
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 50,    0,      0},
  {  240,     210,     SHIFT_SWITCH_HIGH, 500,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#endif
};

AutoShootDriveType drive_points_center_3[] = 
{
#if HIGH_GEAR_TRACTION_FRONT
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 475,    0,      0},
  {  240,     200,     SHIFT_SWITCH_HIGH, 375,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#else
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 200,    0,      0},
  {  240,     217,     SHIFT_SWITCH_HIGH, 625,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#endif
};

AutoShootDriveType drive_points_near_3[] = 
{
#if HIGH_GEAR_TRACTION_FRONT
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 675,    0,      0},
  {  240,     200,     SHIFT_SWITCH_HIGH, 375,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#else
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 550,    0,      0},
  {  235,     240,     SHIFT_SWITCH_HIGH, 300,    0,      0},
  {  240,     127,     SHIFT_SWITCH_HIGH, 200,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#endif
};

UINT8 auto_prog3(void)
{
  UINT8 prog_ret = AUTO_PROGRAM_NOT_DONE;

  if(g_starting_pos == STARTING_POS_CENTER)
  {
    auto_shoot(drive_points_center_3);
  }
  else if(g_starting_pos == STARTING_POS_FAR)
  {
    auto_shoot(drive_points_far_3);
  }
  else if(g_starting_pos == STARTING_POS_NEAR)
  {
    auto_shoot(drive_points_near_3);
  }
  else
  {
    prog_ret = AUTO_PROGRAM_DONE;
  }

  return prog_ret;
}
