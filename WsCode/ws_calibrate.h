/*******************************************************************************
* FILE NAME: ws_calibrate.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_calibrate.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_calibrate_h_
#define __ws_calibrate_h_

/******************************* TYPEDEFS *************************************/
typedef enum
{
  DISPLAY_NONE,
  DISPLAY_DRIVE_X,
  DISPLAY_DRIVE_Y,
  DISPLAY_MANUAL_TURRET,
  DISPLAY_MANUAL_SHOOTER_TILT,

  DISPLAY_POT_SHOOTER
} DisplayType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define CALIBRATE_ZONE      70

#define CALIBRATE_KNOWN_BYTE_1  0xCD
#define CALIBRATE_KNOWN_BYTE_2  0xEF

#define CAL_MASK_SHOOTER_TOP    0x01
#define CAL_MASK_SHOOTER_BOT    0x02

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void calibrate_pots(void);
extern void retrieve_calibration(void);
extern void display_calibration(void);

#endif /* __ws_calibrate_h_ */

