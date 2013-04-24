#ifndef __ws_autonomous_prog_h_
#define __ws_autonomous_prog_h_

typedef enum
{
  AP1_STATE_DONE
} AutoProg1State;

typedef enum
{
  AP2_STATE_DRIVE1,
  AP2_STATE_ROT1,
  AP2_STATE_DRIVE2,
  AP2_STATE_ROT2,
  AP2_STATE_DRIVE3,
  AP2_STATE_ROT3,
  AP2_STATE_DRIVE4,
  AP2_STATE_DONE
} AutoProg2State;

typedef enum
{
  AP3_STATE_DRIVE1,
  AP3_STATE_TURN1,
  AP3_STATE_DRIVE2,
  AP3_STATE_TURN2,
  AP3_STATE_DRIVE3,
  AP3_STATE_DONE
} AutoProg3State;



#endif /* __ws_autonomous_prog_h_ */
