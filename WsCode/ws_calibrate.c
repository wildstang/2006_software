/*******************************************************************************
* FILE NAME: ws_calibrate.c
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
#include "eeprom.h"
#include "ws_includes.h"
#include "delays.h"       /*defined locally*/

#include "ws_calibrate.h"
#include "ws_turret.h"
#include "ws_io.h"


#define CALIBRATION_DEBUG


/*******************************************************************************
* FUNCTION NAME: calibrate_pots
* PURPOSE:       store calibration values for crab & arms in EEPROM
* ARGUMENTS:     none
* RETURNS:       none
*
* To calibrate a pot position:
*   - set the pot to the desired position
*   - enable pot calibration mode
*
*******************************************************************************/
void calibrate_pots(void)
{
  static UINT8 eeprom_init_check_flag = FALSE;
  UINT8 bitmask;
  UINT16 shooter_tilt;


  /* only check once if EEPROM has been initialized */
  if (eeprom_init_check_flag == FALSE)
  {
    /* only initialize EEPROM if the 'known' bits are not correct */
    if ((EEPROM_Read(ADDR_KNOWN_BYTE1) != CALIBRATE_KNOWN_BYTE_1) &&
        (EEPROM_Read(ADDR_KNOWN_BYTE2) != CALIBRATE_KNOWN_BYTE_2))
    {
#ifdef CALIBRATION_DEBUG
      printf("initializing EEPROM  ");
#endif
      /* initialize bitmasks to 0 */
      EEPROM_Write(ADDR_DATA_BITMASK_SHOOTER, 0);

      /* set 'known' bytes */
      EEPROM_Write(ADDR_KNOWN_BYTE1, CALIBRATE_KNOWN_BYTE_1);
      EEPROM_Write(ADDR_KNOWN_BYTE2, CALIBRATE_KNOWN_BYTE_2);
    }

    eeprom_init_check_flag = TRUE;
  }



  if ((Oi_sw_cal_shooter_tilt == 1) && (Oi_sw_cal_shooter_tilt_prev == 0))
  {
    bitmask = EEPROM_Read(ADDR_DATA_BITMASK_SHOOTER);
    shooter_tilt = get_shooter_tilt_pot();

#ifdef CALIBRATION_DEBUG
    printf("shooter ");
#endif

    if (Oi_shooter_tilt > (127 + CALIBRATE_ZONE))
    {
      /* joystick is forward, calibrate top */
#ifdef CALIBRATION_DEBUG
      printf("top %d ", shooter_tilt);
#endif

      /* save calibration values & bitmask */
      EEPROM_Write(ADDR_SHOOTER_TOP_LO, (UINT8)shooter_tilt);
      EEPROM_Write(ADDR_SHOOTER_TOP_HI, (UINT8)(shooter_tilt >> 8));
      EEPROM_Write(ADDR_DATA_BITMASK_SHOOTER,
                   (bitmask | CAL_MASK_SHOOTER_TOP));
    }
    else if (Oi_shooter_tilt < (127 - CALIBRATE_ZONE))
    {
      /* joystick is back, calibrate bottom */
#ifdef CALIBRATION_DEBUG
      printf("bot %d ", shooter_tilt);
#endif

      EEPROM_Write(ADDR_SHOOTER_BOT_LO, (UINT8)shooter_tilt);
      EEPROM_Write(ADDR_SHOOTER_BOT_HI, (UINT8)(shooter_tilt >> 8));
      EEPROM_Write(ADDR_DATA_BITMASK_SHOOTER,
                   (bitmask | CAL_MASK_SHOOTER_BOT));
    }
  }

#ifdef CALIBRATION_DEBUG
    printf("\n");
#endif

  return;
}



/*******************************************************************************
* FUNCTION NAME: retrieve_calibration
* PURPOSE:       retrieve calibration values for crab & arms from EEPROM
* ARGUMENTS:     none
* RETURNS:       none
*******************************************************************************/
void retrieve_calibration()
{
  UINT8 verify1, verify2;
  UINT8 bitmask;

  verify1 = EEPROM_Read(ADDR_KNOWN_BYTE1);
  verify2 = EEPROM_Read(ADDR_KNOWN_BYTE2);

  if ((verify1 == CALIBRATE_KNOWN_BYTE_1) &&
      (verify2 == CALIBRATE_KNOWN_BYTE_2))
  {
    /* RC has been used for calibration, we can trust it */
#ifdef CALIBRATION_DEBUG
    printf("EEPROM is good ");
#endif

    /* shooter calibration vals */
    bitmask = EEPROM_Read(ADDR_DATA_BITMASK_SHOOTER);

#ifdef CALIBRATION_DEBUG
    printf("shooter bm %x ", bitmask);
    printf("pot %d ", get_shooter_tilt_pot());
#endif

    /* Top travel of shooter */
    if ((bitmask & CAL_MASK_SHOOTER_TOP) != 0)
    {
      calibration_vals.shooter_top = 0;
      calibration_vals.shooter_top = EEPROM_Read(ADDR_SHOOTER_TOP_LO);
      calibration_vals.shooter_top |=
        ((UINT16)EEPROM_Read(ADDR_SHOOTER_TOP_HI) << 8);
#ifdef CALIBRATION_DEBUG
      printf("top %d ", calibration_vals.shooter_top);
#endif
    }
    else
    {
      calibration_vals.shooter_top = DEFAULT_SHOOTER_TILT_TOP;
    }

    /* Bottom travel of shooter */
    if ((bitmask & CAL_MASK_SHOOTER_BOT) != 0)
    {
      calibration_vals.shooter_bot = 0;
      calibration_vals.shooter_bot = EEPROM_Read(ADDR_SHOOTER_BOT_LO);
      calibration_vals.shooter_bot |=
        ((UINT16)EEPROM_Read(ADDR_SHOOTER_BOT_HI) << 8);
#ifdef CALIBRATION_DEBUG
      printf("bot %d ", calibration_vals.shooter_bot);
#endif
    }
    else
    {
      calibration_vals.shooter_bot = DEFAULT_SHOOTER_TILT_BOT;
    }
  }
  else
  {
    /* RC has never been given calibration values, use defaults */
#ifdef CALIBRATION_DEBUG
    printf("EEPROM is unknown ");
#endif
    calibration_vals.shooter_top = DEFAULT_SHOOTER_TILT_TOP;
    calibration_vals.shooter_bot = DEFAULT_SHOOTER_TILT_BOT;
  }

#ifdef CALIBRATION_DEBUG
    printf("\r");
#endif

  return;
}



/*******************************************************************************
* FUNCTION NAME: display_calibration
* PURPOSE:       display joystick vals & pot values for crab & arms on OI
* ARGUMENTS:     none
* RETURNS:       none
*
* To display pot values on the OI:
*   - enable calibration mode
*   - move the joystick axis that corresponds to the pot (crab stick X,
*     big arm Y, big arm X, small arm X) full forward or left (depending on
*     the axis) and allow it to return to the middle
*   - now the OI displays the pot value
*
* To display joystick values on the OI:
*   - enable calibration mode
*   - enable manual crab mode
*   - move the joystick axis that you want to display full forward or left
*     (depending on the axis) and allow it to return to the middle
*   - now the OI displays the X/Y axis of the stick
*
*******************************************************************************/
void display_calibration(void)
{
  static DisplayType display_mode = DISPLAY_NONE;
  UINT16 display_data = 0;

  /* Check if the calibrate switch is in the joystick position. */
  if (Oi_calibrate >= OI_CALIBRATE_JOYSTICKS)
  {
    /* print joystick values when switch is to joystick side */
    /* Check if the joystick has been moved to the calibrate zone */
    if (Oi_drive_x >= 127 + CALIBRATE_ZONE)
    {
      display_mode = DISPLAY_DRIVE_X;
    }
    else if (Oi_drive_y >= 127 + CALIBRATE_ZONE)
    {
      display_mode = DISPLAY_DRIVE_Y;
    }
    else if (Oi_turret_stick >= 127 + CALIBRATE_ZONE)
    {
      display_mode = DISPLAY_MANUAL_TURRET;
    }
    else if (Oi_shooter_tilt >= 127 + CALIBRATE_ZONE)
    {
      display_mode = DISPLAY_MANUAL_SHOOTER_TILT;
    }
  }
  else
  {
    /* print pot/encoder values when switch is to calibrate encoder side */
    if (Oi_shooter_tilt > (127 + CALIBRATE_ZONE))
    {
      display_mode = DISPLAY_POT_SHOOTER;
    }
  }

  switch (display_mode)
  {
    case DISPLAY_DRIVE_X:
      display_data = Oi_drive_x;
      break;

    case DISPLAY_DRIVE_Y:
      display_data = Oi_drive_y;
      break;

    case DISPLAY_MANUAL_TURRET:
      display_data = Oi_turret_stick;
      break;

    case DISPLAY_MANUAL_SHOOTER_TILT:
      display_data = Oi_shooter_tilt;
      break;

    case DISPLAY_POT_SHOOTER:
      display_data = get_shooter_tilt_pot();
      break;

    default:
      break;
  }

  display_oi_data((UINT8)display_data, DISPLAY_DATA_CALIBRATE);
  User_Byte2 = (display_mode << 4) | (UINT8)((display_data >> 8) & 0x0f);
  User_Byte3 = (UINT8)display_data;

  return;
}


