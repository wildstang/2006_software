/*******************************************************************************
* FILE NAME: ws_general.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_general.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_general_h_
#define __ws_general_h_

/******************************* TYPEDEFS *************************************/

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/

#define ACCUMULATOR_SLOW_LOOPS  5
#define THUMPER_CYCLE_LOOPS     10


typedef enum
{
  LOADER_AUTO = 0,
  LOADER_MANU
} LoaderMode;

typedef enum
{
  LOADER_AUTO_STOP = 0,
  LOADER_AUTO_LOADING,
  LOADER_AUTO_WAIT_SHOOT,
  LOADER_AUTO_SHOOTING
} LoaderAutoMode;

typedef enum
{
  THUMPER_STATE_OFF = 0,
  THUMPER_STATE_ON
} ThumperStateType;

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void accumulator_control(void);
extern void loader_control(void);
extern void pump_control(void);
extern void thumper_control(void);
extern void bumper_control(void);
extern void set_motor_vals_off(void);

#endif /* __ws_general_h_ */
