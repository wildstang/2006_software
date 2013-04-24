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
/* Auto Program 4
 *
 * Short Thriller - Post pattern and shoot
 * 
 */
AutoShootDriveType drive_points_far_4[] = 
{
#if HIGH_GEAR_TRACTION_FRONT
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 150,    0,      0},
  {  240,     200,     SHIFT_SWITCH_HIGH, 100,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#else
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 50,    0,      0},
  {  240,     210,     SHIFT_SWITCH_HIGH, 230,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      50},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#endif
};

AutoShootDriveType drive_points_center_4[] = 
{
#if HIGH_GEAR_TRACTION_FRONT
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 425,    0,      0},
  {  240,     200,     SHIFT_SWITCH_HIGH, 110,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#else
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 250,    0,      0},
  {  240,     210,     SHIFT_SWITCH_HIGH, 200,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      50},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#endif
};

AutoShootDriveType drive_points_near_4[] = 
{
#if HIGH_GEAR_TRACTION_FRONT
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 575,    0,      0},
  {  240,     200,     SHIFT_SWITCH_HIGH, 135,    0,      0},
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      40},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#else
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,     SHIFT_SWITCH_HIGH, 550,    0,      0},
  {  235,     230,     SHIFT_SWITCH_HIGH, 300,    0,      0},
  {  240,     127,     SHIFT_SWITCH_HIGH, 75,    0,      0},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_HIGH, 0,      0,      50},
  {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0},
#endif
};
UINT8 auto_prog4(void)
{
  UINT8 prog_ret = AUTO_PROGRAM_NOT_DONE;

  if(g_starting_pos == STARTING_POS_CENTER)
  {
    auto_shoot(drive_points_center_4);
  }
  else if(g_starting_pos == STARTING_POS_FAR)
  {
    auto_shoot(drive_points_far_4);
  }
  else if(g_starting_pos == STARTING_POS_NEAR)
  {
    auto_shoot(drive_points_near_4);
  }
  else
  {
    prog_ret = AUTO_PROGRAM_DONE;
  }

  return prog_ret;
}

