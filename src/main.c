#include "MPC5604B_M07N.h"
#include "typedefs.h"
#include "main.h"
#include "dCamera_Lineal.h"

struct LogStruct loger;
struct CAR car;

U16 taskCTR_5msec = 0;
U16 taskCTR_10msec = 0;
U16 taskCTR_20msec = 0;
U16 taskCTR_40msec = 0;
U16 taskCTR_1000msec = 0;


U8 breakLearn = 0;
U8 apexEntry = 0;
S16 apexEntryIn=0;
S16 apexExitDown=0;

const struct CAR_CAL cal = 
{
/*
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
	U8  sensorMinDynRange;
	S8  sensorMaxError;
	U8 apexModError
		U16 maxLearn;
*/
	6.5,0.021,0.8,
	
	190,270,
	
	25,	20,	0.65, 
	
	100, 
	
	4,	680,	450,
	
	5,	30,
	7,	21,
	
	35,20,60,8,140
};


//**function Prototypes*//


int main(void) 
{
	car.ctrl.biasVelocity = constBiasCenter;
	car.ctrl.targetServoPos = 0;
	car.ctrl.targetVelocity = 0;
	car.ctrl.manualMode = 0;
	
	car.adjust.adjPgain = 0;
	car.adjust.adjIgain = 0;
	car.adjust.adjDgain = 0;
	car.adjust.adjSpeedgain = 0;
	car.sensor.teachDone=0;
	
	car.TeachSensorMinDynRange = cal.sensorMinDynRange;
	car.TeachSenseWidthMin = cal.senseWidthMin;
	car.TeachSenseWidthMax = cal.senseWidthMax;
	
	
	
	//Set the active Gains
	car.pGain =  cal.servGain + car.adjust.adjPgain;
	car.iGain =  cal.servGainIgain + car.adjust.adjIgain;
	car.dGain =  cal.servGainDTerm + car.adjust.adjDgain;
	car.speedGain =  cal.maxSpeed + car.adjust.adjSpeedgain;
	car.ctrl.straightLearn = 0;

	initMainHardware();
  
  setupBiasTable(); // sets lookup for differental neutral map


  for (;;) 
  {
  }

}

void taskPIDupdate()
{	
	if(car.ctrl.manualMode==2)	//auto pilot Mode ==2
		{	
		setPIDLoop();
		setStraightLearn();		
		setApexTargeting();
		setSpeedTargets();		
		}
	else if(car.ctrl.manualMode==0) // manual Mode
		{
		SIU.PGPDO[0].R = 0x00000000;
							
		setPIDLoop();
		car.ctrl.controlCenter = 64;
		car.ctrl.targetVelocity = 0;	
		car.ctrl.biasVelocity = constBiasCenter;		
		}			   
}

void setPIDLoop()
{
	//set the steering position, iTerm
				
	car.ctrl.iTerm += (float)car.ctrl.error * car.iGain;
		
	//Limit the iTerm
	if(car.ctrl.iTerm > cal.servGainIgainLimit)
		car.ctrl.iTerm = cal.servGainIgainLimit;
	else if(car.ctrl.iTerm < -cal.servGainIgainLimit)
		car.ctrl.iTerm = -cal.servGainIgainLimit;
		
	
	if(apexExitDown<180)
		car.ctrl.dterm = (car.sensor.c2 - car.sensor.center) * car.dGain;
	else
		car.ctrl.dterm = 0;
	
		
	//set the position, P, and I term only here
	car.ctrl.targetServoPos = (S16)((float)car.ctrl.error*car.pGain + car.ctrl.iTerm + car.ctrl.dterm);
			
	//limit servo position
	if(car.ctrl.targetServoPos<-constServoMax)
		car.ctrl.targetServoPos = -constServoMax;
	else if(car.ctrl.targetServoPos>constServoMax)
		car.ctrl.targetServoPos = constServoMax;
		
}

void setStraightLearn()
{
	//straightLearning
	if((car.sensor.valid == camCenter) && (car.ctrl.targetServoPos > -50) && car.ctrl.targetServoPos < 50) //if steering is somewhat straight
		{
		if(car.ctrl.straightLearn < cal.maxLearn )
			car.ctrl.straightLearn += 1;
		else
			car.ctrl.straightLearn = cal.maxLearn;	
		}
	else
		{
		if(car.ctrl.straightLearn >15) // can learn down to null in 0.11 seconds
			car.ctrl.straightLearn -= 15;
		else
			car.ctrl.straightLearn = 0;
			
		if(car.ctrl.straightLearn >100)
			breakLearn = 5;
		}
}

void setApexTargeting(void)
{
	//apex set
	if((car.ctrl.targetServoPos>200) && apexExitDown == 0)
		{
		apexEntryIn = 100;
		apexExitDown = 100;
		apexEntry = 1;
		}
	else if((car.ctrl.targetServoPos<-200) && apexExitDown == 0)
		{
		apexEntryIn = 100;
		apexExitDown = 100;
		apexEntry = 0;
	}

	//set the control center Apex seeking portion
	if(apexEntryIn>0)
		{
		if(apexEntry==1)
			car.ctrl.controlCenter = (U8)((S16)64 + (S16)((100-apexEntryIn) * (S16)cal.apexModError / 128));
		else
			car.ctrl.controlCenter = (U8)((S16)64 - (S16)((100-apexEntryIn) * (S16)cal.apexModError / 128));
		apexEntryIn--;
	}
	else if(apexExitDown>0)
		{
		if(apexEntry==1)
			car.ctrl.controlCenter = (U8)((S16)64 + (S16)(apexExitDown * (S16)cal.apexModError / 128));
		else
			car.ctrl.controlCenter = (U8)((S16)64 - (S16)(apexExitDown * (S16)cal.apexModError / 128));
		apexExitDown--;
		}
	else
		car.ctrl.controlCenter = (U8)(64);
}

void setSpeedTargets(void)
{
	if(car.sensor.valid >=1) //sensor has a valid read
		{
		//set the target open loop velocity
		car.ctrl.targetVelocity = (U16)car.speedGain + (U16)car.ctrl.straightLearn*3;
		car.ctrl.biasVelocity	= lookupBiasVel(car.ctrl.targetServoPos, car.ctrl.straightLearn );
		}
	else
		{
		car.ctrl.targetVelocity = cal.lostSpeed;			
		car.ctrl.biasVelocity	= lookupBiasVel(car.ctrl.targetServoPos,car.ctrl.straightLearn);	
		}			
}

void task_5msec()
{
 	U32 motLeft;
  	U32 motRight;
 
 	//Smoothen the velocity command target from where the App set it
	car.ctrl.velTarget = (U16)((U32)car.ctrl.velTarget * 3 + car.ctrl.targetVelocity)>>2;
		
	if(car.ctrl.velTarget > 1400) //limit the applied velocity Target
		car.ctrl.velTarget = 1400;
	

	//Apply velocities to left and right based off bias
	if(car.ctrl.biasVelocity>500)
		{
		motLeft = ((U32)car.ctrl.velTarget * 500)/constBiasCenter;
		motRight = ((U32)car.ctrl.velTarget * (constBiasCenter*2-(U32)car.ctrl.biasVelocity))/constBiasCenter;
		}
	else
		{
		motLeft = ((U32)car.ctrl.velTarget * (U32)car.ctrl.biasVelocity)/constBiasCenter;
		motRight = ((U32)car.ctrl.velTarget * 500)/constBiasCenter;	
		}
	
	if(breakLearn>0)
		{
		if(car.ctrl.targetServoPos>100)
			motRight = 0;
		else
			motLeft =0;
		breakLearn--;
		}
	

	vfnSet_Duty_Opwm(6,motLeft);
	vfnSet_Duty_Opwm(7,motRight);
 	vfnSet_Servo(car.ctrl.targetServoPos);
 	
	taskUpdateCameraStart();
 	
	taskCTR_5msec=0;
}

void task_10msec()
{	
 	taskCTR_10msec=0;
}


void task_20msec()
{
	//Logging Task
	if(car.ctrl.manualMode == 2)
	{
		if(car.logPacketIndex<logSamples)
		{		
		//This selectively memcopies some items on the structs for Atlas
		memcpy(&loger.packet[car.logPacketIndex],&car.ctrl.iTerm,21);
		memcpy(&loger.packet[car.logPacketIndex].data[21],&car.sensor.threshold,11);
		}
		car.logPacketIndex += 1;
	}		
	taskCTR_20msec=0;
}


void task_40msec()
{
	U16 tempADC;
	
	//Check to see if Auto Mode was enabled (SW4)
	if((SIU.PGPDI[2].R & 0x10000000) == 0x0000000)
	{
		SIU.PGPDO[0].R = 0x0000C000;  //Enable Motors
		
		
		//Set the active Gains
		car.pGain =  cal.servGain + car.adjust.adjPgain;
		car.iGain =  cal.servGainIgain + car.adjust.adjIgain;
		car.dGain =  cal.servGainDTerm + car.adjust.adjDgain;
		car.speedGain =  cal.maxSpeed + car.adjust.adjSpeedgain;
		 
		car.ctrl.autoTimer = 0;
		car.ctrl.manualMode = 2;
		car.ctrl.iTerm=0;
		car.ctrl.dterm=0;car.logPacketIndex=0;
	}

	//SWITCH 1 is on for Mod Mode
	if((SIU.PGPDI[2].R & 0x80000000) == 0x0000000)
	{
	
			//ADC pot
			ADC.NCMR[0].R = 0x00000010;      	/* Select ANS8:2 inputs for normal conversion */
			ADC.MCR.B.NSTART = 1;   	
			while(	ADC.MCR.B.NSTART ==1)
			{			
			};
			tempADC = (U16)(ADC.CDR[4].B.CDATA);
			ADC.NCMR[0].R = 0x00000001;      	/* Select ANP1:2 inputs for normal conversion */
		
			if(SIU.GPDI[102].R== 0x01) // P gain Mod
			{	
			car.adjust.adjPgain = ((float)tempADC - 512) * cal.servGain / 512;
				car.pGain =  cal.servGain + car.adjust.adjPgain;
			}
			else if(SIU.GPDI[103].R== 0x01) // P gain Mod
			{	
			car.adjust.adjIgain = ((float)tempADC - 512) * cal.servGainIgain / 512;
				car.iGain =  cal.servGainIgain + car.adjust.adjIgain;
			}
			else if(SIU.GPDI[104].R== 0x01) // P gain Mod
			{	
			car.adjust.adjDgain = ((float)tempADC - 512) * cal.servGainDTerm / 512;
			
		car.dGain =  cal.servGainDTerm + car.adjust.adjDgain;
			}
			else if(SIU.GPDI[105].R== 0x01) // P gain Mod
			{	
			car.adjust.adjSpeedgain = ((float)tempADC - 512) * cal.maxSpeed / 512;			
		car.speedGain =  cal.maxSpeed + car.adjust.adjSpeedgain;
			}
		
	
		if(tempADC<0x1E0)
		{
			SIU.GPDO[68].R = 0;
			SIU.GPDO[69].R = 0;
			SIU.GPDO[70].R = 0;
			SIU.GPDO[71].R = 0;
			if(tempADC<0x140)
				SIU.GPDO[71].R = 1;
			if(tempADC<0x100)
				SIU.GPDO[70].R = 1;
			if(tempADC<0x50)
				SIU.GPDO[69].R = 1;
			if(tempADC<0x10)
				SIU.GPDO[68].R = 1;
			
		}
		else if(tempADC>0x220)
		{			
		
			SIU.GPDO[68].R = 0;
			SIU.GPDO[69].R = 0;
			SIU.GPDO[70].R = 0;
			SIU.GPDO[71].R = 0;
			if(tempADC>0x280)
				SIU.GPDO[68].R = 1;
			if(tempADC>0x300)
				SIU.GPDO[69].R = 1;
			if(tempADC>0x380)
				SIU.GPDO[70].R = 1;
			if(tempADC>0x3C0)
				SIU.GPDO[71].R = 1;
		}
		else	
		{
			SIU.GPDO[68].R = 0;
			SIU.GPDO[69].R = 1;
			SIU.GPDO[70].R = 1;
			SIU.GPDO[71].R = 0;
		}
		

	
	
	
	}
	else 
	{//use this leg for LED control of sensor Status
	
	
	if(car.sensor.valid>0)
		SIU.GPDO[68].R = 0;
	else
	SIU.GPDO[68].R = 1;
	if(car.ctrl.error<2 & car.ctrl.error>-2)
		SIU.GPDO[69].R = 0;
	else
		SIU.GPDO[69].R = 1;
	
	if(car.sensor.width>car.TeachSenseWidthMin & car.sensor.width < car.TeachSenseWidthMax)
		SIU.GPDO[70].R = 0;
	else
		SIU.GPDO[70].R = 1;
	
	if(car.sensor.threshold > car.TeachSensorMinDynRange)
		SIU.GPDO[71].R = 0;
	else
		SIU.GPDO[71].R = 1;
	}
	

	taskCTR_40msec=0;
}




void task_1000msec()
{
	if(car.ctrl.manualMode==2)
	{
		car.ctrl.autoTimer++;
		if(car.ctrl.autoTimer >= cal.runTime)//shut off controls on timeout
		{			
			car.ctrl.manualMode=0;	
			SIU.PGPDO[0].R = 0x00000000;
		}
	}	
	
	taskCTR_1000msec=0;
}


void Delay(void)
{
U8 dly;
  for(dly=0;dly<40;dly++)
  {  	
  };
}




void Pit1ISR(void) 
{
	if(taskCTR_5msec++>=10) task_5msec();
	if(taskCTR_10msec++>=20) task_10msec();	
	if(taskCTR_20msec++>=40) task_20msec();
	if(taskCTR_40msec++>=80) task_40msec();
	if(taskCTR_1000msec++>=2000) task_1000msec();

	PIT.CH[1].TFLG.B.TIF = 1;    /* MPC56xxB/P/S: Clear PIT 1 flag by writing 1 */
}


void Pit2ISR(void) 
{
	taskUpdateCamera();
	PIT.CH[2].TFLG.B.TIF = 1;    /* MPC56xxB/P/S: Clear PIT 1 flag by writing 1 */
}

void setupBiasTable()
{
	S8 i = 41;
	float res;
	float rad;

	
	while(i<=81)
		{
		rad = (float)(i-40)*10;
		res = -0.0016f*rad*rad - 0.1564f*rad + 493.18f;
		car.adjust.biasVelTable[i] = res;
		i++;
		}
	
	
	i = 39;
	while(i>=0)
		{
		rad = -(float)(i-40)*10;
		res = 1000-(-0.0016f*rad*rad - 0.1564f * rad + 493.18f);
		car.adjust.biasVelTable[i] = (S16)res;	
		i--;
		}
	
	car.adjust.biasVelTable[40] = 500;
}

S16 lookupBiasVel(S16 pwmTarget, U16 speedlearn)
{
	S16 ret = 0;
	S8 index = (S8)((S8)(pwmTarget/10)+40);
	
	if(index<0) index = 0;
	if(index>80) index = 80;
	ret = car.adjust.biasVelTable[(U8)index];
	if(speedlearn>40)
		{
		if(index<40)
			ret = ret*4/3;
		
		else if(index>40)
			ret = ret*2/3;
		}

	return ret;	
}

