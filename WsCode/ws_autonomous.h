/*******************************************************************************
* FILE NAME: ws_autonomous.h
*
* DESCRIPTION:
*  This is the include file which corresponds to autonomous.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_autonomous_h_
#define __ws_autonomous_h_

#define FAKE_AUTON 0
#define PRINT_TASK_INFO 1

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/


/* autonomous program selector knob */
/* ORIGINAL NUMBERS*/
/*
#define AUTO_PROG_OI_SEL0   250
#define AUTO_PROG_OI_SEL1   229
#define AUTO_PROG_OI_SEL2   203
#define AUTO_PROG_OI_SEL3   176
#define AUTO_PROG_OI_SEL4   152
#define AUTO_PROG_OI_SEL5   127
#define AUTO_PROG_OI_SEL6   100
#define AUTO_PROG_OI_SEL7    75
#define AUTO_PROG_OI_SEL8    49
#define AUTO_PROG_OI_SEL9    22
#define AUTO_PROG_OI_SEL10    4
#define AUTO_PROG_OI_DIFF     4
*/
#define AUTO_PROG_OI_SEL0   250
#define AUTO_PROG_OI_SEL1   229
#define AUTO_PROG_OI_SEL2   203
#define AUTO_PROG_OI_SEL3   176
#define AUTO_PROG_OI_SEL4   149
#define AUTO_PROG_OI_SEL5   124
#define AUTO_PROG_OI_SEL6    98
#define AUTO_PROG_OI_SEL7    72
#define AUTO_PROG_OI_SEL8    45
#define AUTO_PROG_OI_SEL9    20
#define AUTO_PROG_OI_SEL10    4
#define AUTO_PROG_OI_DIFF     4

/* starting position selector */
#define AUTO_POS_OI_FAR        4
#define AUTO_POS_OI_CENTER   127
#define AUTO_POS_OI_NEAR     250
#define AUTO_POS_OI_DIFF       4

/* starting delay selector */
#define AUTO_DELAY_OI_NONE       4
#define AUTO_DELAY_OI_ONE      127
#define AUTO_DELAY_OI_TWO      250
#define AUTO_DELAY_OI_DIFF       4

/* Constants to determine program state */
#define AUTO_PROGRAM_NOT_DONE  0
#define AUTO_PROGRAM_DONE      1

/* Constant for the size of auto programs */
#define MAX_AUTO_PROGRAMS  1

#define AUTO_LED_BLINK_END 8
#define AUTO_LED_BLINK_DELAY_END (AUTO_LED_BLINK_END + 28)

/****************************** STRUCTURES ************************************/

/*******************************************
2006 Autonomous Constants and Data Types
*******************************************/
typedef enum
{
  STARTING_POS_UNINIT = 0,
  STARTING_POS_FAR,
  STARTING_POS_CENTER,
  STARTING_POS_NEAR
} StartingPosType;

typedef enum
{
  STARTING_DELAY_NONE = 0,
  STARTING_DELAY_ONE,
  STARTING_DELAY_TWO
} StartingDelayType;

typedef enum
{
  BLINK_ON,
  BLINK_OFF,
  BLINK_DELAY
} LEDBlinkStates;

typedef enum
{
  AUTO_OFF = 0,
  AUTO_ON
} AutoStateType;

typedef enum
{
  STARTING_POS_NOT_SET,
  STARTING_POS_SET
} StartPosSetType;

typedef enum
{
  TASK_STATE_PROCESSING = 0,
  TASK_STATE_DONE
} AutoTaskState;

typedef enum
{
  AUTO_SHOOT_INVALID = 0,
  AUTO_SHOOT_INIT,
  AUTO_SHOOT_DELAY,
  AUTO_SHOOT_DRIVE,
  AUTO_SHOOT_DEAD_ENCODER,
  AUTO_SHOOT_STOP
} AutoShootState;



/************************* FUNCTION PROTOTYPES ********************************/

extern void autonomous_init(void);
extern void auto_lock_in(void);
extern void auto_main(void);
extern void auto_chooser(void);
extern void auto_output_off(void);
extern void display_auto_data(void);
extern UINT8 auto_shoot(AutoShootDriveType[]);
extern UINT8 auto_run_program(UINT8, UINT8);
extern UINT8 auto_prog1(void);
extern UINT8 auto_prog2(void);
extern UINT8 auto_prog3(void);
extern UINT8 auto_prog4(void);
extern UINT8 auto_prog5(void);
extern UINT8 auto_prog6(void);
extern UINT8 auto_prog7(void);
extern UINT8 auto_prog8(void);
extern UINT8 auto_prog9(void);
extern UINT8 auto_prog10(void);

extern UINT8 g_starting_pos;
extern UINT8 g_starting_delay;

#endif /* __ws_autonomous_h_ */

