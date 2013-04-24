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

/* Auto Program 6
 *
 * Short slant and go
 * 
 */

AutoShootDriveType drive_points_far_6[] = 
{
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,    SHIFT_SWITCH_HIGH,  40,       0,      0},
  {  220,     127,    SHIFT_SWITCH_HIGH,  130,      0,      0},
  {  127,       0,    SHIFT_SWITCH_HIGH,  120,      0,      0},
  {  240,     127,    SHIFT_SWITCH_HIGH,  100,      0,      0},
  {  127,     127,    SHIFT_SWITCH_HIGH,  0,        0,      60},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_LOW,  0,       0,      0},
};

AutoShootDriveType drive_points_center_6[] = 
{
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,    SHIFT_SWITCH_HIGH,  40,       0,      0},
  {  220,     127,    SHIFT_SWITCH_HIGH,  150,      0,      0},
  {  127,       0,    SHIFT_SWITCH_HIGH,  180,      0,      0},
  {  240,     127,    SHIFT_SWITCH_HIGH,  280,      0,      0},
  {  127,     127,    SHIFT_SWITCH_HIGH,  0,        0,      60},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_LOW,  0,       0,      0},
};

AutoShootDriveType drive_points_near_6[] = 
{
  /**************************************************
   *                                              NOT_USED
   * y_stick, x_stick, shifter,           l_tick, r_tick, delay
   *************************************************/
  {  240,     127,    SHIFT_SWITCH_HIGH,  30,       0,      0},
  {  220,     127,    SHIFT_SWITCH_HIGH,  150,      0,      0},
  {  127,       0,    SHIFT_SWITCH_HIGH,  180,      0,      0},
  {  254,     127,    SHIFT_SWITCH_HIGH,  300,      0,      0},
  {  240,     127,    SHIFT_SWITCH_HIGH,  300,      0,      0},
  {  230,     127,    SHIFT_SWITCH_HIGH,  50,       0,      0},
  {  127,     127,    SHIFT_SWITCH_HIGH,  0,        0,      60},
  /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
  {  127,     127,     SHIFT_SWITCH_LOW,  0,       0,      0},
};

UINT8 auto_prog6(void)
{

  UINT8 prog_ret = AUTO_PROGRAM_NOT_DONE;


  if(g_starting_pos == STARTING_POS_CENTER)
  {
    auto_shoot(drive_points_center_6);
  }
  else if(g_starting_pos == STARTING_POS_NEAR)
  {
    auto_shoot(drive_points_near_6);
  }
  else if(g_starting_pos == STARTING_POS_FAR)
  {
    auto_shoot(drive_points_far_6);
  }
  else
  {
    prog_ret = AUTO_PROGRAM_DONE;
  }

  return prog_ret;
}

