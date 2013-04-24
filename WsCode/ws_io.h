/*******************************************************************************
* FILE NAME: ws_io.h
*
* DESCRIPTION:
*  This is the include file which corresponds to io_code.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_io_h_
#define __ws_io_h_

/******************************* TYPEDEFS *************************************/

typedef enum
{
  DISPLAY_DATA_PSI,
  DISPLAY_DATA_AUTO,
  DISPLAY_DATA_CALIBRATE,
  DISPLAY_DATA_OVERRIDE,
  DISPLAY_DATA_TURRET
} DisplayDataType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/
#define OI_CALIBRATE_ENCODERS    50
#define OI_CALIBRATE_JOYSTICKS   200

#define SHIFT_SWITCH_LOW          0
#define SHIFT_SWITCH_HIGH         1

#define ACCUMULATOR_SW_IN         200
#define ACCUMULATOR_SW_OUT        50

#define LOADER_SW_UP              50
#define LOADER_SW_DOWN            200

#define SHOOTER_SW_ON             HAT_UP
#define SHOOTER_SW_OFF            HAT_DOWN

#define PRESSURE_BELOW_120        0
#define PRESSURE_ABOVE_120        1

#define ACCUMULATOR_SPEED_FAST_IN       0
#define ACCUMULATOR_SPEED_SLOW_IN       (127 - 70)
#define ACCUMULATOR_SPEED_SLOW_OUT      (127 + 70)
#define ACCUMULATOR_SPEED_FAST_OUT      254

#define LOADER_SPEED_UP           254
#define LOADER_SPEED_DOWN         0

#define SHOOTER_SPEED_ON          (127 + 102)
#define SHOOTER_SPEED_OFF         127

#define BALL_PRESENT              0
#define BALL_NOT_PRESENT          1

#define SHOOTER_NOT_READY         0
#define SHOOTER_READY             1

/**************************************************************
 * Inputs
 **************************************************************/

/***** RC Analog Inputs *****/
#define Analog_in_tilt_pot               rc_ana_in01
#define Analog_in_02                     rc_ana_in02
#define Analog_in_03                     rc_ana_in03
#define Analog_in_04                     rc_ana_in04
#define Analog_in_05                     rc_ana_in05
#define Analog_in_06                     rc_ana_in06
#define Analog_in_07                     rc_ana_in07
#define Analog_in_08                     rc_ana_in08
#define Analog_in_09                     rc_ana_in09
#define Analog_in_10                     rc_ana_in10
#define Analog_in_11                     rc_ana_in11
#define Analog_in_12                     rc_ana_in12
#define Analog_in_13                     rc_ana_in13
#define Analog_in_14                     rc_ana_in14
#define Analog_in_15                     rc_ana_in15
#define Analog_in_16                     rc_ana_in16


/* The digital inputs / outputs are the same pins, a pin can either be an
   output or an input, not both */
/***** RC Digital Inputs *****/
#define Dig_in_pressure           rc_dig_in01
#define Dig_in_turret_sw_r        rc_dig_in02
#define Dig_in_turret_sw_l        rc_dig_in03
#define Dig_in_04                 rc_dig_in04
#define Dig_in_05                 rc_dig_in05
#define Dig_in_06                 rc_dig_in06
#define Dig_in_07                 rc_dig_in07
#define Dig_in_08                 rc_dig_in08
#define Dig_in_09                 rc_dig_in09
#define Dig_in_10                 rc_dig_in10
#define Dig_in_11                 rc_dig_in11
#define Dig_in_12                 rc_dig_in12
#define Dig_in_13                 rc_dig_in13
#define Dig_in_ball_top_loader    rc_dig_in14
#define Dig_in_shooter_state      rc_dig_in15
#define Dig_in_16                 rc_dig_in16
#define Dig_in_17                 rc_dig_in17
#define Dig_in_18                 rc_dig_in18

/***** RC Digital Outputs *****/
#define Dig_out_01                rc_dig_out01
#define Dig_out_02                rc_dig_out02
#define Dig_out_03                rc_dig_out03
#define Dig_out_brake_mode        rc_dig_out04
#define Dig_out_05                rc_dig_out05
#define Dig_out_06                rc_dig_out06
#define Dig_out_07                rc_dig_out07
#define Dig_out_08                rc_dig_out08
#define Dig_out_09                rc_dig_out09
#define Dig_out_see_green         rc_dig_out10
#define Dig_out_11                rc_dig_out11
#define Dig_out_12                rc_dig_out12
#define Dig_out_13                rc_dig_out13
#define Dig_out_14                rc_dig_out14
#define Dig_out_15                rc_dig_out15
#define Dig_out_16                rc_dig_out16
#define Dig_out_17                rc_dig_out17
#define Dig_out_18                rc_dig_out18


/***** Drive Joystick Analog Inputs *****/
#define Oi_drive_x                    p4_x
#define Oi_drive_y                    p4_y

/***** Drive Joystick Digital Inputs *****/
#define Oi_sw_turbo                   p4_sw_trig /* drive stick trigger */
#define Oi_sw_bumper_out              p4_sw_top
#define Oi_sw_bumper_out_prev         p4_sw_top_prev
#define Oi_sw_bumper_in               p4_sw_aux1
#define Oi_sw_bumper_in_prev          p4_sw_aux1_prev


/***** Manipulator Joystick Analog Inputs *****/
#define Oi_turret_stick               p2_x
#define Oi_shooter_tilt               p2_y
#define Oi_shooter                    p2_aux

/***** Manipulator Joystick Digital Inputs *****/
#define Oi_sw_cal_shooter_tilt        p2_sw_trig
#define Oi_sw_cal_shooter_tilt_prev   p2_sw_trig_prev
#define Oi_sw_turret_manu_move        p2_sw_trig
#define Oi_sw_turret_manu_move_prev   p2_sw_trig_prev
#define Oi_sw_shooter_tilt_manu_move  p2_sw_top
#define Oi_sw_camera_up               p2_sw_aux1
#define Oi_sw_camera_up_prev          p2_sw_aux1_prev


/***** Button Box Inputs *****/
/* Driver */
#define Oi_accumulator                p3_aux
#define Oi_sw_shifter                 p3_sw_trig

/* Manipulator */
#define Oi_sw_turret_auto             p1_sw_top
#define Oi_loader_manu                p1_x

// NOTE: p2_sw_aux2 doesn't work on 2005 OI
/*
#define Oi_sw_loader_auto             p2_sw_aux2
#define Oi_sw_loader_auto_prev        p2_sw_aux2_prev
*/

#define Oi_sw_thumper                 p1_sw_aux1

/* General */
#define Oi_calibrate                  p3_wheel
#define Oi_auto_prog_select           p3_y
#define Oi_auto_pos_select            p3_x
#define Oi_auto_delay_select          p1_y
#define Oi_sw_auto_lockin             p3_sw_top

/* debug buttons */
#define Oi_sw_get_dst_hdg             p1_sw_aux2
#define Oi_sw_center_camera           p1_sw_aux1
#define Oi_sw_center_camera_prev      p1_sw_aux1_prev
#define Oi_sw_9090_camera             p1_sw_top
#define Oi_sw_9090_camera_prev        p1_sw_top_prev
#define Oi_sw_cc_center_n_look        p1_sw_trig
#define Oi_sw_cc_center_n_look_prev   p1_sw_trig_prev
#define Oi_sw_cc_start_moving         p3_sw_aux2
#define Oi_sw_cc_start_moving_prev    p3_sw_aux2_prev
#define Oi_sw_encoder_debug           p2_sw_aux1



/**************************************************************
 * Outputs
 **************************************************************/
/***** RC Digital Outputs *****/
#define Rc_relay_shifter_1           relay1_fwd   /* blue/black   */
#define Rc_relay_shifter_2           relay1_rev
#define Rc_relay_bumper_1            relay2_fwd   /* green/black  */
#define Rc_relay_bumper_2            relay2_rev   /* green/white  */
#define Rc_relay_ball_accel_fwd      relay3_fwd   /* orange/black */
#define Rc_relay_ball_accel_rev      relay3_rev
#define Rc_relay_4_fwd               relay4_fwd   /* red/black    */
#define Rc_relay_4_rev               relay4_rev
#define Rc_relay_5_fwd               relay5_fwd   /* yellow/black */
#define Rc_relay_5_rev               relay5_rev
#define Rc_relay_pump_on             relay6_fwd   /* red/green    */
#define Rc_relay_6_rev               relay6_rev
#define Rc_relay_7_fwd               relay7_fwd   /* grey/black   */
#define Rc_relay_7_rev               relay7_rev
#define Rc_relay_8_fwd               relay8_fwd   /* black/black  */
#define Rc_relay_8_rev               relay8_rev

/***** RC Analog Outputs *****/
#define Rc_analog_out_r_drive_1           pwm01   /* blue         */
#define Rc_analog_out_r_drive_2           pwm02   /* green        */
#define Rc_analog_out_l_drive_1           pwm03   /* orange       */
#define Rc_analog_out_l_drive_2           pwm04   /* red          */
#define Rc_analog_out_accumulator         pwm05   /* yellow       */
#define Rc_analog_out_loader              pwm06   /* grey         */
#define Rc_analog_out_turret              pwm07   /* purple       */
#define Rc_analog_out_shooter             pwm08   /* brown        */
#define Rc_analog_out_shooter_tilt        pwm09   /* black        */
#define Rc_analog_out_cam_servo_pan       pwm10   /* blue/brown   */
#define Rc_analog_out_cam_servo_tilt      pwm11   /* green/brown  */
#define Rc_analog_out_thumper             pwm12   /* orange/brown */
#define Rc_analog_out_pwm13               pwm13   /* ?            */
#define Rc_analog_out_pwm14               pwm14   /* ?            */
#define Rc_analog_out_pwm15               pwm15   /* ?            */
#define Rc_analog_out_pwm16               pwm16   /* ?            */

/**************************************************************
 * LEDs
 **************************************************************/
/* #defines for auton LEDs */
#define NEAR_POS_LED      Relay2_red
#define CENTER_POS_LED    Relay2_green
#define FAR_POS_LED       Relay1_red

/* LEDs for turret lock-on */
#define TURRET_SEE_GREEN_LED_MASK  LED_MASK_PWM1_RED    /* blue LED */
#define TURRET_AUTO_LED_MASK       LED_MASK_PWM2_RED    /* yellow LED */
#define TURRET_FIRE_READY_LED_MASK LED_MASK_PWM2_GREEN  /* red LED */

/***** Input Scaling *****/
/* drive joystick scaling constants */
#define DRIVE_STICK_X_MIN        7
#define DRIVE_STICK_X_MIDDLE     127
#define DRIVE_STICK_X_MAX        254
#define DRIVE_STICK_Y_MIN        23
#define DRIVE_STICK_Y_MIDDLE     127
#define DRIVE_STICK_Y_MAX        253

#define MANUAL_TURRET_STICK_MIN      33
#define MANUAL_TURRET_STICK_MIDDLE  127
#define MANUAL_TURRET_STICK_MAX     250

/************ STILL NEED TO GET REAL NUMBERS ************/
#define MANUAL_SHOOTER_TILT_STICK_MIN      41
#define MANUAL_SHOOTER_TILT_STICK_MIDDLE  127
#define MANUAL_SHOOTER_TILT_STICK_MAX     254

/* lift joystick scaling constants */

#define SC_CALIB_STICK_DEADZONE   5

#define BOT_SPEAR_MOVE_COUNT    80

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/
extern void assign_outputs_slow(void);
extern void assign_outputs_fast(void);
extern UINT8 joystick_scaling(UINT8, UINT8, UINT8, UINT8);
extern void io_print_oi_inputs(void);
extern void io_print_rc_inputs(void);
extern void display_oi_data(UINT8, DisplayDataType);

#endif /* __ws_io_h_ */

