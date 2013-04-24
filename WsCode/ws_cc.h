/*******************************************************************************
* FILE NAME: ws_cc.h
*
* DESCRIPTION:
*  This is the include file which corresponds to ws_cc.c
*
*  It contains some aliases and function prototypes used in those files.
*
* USAGE:
*  If you add your own routines to those files, this is a good place to add
*  your custom macros (aliases), type definitions, and function prototypes.
*******************************************************************************/

#ifndef __ws_cc_h_
#define __ws_cc_h_

/******************************* TYPEDEFS *************************************/

typedef enum
{
   CC_SUCCESS = 1,
   CC_TIMEOUT,
   CC_FAIL
} CcReturnType;

typedef enum
{
  LOOK_HIGH = 0,
  LOOK_LOW
} LookType;

/******************************** MACROS **************************************/

/***************************** DEFINITIONS ************************************/

/* Command bytes sent to CC to request data */
/* State(8bit), Dist(16bit), Hdg(16bit), PanPwm(8bit), TiltPwm(8bit) */
#define  CC_CMD_REQ_DIST_HDG               1
/* Left Encoder(8bit), Right Encoder(8bit), Orient(8bit) */
#define  CC_CMD_REQ_ENCODER                2
/* Shooter Speed(16bit) */
#define  CC_CMD_REQ_SHOOTER_SPEED          3

/* Command bytes sent to CC to set data or state */
/* position_id(8bit)  */
#define  CC_CMD_SET_POSITION               101
/* object track ID(8bit) */
#define  CC_CMD_SET_TRACK_OBJECT           102
/* no data  */
#define  CC_CMD_CENTER_AND_LOOK            103
/* no data  */
#define  CC_CMD_START_MOVING               104
/* no data  */
#define  CC_CMD_CENTER_AND_LOOK_HIGH       105

/* ack from CC for starting pos command */
#define  CC_ACK_STARTING_POS               151
/* ack from CC for object track command */
#define  CC_ACK_TRACK_OBJECT               152
/* ack from CC for center & look command */
#define  CC_ACK_CENTER_AND_LOOK            153
/* ack from CC for start moving command */
#define  CC_ACK_START_MOVING               154

/* Command bytes sent to CC for debugging purposes */
/* set camera position to center */
#define  CC_CMD_SET_CAMERA_CENTER          201
/* set camera position to 90 degrees right & down */
#define  CC_CMD_SET_CAMERA_9090            202
/* set camera position to store position */
#define  CC_CMD_SET_CAMERA_STORE           203


/* Num of loops before giving up on a resp from the CC */
#define  CC_LOOP_CNT_TIMEOUT  10000


/* Size of the response to various commands */
#define CC_RESP_DIST_HDG_SIZE     8  /* 1 byte status, 2 bytes dist, 2 bytes
                                        hdg, 1 byte pan pwm, 1 byte tilt pwm,
                                        1 byte valid data flag */
#define CC_RESP_ENCODER_VAL_SIZE  3  /* 1 byte left, 1 byte right, 1 byte orient*/
#define CC_RESP_STARTING_POS_SIZE    1  /* 1 byte cmd */
#define CC_RESP_CENTER_AND_LOOK_SIZE 1  /* 1 byte cmd */
#define CC_RESP_START_MOVING_SIZE    1  /* 1 byte cmd */
#define CC_RESP_TRACK_OBJECT_SIZE    1  /* 1 byte cmd */
#define CC_RESP_SHOOTER_SPEED_SIZE   2  /* 2 bytes speed */


/* IDs of objects to trace */
#define OBJ_TRACK_LIGHT      1


/* Tracking states returned in Dist Hdg Response */
#define STATE_SEE_GREEN            1
#define STATE_NO_GREEN             2
#define STATE_NO_DATA              3
#define STATE_NO_GREEN_LOST_RIGHT  4
#define STATE_NO_GREEN_LOST_LEFT   5


/* Valid Data states */
#define CC_INVALID_DATA      0
#define CC_VALID_DATA        1

/****************************** STRUCTURES ************************************/

/************************* FUNCTION PROTOTYPES ********************************/

extern UINT8 cc_set_position(UINT8);
extern UINT8 cc_center_and_look(UINT8);
extern UINT8 cc_start_moving(void);
extern UINT8 cc_set_track_object(UINT8);
extern void cc_set_camera_pos(UINT8);
extern UINT8 cc_get_dist_hdg(DistHdgType *);
extern UINT8 cc_get_shooter_speed(UINT16 *);
extern UINT8 cc_get_encoder_vals(EncoderValsType *);
extern UINT8 read_cc(unsigned char *, UINT8, UINT8, UINT8, UINT16);

#endif /* __ws_cc_h_ */

