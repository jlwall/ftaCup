#include "typedefs.h"
#include "MPC5604B_M07N.h"
#include "comms.h"


#define constBiasCenter 500
#define constServoMax 350
#define constServoMiddle 1500

void task_1msec(void);
void task_2msec(void);
void task_5msec(void);
void task_10msec(void);
void task_40msec(void);
void task_100msec(void);
void task_500msec(void);
void task_1000msec(void);

void Pit1ISR(void);
void Pit2ISR(void);
void SwIrq4ISR(void);

struct CAR_ADJUST
{
	
	float adjPgain;
	float adjIgain;
	float adjDgain;
	float adjSpeedgain;
	
};

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
	
	float servGainIgainLimit;

	S16 errorTol;
	U16 maxSpeed;
	U16 lostSpeed;
	
	U8  senseWidthMin;
	U8  senseWidthMax;
	U8  runTime;
	U16  sensorMinDynRange;
	S8  sensorMaxError;
	U8 apexModError;
	
};

struct LIGHT_SENSOR
{
	U16  array[128];
	S16  arrayDerive[128];
	U8	cornLeft;
	U8	cornRight;
	U8	deriveCornLeft;
	U8	deriveCornRight;
	U8 	width;
	U8  center;
	U8  c1;
	U8  c2;
	U8  c3;
	U8  c4;
	U8  valid;
	U16  threshold;
	U16  valMax;
	U16  valMin;
	S16  deriveValMax;
	S16  deriveValMin;
	
	
	
};

struct CAR_CTRL {

float pGain;
float iGain;
float dGain;
float speedGain;

float iTerm;
float dterm;  
float radiusTarget;
S16 targetServoPos;
U16 targetVelocity;
S16 biasVelocity;
S16 errorRate;
S16 servoDegTarget;
U16 velTarget;
S16 error;
U8 autoTimer;
U8 manualMode;

	U8  controlCenter;
} ;

struct CAR {
struct CAR_CTRL ctrl;
struct LIGHT_SENSOR sensor;
struct CAR_ADJUST adjust;
};