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
#include "ws_auto_drive.h"
#include "ws_general.h"
#include "ws_turret.h"

/* Auto Program 2
 * The Sticker - Shoot from starting position
 */
UINT8 auto_prog2(void)
{
  UINT8 prog_ret = AUTO_PROGRAM_NOT_DONE;

  AutoShootDriveType drive_points[] = 
  {
    /**************************************************
     *                                              NOT_USED
     * y_stick, x_stick, shifter,           l_tick, r_tick, delay
     *************************************************/
    /**** D O N E  S E T  T I C K S  T O  Z E R O ***/
    {  127,     127,     SHIFT_SWITCH_LOW,  0,      0,      0}
  };

  auto_shoot(drive_points);

  return prog_ret;
}

