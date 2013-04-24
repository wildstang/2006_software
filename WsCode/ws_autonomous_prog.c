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

UINT8 auto_run_program(UINT8 auto_lock, UINT8 prog)
{
  UINT8 ret = AUTO_PROGRAM_DONE;

  /* If we're not locked in, go to the default case
   * of do nothing */
  if(auto_lock == 0)
  {
    prog = 0;
  }

  switch(prog)
  {
    case 1:
      ret = auto_prog1();
      break;
    case 2:
      ret = auto_prog2();
      break;
    case 3:
      ret = auto_prog3();
      break;
    case 4:
      ret = auto_prog4();
      break;
    case 5:
      ret = auto_prog5();
      break;
    case 6:
      ret = auto_prog6();
      break;
    case 7:
      ret = auto_prog7();
      break;
    case 8:
      ret = auto_prog8();
      break;
    case 9:
      ret = auto_prog9();
      break;
    case 10:
      ret = auto_prog10();
      break;
    default:
      ret = auto_prog1();
      break;
  }
  return ret;
}


