#include "typedefs.h"
#include "MPC5604B_M07N.h"
#include "comms.h"

void task_1msec(void);
void task_2msec(void);
void task_5msec(void);
void task_10msec(void);
void task_40msec(void);
void task_100msec(void);
void task_500msec(void);
void task_1000msec(void);

void Pit1ISR(void);
void SwIrq4ISR(void);

struct CAR_CAL 
{
	float servGain;
	float servGainIgain;
	float servGainDTerm;
	float setupTrack;
	float setupWheelBase;
	float servoArm;
	float steeringNuckle;
	float maxAccel;

	S16 errorTol;
	U16 maxSpeed;
	U16 lostSpeed;
	
};

struct CAR_CTRL {
float iTerm;
float dterm;  
float radiusTarget;
S16 targetServoPos;
S16 targetVelocity;
S16 error;
S16 biasVelocity;
S16 errorRate;
S16 servoDegTarget;
U16 velTarget;
U8 center;
U8 c1;
U8 c2;
U8 c3;
U8 c4;
} ;

struct CAR {
struct CAR_CTRL ctrl;
};