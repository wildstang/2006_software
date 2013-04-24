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

/* Auto Program 1 
 * Sleeper - DO NOTHING
 */
UINT8 auto_prog1(void)
{
  static UINT8 prog_state = AP1_STATE_DONE;
  UINT8 prog_ret = AUTO_PROGRAM_DONE;

  switch(prog_state)
  {
    case AP1_STATE_DONE:
      prog_ret = AUTO_PROGRAM_DONE;
      break;
  }
  return prog_ret;
}
