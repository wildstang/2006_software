/*******************************************************************************
* FILE NAME: ws_turret.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_turret.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_turret_h_
#define __ws_turret_h_

/******************************* TYPEDEFS *************************************/

typedef enum
{
  TURRET_INVALID = 0,
  TURRET_MANUAL,
  TURRET_MANUAL_LOCK,
  TURRET_AUTO,
  TURRET_AUTO_LOST
} TurretStateType;


typedef enum
{
  LATCH_NONE = 0,
  LATCH_LEFT,
  LATCH_RIGHT
} TurretLatchStateType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define MANUAL_TURRET_DEADZONE            5
#define MANUAL_SHOOTER_TILT_DEADZONE      20

#define TURRET_SCALE_NUMERATOR            1
#define TURRET_SCALE_DENOMINATOR          1

#define SHOOTER_TILT_SCALE_NUMERATOR      1
#define SHOOTER_TILT_SCALE_DENOMINATOR    1

#ifdef REAL_ROBOT
/* real robot */
#define CC_TILT_RET_LOW                   69
#define CC_TILT_RET_HIGH                  134
#endif

#ifdef PROTO_ROBOT
/* proto */
#define CC_TILT_RET_LOW                   42
#define CC_TILT_RET_HIGH                  116
#endif

#define CC_TILT_LOOKUP_SIZE               (CC_TILT_RET_HIGH - CC_TILT_RET_LOW)

#define USE_TURRET_LOST_LOOP_LIMIT        0
#define TURRET_LOST_LOOP_LIMIT            240

#define TURRET_CENTER                      9

#define SHOOTER_TILT_LIMIT_TOP            394 /* MAX 384 */
#define SHOOTER_TILT_LIMIT_BOT            464 /* MAX 474 */
#define DEFAULT_SHOOTER_TILT_TOP          SHOOTER_TILT_LIMIT_TOP
#define DEFAULT_SHOOTER_TILT_BOT          SHOOTER_TILT_LIMIT_BOT

#define SHOOTER_DESIRED_SPEED             2500
#define SHOOTER_BASE_PWM                  108
#define SHOOTER_SPEED_DIFF                30


/* THIS SHOULD BE SET TO 0 UNLESS TUNING THE TURRET PID!!! */
#define TURRET_PID_TUNE_CODE 0

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
void turret_init(void);
void shooter_init(void);

void turret_oi_input(void);
INT8 turret_manual_control(void);
void turret_set_pos(UINT8, INT8);
INT8 turret_pos_feedback(void);
void process_turret_sensors(void);
INT8 turret_pid_tune(void);

void shooter_manual_control(void);
void shooter_set_pos(UINT8, UINT16);
INT8 shooter_pos_feedback(void);

void shooter_oi_input(void);
void shooter_set_state(ShooterType state);
INT8 shooter_speed_feedback(void);

UINT8 shooter_locked(UINT8 ignore_speed);

UINT16 get_shooter_tilt_pot(void);
UINT16 lookup_shooter_tilt(UINT8 pos);

void set_led_state(LedState, UINT8);
void display_turret_leds(void);

#endif /* __ws_turret_h_ */
