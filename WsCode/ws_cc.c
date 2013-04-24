/*******************************************************************************
* FILE NAME: ws_cc.c
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
#include "ws_includes.h"
#include "user_routines.h"
#include "delays.h"       /*defined locally*/
#include "ws_io.h"
#include "serial_ports.h"
#include "ws_cc.h"


/* Use these defines to dump debug info from the CC in input_data */
/*
#define DEBUG_DIST_HDG
*/
/*
#define DEBUG_ENCODER_VALS
*/
/*
 */
#define PRINT_CC_ERRORS


/*******************************************************************************
* FUNCTION NAME: cc_set_position
* PURPOSE:       Tell the CC the current position of the robot.
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
UINT8 cc_set_position(UINT8 starting_position_id)
{
  CcReturnType cc_ret_val = CC_SUCCESS;
  UINT8        ack_data[CC_RESP_STARTING_POS_SIZE];

  if (read_cc(&(ack_data[0]), CC_CMD_SET_POSITION, starting_position_id,
              CC_RESP_STARTING_POS_SIZE, CC_LOOP_CNT_TIMEOUT) <
      CC_RESP_STARTING_POS_SIZE)
  {
    /* No response from the CC */
    cc_ret_val = CC_TIMEOUT;
#ifdef PRINT_CC_ERRORS
    printf("CC TO-SetPos ");
#endif
  }
  else
  {
    /*
    printf("pos_data=%X ", (int)ack_data[0]);
    */
    /* ack_data is |cmd|, verify the data */
    if (ack_data[0] != CC_ACK_STARTING_POS)
    {
      cc_ret_val = CC_FAIL;
    }
  }

  return (cc_ret_val);
}


/*******************************************************************************
* FUNCTION NAME: cc_center_and_look
* PURPOSE:       Tell the CC to point forward & look for green
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
UINT8 cc_center_and_look(UINT8 pos)
{
  CcReturnType cc_ret_val = CC_SUCCESS;
  UINT8        ack_data[CC_RESP_CENTER_AND_LOOK_SIZE];
  UINT8        cmd;

  if (pos == LOOK_HIGH)
  {
    cmd = CC_CMD_CENTER_AND_LOOK_HIGH;
  }
  else
  {
    cmd = CC_CMD_CENTER_AND_LOOK;
  }

  if (read_cc(&(ack_data[0]), cmd, 0,
              CC_RESP_CENTER_AND_LOOK_SIZE, CC_LOOP_CNT_TIMEOUT) <
      CC_RESP_CENTER_AND_LOOK_SIZE)
  {
    /* No response from the CC */
    cc_ret_val = CC_TIMEOUT;
#ifdef PRINT_CC_ERRORS
    printf("CC TO-Center ");
#endif
  }
  else
  {
    /*
    printf("ack=%X ", (int)ack_data[0]);
    */
    /* ack_data is |cmd|, verify the data */
    if (ack_data[0] != CC_ACK_CENTER_AND_LOOK)
    {
      cc_ret_val = CC_FAIL;
#ifdef PRINT_CC_ERRORS
      printf("CC BadAck Center %d\r", ack_data[0]);
#endif
    }
  }

  return (cc_ret_val);
}


/*******************************************************************************
* FUNCTION NAME: cc_start_moving
* PURPOSE:       Tell the CC to start moving to follow green
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
UINT8 cc_start_moving(void)
{
  CcReturnType cc_ret_val = CC_SUCCESS;
  UINT8        ack_data[CC_RESP_START_MOVING_SIZE];

  if (read_cc(&(ack_data[0]), CC_CMD_START_MOVING, 0,
              CC_RESP_START_MOVING_SIZE, CC_LOOP_CNT_TIMEOUT) <
      CC_RESP_START_MOVING_SIZE)
  {
    /* No response from the CC */
    cc_ret_val = CC_TIMEOUT;
#ifdef PRINT_CC_ERRORS
    printf("CC TO-Move ");
#endif
  }
  else
  {
    /*
    printf("ack=%X ", (int)ack_data[0]);
    */
    /* ack_data is |cmd|, verify the data */
    if (ack_data[0] != CC_ACK_START_MOVING)
    {
      cc_ret_val = CC_FAIL;
#ifdef PRINT_CC_ERRORS
      printf("CC BadAck Move %d\r", ack_data[0]);
#endif
    }
  }

  return (cc_ret_val);
}


/*******************************************************************************
* FUNCTION NAME: cc_set_track_object
* PURPOSE:       Tell the CC the current position of the robot.
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
UINT8 cc_set_track_object(UINT8 object_id)
{
  CcReturnType   cc_ret_val = CC_SUCCESS;
  unsigned char  ack_data[CC_RESP_TRACK_OBJECT_SIZE];

  UINT8 res;

  res = read_cc(&(ack_data[0]), CC_CMD_SET_TRACK_OBJECT, object_id,
                CC_RESP_TRACK_OBJECT_SIZE, CC_LOOP_CNT_TIMEOUT);

  if (res < CC_RESP_TRACK_OBJECT_SIZE)
  {
    /* No response from the CC */
    cc_ret_val = CC_TIMEOUT;
#ifdef PRINT_CC_ERRORS
    printf("CC TO-TrackObj ");
#endif
  }
  else
  {
    /*
    printf("obj_data=%02X ", (int)ack_data[0]);
    */
    /* ack_data is |cmd|, verify the data */
    if (ack_data[0] != CC_ACK_TRACK_OBJECT)
    {
      cc_ret_val = CC_FAIL;
    }
  }

  return (cc_ret_val);
}


/*******************************************************************************
* FUNCTION NAME: cc_set_camera_pos
* PURPOSE:       Tell the CC to set the camera to a pre-defined position
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
void cc_set_camera_pos(UINT8 cmd)
{
  Write_Serial_Port_Two(cmd);

  return;
}


/*******************************************************************************
* FUNCTION NAME: cc_get_dist_hdg
* PURPOSE:       Read the distance & heading to the object from the CC and
*                the robot's orientation
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
UINT8 cc_get_dist_hdg(DistHdgType *p_curr_dist_hdg)
{
  CcReturnType cc_ret_val = CC_SUCCESS;
  UINT8        dist_hdg_data[CC_RESP_DIST_HDG_SIZE];

  /* request Distance & Heading to target */
  /*printf("Send1 ");*/
  if (read_cc(&(dist_hdg_data[0]), CC_CMD_REQ_DIST_HDG, 0,
              CC_RESP_DIST_HDG_SIZE, CC_LOOP_CNT_TIMEOUT) <
      CC_RESP_DIST_HDG_SIZE)
  {
    /* No response from the CC...tell the auton code not to run */
    cc_ret_val = CC_TIMEOUT;
    p_curr_dist_hdg->status = STATE_NO_GREEN;
    Dig_out_see_green = 0;
#ifdef PRINT_CC_ERRORS
    printf("CC TO-DistHdg ");
#endif
  }
  else
  {
    /* dist_hdg_data is |status|Dhigh|Dlow|Hhigh|Hlow|PanPwm|TiltPwm|, break
       out the data */
#ifdef DEBUG_RAW_DIST_HDG
    printf("dist_hdg_data=%02X%02X%02X%02X%02X%02X%02X%02X ",
           dist_hdg_data[0], dist_hdg_data[1], dist_hdg_data[2],
           dist_hdg_data[3], dist_hdg_data[4], dist_hdg_data[5],
           dist_hdg_data[6], dist_hdg_data[7]);
#endif

    /* Status is byte 0 */
    p_curr_dist_hdg->status = dist_hdg_data[0];

    /* Turn on onboard LED on if we see green */
    if(p_curr_dist_hdg->status == STATE_SEE_GREEN)
    {
      Dig_out_see_green = 0;
    }
    else
    {
      Dig_out_see_green = 1;
    }


    /* Dist is byte 1 and 2 */
    p_curr_dist_hdg->dist = dist_hdg_data[2];
    p_curr_dist_hdg->dist = p_curr_dist_hdg->dist +
                            ((int)dist_hdg_data[1] << 8);

    /* Heading is byte 3 and 4 */
    p_curr_dist_hdg->hdg = dist_hdg_data[4];
    p_curr_dist_hdg->hdg = p_curr_dist_hdg->hdg +
                           ((int)dist_hdg_data[3] << 8);

    /* Pan PWM is byte 5 */
    p_curr_dist_hdg->pan_pwm = dist_hdg_data[5];
    motor_vals.cam_servo_pan = p_curr_dist_hdg->pan_pwm;

    /* Tilt PWM is byte 6 */
    p_curr_dist_hdg->tilt_pwm = dist_hdg_data[6];
    motor_vals.cam_servo_tilt = p_curr_dist_hdg->tilt_pwm;

    /* Valid data flag is byte 7 */
    p_curr_dist_hdg->valid_data = dist_hdg_data[7];

    /*
    if (p_curr_dist_hdg->dist > 1000)
    {
      cc_ret_val = CC_TIMEOUT;
      printf("CC BV01 ");
    }
    */
  }

#ifdef DEBUG_DIST_HDG
  printf("S=%u D=%d H=%d P=%u T=%u V=%u\r", p_curr_dist_hdg->status,
         p_curr_dist_hdg->dist, p_curr_dist_hdg->hdg, p_curr_dist_hdg->pan_pwm,
         p_curr_dist_hdg->tilt_pwm, p_curr_dist_hdg->valid_data);
#endif

  return(cc_ret_val);
}


/*******************************************************************************
* FUNCTION NAME: cc_get_shooter_speed
* PURPOSE:       Read the shooter speed from the CC
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
UINT8 cc_get_shooter_speed(UINT16 *p_shooter_speed)
{
  CcReturnType cc_ret_val = CC_SUCCESS;
  UINT8        shooter_speed_data[CC_RESP_SHOOTER_SPEED_SIZE];

  /* request shooter speed */
  if (read_cc(&(shooter_speed_data[0]), CC_CMD_REQ_SHOOTER_SPEED, 0,
              CC_RESP_SHOOTER_SPEED_SIZE, CC_LOOP_CNT_TIMEOUT) <
      CC_RESP_SHOOTER_SPEED_SIZE)
  {
    /* No response from the CC...tell the auton code not to run */
    cc_ret_val = CC_TIMEOUT;
#ifdef PRINT_CC_ERRORS
    printf("CC TO-ShooterSpeed ");
#endif
  }
  else
  {
    /* shooter_speed_data is |Speedhigh|Speedlow|, break out the data */
#ifdef DEBUG_RAW_SHOOTER_SPEED
    printf("shooter_speed_data=%02X%02X ", shooter_speed_data[0],
           shooter_speed_data[1]);
#endif

    User_Byte5 = shooter_speed_data[0];
    User_Byte6 = shooter_speed_data[1];

    /* shooter speed is byte 0 and 1 */
    *p_shooter_speed = shooter_speed_data[1];
    *p_shooter_speed = *p_shooter_speed + ((int)shooter_speed_data[0] << 8);
  }

#ifdef DEBUG_SHOOTER_SPEED
  printf("S=%d\r", *p_shooter_speed);
#endif

  return(cc_ret_val);
}


/*******************************************************************************
* FUNCTION NAME: cc_get_encoder_vals
* PURPOSE:       Read the encoder values from the CC
* CALLED FROM:
* ARGUMENTS:
* RETURNS:
*******************************************************************************/
UINT8 cc_get_encoder_vals(EncoderValsType *p_encoder_vals)
{
  CcReturnType cc_ret_val = CC_SUCCESS;
  UINT8        encoder_vals_data[CC_RESP_ENCODER_VAL_SIZE];

  /* request encoder values */
  if (read_cc(&(encoder_vals_data[0]), CC_CMD_REQ_ENCODER, 0,
              CC_RESP_ENCODER_VAL_SIZE, CC_LOOP_CNT_TIMEOUT) <
      CC_RESP_ENCODER_VAL_SIZE)
  {
    /* No response from the CC...tell the auton code not to run */
    cc_ret_val = CC_TIMEOUT;
#ifdef PRINT_CC_ERRORS
    printf("CC TO-Encoder ");
#endif
  }
  else
  {
    /* encoder_vals_data is |Left|Right|Orient|, break out the data */
#ifdef DEBUG_ENCODER_VALS
    printf("encoder_vals_data=%X%X%X ", (int)encoder_vals_data[0],
           (int)encoder_vals_data[1],(int)encoder_vals_data[2]);
#endif

    /* Left encoder is byte 0 */
    p_encoder_vals->left = encoder_vals_data[0];

    /* Right encoder is byte 1 */
    p_encoder_vals->right = encoder_vals_data[1];

    /* Orient is byte 2 */
    p_encoder_vals->orient = encoder_vals_data[2];
  }

#ifdef DEBUG_ENCODER_VALS
  printf("L=%d R=%d O=%d\r", (int)p_encoder_vals->left,
          (int)p_encoder_vals->right, (int)p_encoder_vals->orient);
#endif

  return(cc_ret_val);
}


/*******************************************************************************
* FUNCTION NAME: read_cc
* PURPOSE:       Read a register on the CC.
* CALLED FROM:
* ARGUMENTS:
*            p_data    - Pointer to the buffer where the contents of the return
*                        data from the CC will be put
*            cmd       - Command byte sent to CC, either setting or requesting
*                        data
*            resp_size - Size of the response message from CC
*            timeout   - Timeout to wait for each byte of the message being
*                        returned
* RETURNS:
*            Number of bytes received from the CC
*******************************************************************************/
UINT8 read_cc(unsigned char* p_data, UINT8 cmd, UINT8 data, UINT8 resp_size,
              UINT16 timeout_per_char)
{
  UINT8  byte_recvd;      /* Was a byte recvd in this loop */
  UINT8  bytes_recvd = 0; /* How many bytes were recvd total */
  UINT8  i;
  UINT16 loop_count;

  /* Clear any extra chars out from the input buffer */
  clear_serial_port_two_rx();

  /* Send the command */
  Write_Serial_Port_Two(cmd);

  /* send out the data if there is any */
  if (data != 0)
  {
    Delay10TCY();
    Write_Serial_Port_Two(data);
  }

  /* Now recv all the expected response data, break out after all the
   * expected bytes are recvd or until we don't recv a byte.
   */
  byte_recvd = 1;   /* Init to 1 so the first loop works */
  for (i = 0; (i < resp_size) && (byte_recvd == 1); i++)
  {
    loop_count = timeout_per_char;
    byte_recvd = 0;

    do
    {
      /* only read from buffer if there is data waiting */
      if (Serial_Port_Two_Byte_Count() > 0)
      {
        /* put data recieved into byte_recvd */
        p_data[i] = Read_Serial_Port_Two();
        byte_recvd = 1;
      }

      loop_count--;
    } while ((byte_recvd == 0) && (loop_count > 0));

    /* Add the byte we recvd to the total count */
    bytes_recvd += byte_recvd;
  }

  return(bytes_recvd);
}

