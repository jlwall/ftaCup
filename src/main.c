#include "MPC5604B_M07N.h"


#include "typedefs.h"
#include "main.h"

struct LogStruct loger;

vuint32_t i,j;                					/* Dummy idle counter */

volatile uint32_t dly,lly,chw,adcdata,curdata;

int16_t posr,posl;
float posp,posi,poserr,posd,last_poserr[10]; 

float kp=0.2,kd=0.4,ki=0.05,p;

S16 testServo = -350;

U16 taskCTR_2msec = 0;
U16 taskCTR_5msec = 0;
U16 taskCTR_10msec = 0;
U16 taskCTR_20msec = 0;
U16 taskCTR_40msec = 0;
U16 taskCTR_100msec = 0;
U16 taskCTR_500msec = 0;
U16 taskCTR_1000msec = 0;
struct CAR car;
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
	7.3,0.026,0,
	
	190,270,
	
	25,	20,	0.65, 
	
	100, 
	
	4,	620,	400,
	
	5,	30,
	7,	21,
	
	35,20,60,2,140
};


    uint32_t Pit1Ctr = 0;   /* Counter for PIT 1 interrupts */
    uint32_t SWirq4Ctr = 0;	/* Counter for software interrupt 4 */

//**function Prototypes*//

#define FLASH_CONFIG_DATA 0x1084126F /* MPC56xxS flash config value for 64 MHz */
#define FLASH_CONFIG_REG CFLASH.PFCR0.R /* Flash config. register address */
#define FLASH_ACCESS_PROT_DATA 0x03F2005D /* MPC56xxS flash access prot. value */
#define FLASH_ACCESS_PROT_REG CFLASH.FAPR.R /* Flash Access Prot. Reg. address */
asm void enable_accel_BTB(void) {
li r0, 0 /* Enable branch acceleration (HID[PBRED]=0) */
mtHID0 r0
li r0, 0x0201 /* Invalidate Branch Target Buffers and enable them */
mtBUCSR r0
}

int main(void) 
{
	volatile int i = 0;
	char option = 0;
	
	
	/* NOTE: Structures are default aligned on a boundary which is a multiple of */
/* the largest sized element, 4 bytes in this case. The first two */
/* instructions are 4 bytes, so the last instruction is duplicated to */
/* avoid the compiler adding padding of 2 bytes before the instruction. */
uint32_t mem_write_code_vle [] = {
0x54640000, /* e_stw r3,(0)r4 machine code: writes r3 contents to addr in r4 */
0x7C0006AC, /* mbar machine code: ensure prior store completed */
0x00040004 /* 2 se_blr's machine code: branches to return address in link reg.*/
};
typedef void (*mem_write_code_ptr_t)(uint32_t, uint32_t);
/* create a new type def: a func pointer called mem_write_code_ptr_t */
/* which does not return a value (void) */
/* and will pass two 32 bit unsigned integer values */
/* (per EABI, the first parameter will be in r3, the second r4) */
(*((mem_write_code_ptr_t)mem_write_code_vle)) /* cast mem_write_code as func ptr*/
/* * de-references func ptr, i.e. converts to func*/
(FLASH_CONFIG_DATA, /* which passes integer (in r3) */
(uint32_t)&FLASH_CONFIG_REG); /* and address to write integer (in r4) */
(*((mem_write_code_ptr_t)mem_write_code_vle)) /* cast mem_write_code as func ptr*/
/* * de-references func ptr, i.e. converts to func*/
(FLASH_ACCESS_PROT_DATA, /* which passes integer (in r3) */
(uint32_t)&FLASH_ACCESS_PROT_REG); /* and address to write integer (in r4) */
enable_accel_BTB(); /* Enable branch accel., branch target buffers */

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


  for (;;) {
  }

}



void task_2msec()
{


	taskCTR_2msec=0;
}

void taskPIDupdate()
{	
	if(car.ctrl.manualMode==2)	//auto pilot Mode ==2
	{	
		//set the steering position, iTerm
	//	if((car.ctrl.error > (cal.errorTol/2)) || (car.ctrl.error < -(cal.errorTol/2)))
			{					
			car.ctrl.iTerm += (float)car.ctrl.error * car.iGain;
			
			//Limit the iTerm
			if(car.ctrl.iTerm > cal.servGainIgainLimit)
				car.ctrl.iTerm = cal.servGainIgainLimit;
			else if(car.ctrl.iTerm < -cal.servGainIgainLimit)
				car.ctrl.iTerm = -cal.servGainIgainLimit;
			}
		
			//apply the dTerm
		//	car.ctrl.dterm = (car.sensor.center - car.sensor.c2) * car.dGain;
		
			
			//set the position, P, and I term only here
			car.ctrl.targetServoPos = (S16)((float)car.ctrl.error*car.pGain + car.ctrl.iTerm);// +car.ctrl.dterm);
			//car.ctrl.targetServoPos = (S16)((float)car.ctrl.error*car.pGain);
		
			//limit servo position
			if(car.ctrl.targetServoPos<-constServoMax)
				car.ctrl.targetServoPos = -constServoMax;
			else if(car.ctrl.targetServoPos>constServoMax)
				car.ctrl.targetServoPos = constServoMax;
			
			//straightLearning
			if((car.ctrl.targetServoPos > -50) && car.ctrl.targetServoPos < 50) //if steering is somewhat straight
			{
			if(car.ctrl.straightLearn < cal.maxLearn ) //can learn +100 in 1 second
				car.ctrl.straightLearn += 1;
			else
				car.ctrl.straightLearn = cal.maxLearn;	
			}
			else
			{
				if(car.ctrl.straightLearn >20) // can learn down to null in 0.11 seconds
				{
					
					car.ctrl.straightLearn -= 20;
				}
				else
					car.ctrl.straightLearn = 0;
			}
			
			
			//set the control center Apex seeking portion
			car.ctrl.controlCenter = (U8)(64 + car.ctrl.targetServoPos * (S16)cal.apexModError / constServoMax);		
			//car.ctrl.controlCenter = 64;
	
		if(car.sensor.valid >=1) //sensor has a valid read
		{
			
			//set the target open loop velocity
			car.ctrl.targetVelocity = (U16)car.speedGain + (U16)car.ctrl.straightLearn*2;
			

			//aditional speed damping for turning events
			if(car.ctrl.targetServoPos>100)
				car.ctrl.targetVelocity = car.ctrl.targetVelocity * 4 / 5;
			else if(car.ctrl.targetServoPos<-100)
				car.ctrl.targetVelocity = car.ctrl.targetVelocity * 4 / 5;

			car.ctrl.biasVelocity	= lookupBiasVel(car.ctrl.targetServoPos );
		}
		else
		{
			car.ctrl.targetVelocity = cal.lostSpeed;			
			car.ctrl.biasVelocity	= lookupBiasVel(car.ctrl.targetServoPos );	
		}		
		
	}
	else if(car.ctrl.manualMode==0) // manual Mode
	{
		SIU.PGPDO[0].R = 0x00000000;
		car.ctrl.targetVelocity = 0;	
		car.ctrl.biasVelocity = constBiasCenter;	
		
		
		if((car.ctrl.error > (cal.errorTol/2)) || (car.ctrl.error < -(cal.errorTol/2)))
		{						
			car.ctrl.iTerm += (float)car.ctrl.error * car.iGain;
			
			//Limit the iTerm
			if(car.ctrl.iTerm > cal.servGainIgainLimit)
				car.ctrl.iTerm = cal.servGainIgainLimit;
			else if(car.ctrl.iTerm < -cal.servGainIgainLimit)
				car.ctrl.iTerm = -cal.servGainIgainLimit;
		}
		
		//car.ctrl.dterm = (car.sensor.center - car.sensor.c2) * car.ctrl.dGain;
		
		
		//set the position, P, and I term only here
		//car.ctrl.targetServoPos = (S16)((float)car.ctrl.error*car.ctrl.pGain + car.ctrl.iTerm + car.ctrl.dterm);
	//	if(testServo < 350)
	//		car.ctrl.targetServoPos = testServo++;
	//	else
		car.ctrl.targetServoPos = (S16)(float)car.ctrl.error*car.pGain;
		
		//set the position, P, and I term only here
		if(car.ctrl.targetServoPos<-constServoMax)
			car.ctrl.targetServoPos = -constServoMax;
		else if(car.ctrl.targetServoPos>constServoMax)
			car.ctrl.targetServoPos = constServoMax;
		
	
		
		car.ctrl.controlCenter = 64;
	
	}			   
}

void task_5msec()
{
 	U32 motLeft;
  	U32 motRight;
 
 	//Smoothen the velocity command target from where the App set it
	car.ctrl.velTarget = (U16)((U32)car.ctrl.velTarget * 7 + car.ctrl.targetVelocity)>>3;
		
	if(car.ctrl.velTarget > 1400) //limit the applied velocity Target
		car.ctrl.velTarget = 1400;
	

	//Apply velocities to left and right based off bias
	motLeft = ((U32)car.ctrl.velTarget * (U32)car.ctrl.biasVelocity)/constBiasCenter;
	motRight = ((U32)car.ctrl.velTarget * (constBiasCenter*2-(U32)car.ctrl.biasVelocity))/constBiasCenter;

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

/*	if(car.ctrl.manualMode == 2)
	{
		if(car.logPacketIndex<logSamples)
		{		
		memcpy(&loger.packet[car.logPacketIndex],&car.ctrl.iTerm,21);
		memcpy(&loger.packet[car.logPacketIndex].data[21],&car.sensor.threshold,11);
		}
		car.logPacketIndex += 1;
	}*/
		
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
		 
		car.ctrl.autoTimer = cal.runTime;
		car.ctrl.manualMode = 2;
		car.ctrl.iTerm=0;
		car.ctrl.dterm=0;
		loger.packet[0].data[0]=0;
	//	if(loger.packet[499].data[39]==0)
	//	loger.packet[499].data[39]=0xff;
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
	if(car.ctrl.autoTimer>=0)
	{
		car.ctrl.autoTimer--;
		if((car.ctrl.autoTimer==0)&car.ctrl.manualMode==2)
		{			
			car.ctrl.manualMode=0;	
			SIU.PGPDO[0].R = 0x00000000;
		}
	}	
	
	taskCTR_1000msec=0;
}


void Delay(void)
{
  for(dly=0;dly<40;dly++)
  {  	
  };
}




void Pit1ISR(void) 
{
	Pit1Ctr++;              /* Increment interrupt counter */
  //	if ((Pit1Ctr & 1)==0)
   //		{ /* If PIT1Ctr is even*/
   // 	INTC.SSCIR[4].R = 2;      /*  then nvoke software interrupt 4 */
  	//	}
 
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


void SwIrq4ISR(void)
{
	SWirq4Ctr++;		 		/* Increment interrupt counter */
	

	
  	INTC.SSCIR[4].R = 1;		/* Clear channel's flag */  
}

void setupBiasTable()
{
	S8 i = 41;
	float res;
	float rad;

	
	while(i<=81)
	{
	rad = (float)(i-40)*10;
	res = -0.0008f*rad*rad - 0.1764f*rad + 493.18f;
	car.adjust.biasVelTable[i] = res;
	
	
	
		i++;
	}
	
	
	i = 39;
	while(i>=0)
	{
	rad = -(float)(i-40)*10;
	res = 1000-(-0.0008f*rad*rad - 0.1764f * rad + 493.18f);
	car.adjust.biasVelTable[i] = (S16)res;
	
	
	
		i--;
	}
	
	car.adjust.biasVelTable[40] = 500;
}

S16 lookupBiasVel(S16 pwmTarget)
{
	S8 index = (S8)((S8)(pwmTarget/10)+40);
	
	if(index<0) index = 0;
	if(index>80) index = 80;
	return car.adjust.biasVelTable[(U8)index];
	
}






