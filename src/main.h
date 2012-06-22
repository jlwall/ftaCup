#include "typedefs.h"
#include "MPC5604B_M07N.h"
#include "comms.h"


#define constBiasCenter 500
#define constServoMax 350
#define constServoMiddle 1500

#define logSamples 300

void task_2msec(void);
void task_5msec(void);
void task_10msec(void);
void task_40msec(void);
void task_1000msec(void);
void taskPIDupdate(void);

void setPIDLoop(void);
void setStraightLearn(void);
void setApexTargeting(void);
void setSpeedTargets(void);

void Pit1ISR(void);
void Pit2ISR(void);
void setupBiasTable(void);
S16 lookupBiasVel(S16 pwmTarget, U16 speedlearn);

struct CAR_ADJUST
{
	
	float adjPgain;
	float adjIgain;
	float adjDgain;
	float adjSpeedgain;
	S16 biasVelTable[81]; //40 is neutral
	
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
	U8  senseWidthMinDer;
	U8  senseWidthMaxDer;
	
	U8  runTime;
	U16  sensorMinDynRange;
	S8  sensorMaxError;
	U8 apexModError;
	U16 maxLearn;
	
};

struct LIGHT_SENSOR
{
	U16  array[128];
	U16  arrayTeach[128];
	U16  threshold;
	U16  valMax;
	U16  valMin;
	U8	cornLeft;
	U8	cornRight;
	U8 	width;
	U8  center;
	U8  c1;
	U8  c2;
	U8  c3;
	U8  c4;
	U8  valid;
	U8 teachDone;
	U16  valMaxTemp;
	U16  valMinTemp;
	U8 	errorCounter;
		
}; //16 bytes to log

struct CAR_CTRL {
float iTerm;
float dterm;  
S16 targetServoPos;
U16 targetVelocity;
S16 biasVelocity;
S16 servoDegTarget;
U16 velTarget;
S16 error;
U16 straightLearn;
S8 autoTimer;
U8 manualMode;
U8  controlCenter;
} ; //24  bytes to log

struct CAR {
struct CAR_CTRL ctrl;
struct LIGHT_SENSOR sensor;
struct CAR_ADJUST adjust;

float pGain;
float iGain;
float dGain;
float speedGain;
U8  TeachSenseWidthMin;
U8  TeachSenseWidthMax;
U16  TeachSensorMinDynRange;
U16 logPacketIndex;
};

struct LogPacket
{
	U8 data[40];
};

struct LogStruct
{
	struct LogPacket packet[logSamples];
};