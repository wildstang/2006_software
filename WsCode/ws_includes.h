/*******************************************************************************
* FILE NAME: ws_includes.h
*
* DESCRIPTION:
*  General structures & enumerations
*
*******************************************************************************/

#ifndef __ws_includes_h_
#define __ws_includes_h_

/******************************* TYPEDEFS *************************************/

typedef enum
{
  LED_ON,
  LED_OFF,
  LED_UNCH
} LedState;

typedef enum
{
  FDBK_ENABLED,
  FDBK_DISABLED
} FeedbackState;

typedef enum
{
  RELAY_OFF = 0,
  RELAY_FWD,
  RELAY_REV
} RelayValsType;

typedef enum
{
  PUMP_OFF = 0,
  PUMP_ON
} PumpRunType;

typedef enum
{
  SHIFTER_LOW = 0,
  SHIFTER_HIGH
} ShifterPosType;

typedef enum
{
  BUMPER_IN = 0,
  BUMPER_HALF_OUT,
  BUMPER_OUT
} BumperPosType;

typedef enum
{
  BALL_ACCEL_OFF = 0,
  BALL_ACCEL_FWD,
  BALL_ACCEL_REV
} BallAccelType;

typedef enum
{
  THUMPER_UP = 0,
  THUMPER_DOWN
} ThumperType;

typedef enum
{
  ACCUMULATOR_OFF = 0,
  ACCUMULATOR_IN_FAST,
  ACCUMULATOR_OUT_FAST,
  ACCUMULATOR_IN_SLOW,
  ACCUMULATOR_OUT_SLOW
} AccumulatorType;

typedef enum
{
  LOADER_OFF = 0,
  LOADER_UP,
  LOADER_DOWN
} LoaderType;

typedef enum
{
  SHOOTER_OFF = 0,
  SHOOTER_ON
} ShooterType;

typedef enum
{
  BRAKE_MODE_ON = 0,
  BRAKE_MODE_OFF
} BrakeModeType;

/******************************** MACROS **************************************/


#define MIN_MAX(variable,min_val,max_val)  MIN((variable),(min_val)); MAX((variable),(max_val))

#define MAX(variable, max_val)  if (variable > (max_val)) variable = (max_val)
#define MIN(variable, min_val)  if (variable < (min_val)) variable = (min_val)

#define MAX_RETURN(value, max_val) ((value) > (max_val) ? (max_val) : (value))
#define MIN_RETURN(value, min_val) ((value) < (min_val) ? (min_val) : (value))

#define DEADZONE(var, zone)  if ((var > (127 - (zone))) && \
                                 (var < (127 + (zone)))) \
                               var = 127

#define GET_ANALOG_VALUE_SHIFT(a) (Get_Analog_Value(a) >> 2)

#define HAT_RANGE_CHECK(hat, value) \
   ((((hat) >= ((int)(value) - HAT_RANGE)) && \
     ((hat) <= ((int)(value) + HAT_RANGE))) ? \
    TRUE : FALSE)

/***************************** DEFINITIONS ************************************/

#define SUCCESS 1
#define FAIL    0

#define ROBOT_ENABLED  0
#define ROBOT_DISABLED 1

#define AUTO_ENABLED   1
#define AUTO_DISABLED  0

#define NUM_PACKETS_PER_SEC 40

#define HAT_RANGE    10

#define HAT_NONE     127
#define HAT_UP       HAT_RANGE
#define HAT_DOWN     254-HAT_RANGE
#define HAT_LEFT     185
#define HAT_RIGHT    65

/****************************** STRUCTURES ************************************/
typedef struct
{
  UINT8  status;
  UINT16 dist;
  INT16  hdg;
  UINT8  pan_pwm;
  UINT8  tilt_pwm;
  UINT8  valid_data;
} DistHdgType;

typedef struct
{
  INT8 left;
  INT8 right;
  UINT8 orient;
} EncoderValsType;

typedef struct calibration_vals_
{
  UINT16 shooter_top;
  UINT16 shooter_bot;
} CalibrationValsType;

typedef struct auto_shoot_drive_
{
  UINT8    y_stick;
  UINT8    x_stick;
  UINT8    shifter;
  UINT16   left_ticks;
  UINT16   right_ticks;
  UINT8    delay;
} AutoShootDriveType;

typedef struct motor_vals_
{
  INT8                left_drive;
  INT8                right_drive;
  PumpRunType         pump;
  ShifterPosType      shifter_position;
  BumperPosType       bumper_position;
  AccumulatorType     accumulator;
  BallAccelType       ball_accel;
  ThumperType         thumper;
  LoaderType          loader;
  INT8                turret;
  INT8                shooter_tilt;
  INT8                shooter;
  UINT8               cam_servo_pan;
  UINT8               cam_servo_tilt;
  BrakeModeType       brake_mode;
} MotorValsType;

extern MotorValsType motor_vals;
extern CalibrationValsType calibration_vals;
extern EncoderValsType g_encoder_vals;
extern UINT8 g_cc_encoder_ret_val;


/************************* FUNCTION PROTOTYPES ********************************/

#endif /* __ws_includes_h_ */

