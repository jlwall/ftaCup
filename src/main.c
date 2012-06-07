#include "MPC5604B_M07N.h"


#include "typedefs.h"
#include "main.h"



vuint32_t i,j;                					/* Dummy idle counter */

volatile uint32_t dly,lly,chw,adcdata,curdata;

int16_t posr,posl;
float posp,posi,poserr,posd,last_poserr[10]; 

float kp=0.2,kd=0.4,ki=0.05,p;


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
	U8  runTime;
	U8  sensorMinDynRange;
		S8  sensorMaxError;
*/
	4.2,0.4,0.5,190,270,25,20,0.65, 100, 5,650,220,15,40,15,12,50
};


    uint32_t Pit1Ctr = 0;   /* Counter for PIT 1 interrupts */
    uint32_t SWirq4Ctr = 0;	/* Counter for software interrupt 4 */

//**function Prototypes*//


int main(void) 
{
	volatile int i = 0;
	char option = 0;

	car.ctrl.biasVelocity = constBiasCenter;
	car.ctrl.targetServoPos = 0;
	car.ctrl.targetVelocity = 0;
	car.ctrl.manualMode = 0;

	initMainHardware();
  


  /* Loop forever */
  for (;;) {
  		TransmitData("\n\r**The Freescale FTA**");
		TransmitData("\n\r*********************");
		TransmitData("0.Stop Mode\n\r");
		TransmitData("1.Manual\n\r");
		TransmitData("2.AutoPilot\n\r");
		TransmitData("3.ServoTest\n\r");
		TransmitData("6.Camera\n\r");
		TransmitData("\n\r**********************");
		
		option = ReadData();
		
		switch(option)
		{
			case '0':
				car.ctrl.manualMode = 0;
			break;			
			case '1':
				
				TransmitData("\n\r**Manual Mode**");
				TransmitData("\n\r*********************");
				SIU.PGPDO[0].R = 0x0000C000;		/* Enable Right the motors */
				car.ctrl.targetVelocity = 0;
				car.ctrl.targetServoPos = 0;
				car.ctrl.biasVelocity = constBiasCenter;				
				car.ctrl.manualMode = 1;   //0 = dormant, 1 = manual, 2 = auto
				
				 for (;;) 
				 {
					option = ReadData();
			
					switch(option)
					{
						case 'r':	car.ctrl.biasVelocity -= 5; 
							if(car.ctrl.biasVelocity <0)
								car.ctrl.biasVelocity =0;	break;
						case 'y':	car.ctrl.biasVelocity += 5; 
							if(car.ctrl.biasVelocity >constBiasCenter*2)
								car.ctrl.biasVelocity =constBiasCenter*2
								;break;
						case 'f':	car.ctrl.targetServoPos -= 8; break;
						case 'h':	car.ctrl.targetServoPos += 8; break;
						case 't':	car.ctrl.targetVelocity += 10; break;
						case 'g':
							car.ctrl.targetVelocity -= 10; 
							if(car.ctrl.targetVelocity <0)
								car.ctrl.targetVelocity =0;							
							break;
						case 'x':goto endOut; break;
					}						
					
				 }
				 endOut:
				 	SIU.PGPDO[0].R = 0x00000000;		/* Enable Right the motors */
				 	car.ctrl.manualMode = 0;
			break;
			case '2':
				SIU.PGPDO[0].R = 0x0000C000;
				car.ctrl.autoTimer = cal.runTime;
				car.ctrl.manualMode = 2;
			break;
			case '3':  
				SERVO();  // test the servo
			break;
		
			case '6':
				printlistall();  //camera debug output
			break;	
			
			default:
			TransmitData((char*)&option);
			break;
		}
  }
}



void task_1msec()
{
	
}

void task_2msec()
{
 	U32 motLeft;
  	U32 motRight;
 
	car.ctrl.velTarget = (U16)((U32)car.ctrl.velTarget * 2 + car.ctrl.targetVelocity)/3;

	motLeft = ((U32)car.ctrl.velTarget * (U32)car.ctrl.biasVelocity)/constBiasCenter;
	motRight = ((U32)car.ctrl.velTarget * (constBiasCenter*2-(U32)car.ctrl.biasVelocity))/constBiasCenter;

	vfnSet_Duty_Opwm(6,motLeft);
	vfnSet_Duty_Opwm(7,motRight);

	taskCTR_2msec=0;
}

void task_5msec()
{
 	
	taskCTR_5msec=0;
}

void task_10msec()
{
	u8Capture_Pixel_Values();
	
	if(car.ctrl.manualMode==2)	//auto pilot Mode
	{
	
		//set the steering position, iTerm
		if((car.ctrl.error>cal.errorTol) || (car.ctrl.error < -cal.errorTol))
		{					
			car.ctrl.iTerm += (float)car.ctrl.error * cal.servGainIgain;
			
			//Limit the iTerm
			if(car.ctrl.iTerm > cal.servGainIgainLimit)
				car.ctrl.iTerm = cal.servGainIgainLimit;
			else if(car.ctrl.iTerm < -cal.servGainIgainLimit)
				car.ctrl.iTerm = -cal.servGainIgainLimit;
		}
		
		
		//set the position, P, and I term only here
		car.ctrl.targetServoPos = (S16)((float)car.ctrl.error*cal.servGain + car.ctrl.iTerm + car.ctrl.dTerm);
		
			//limit the servo Travel
		if(car.ctrl.targetServoPos < -constServoMax)
			car.ctrl.targetServoPos = -constServoMax;
		else if(car.ctrl.targetServoPos > constServoMax)
			car.ctrl.targetServoPos = constServoMax;
			
			
		if(car.sensor.valid==1) //sensor has a valid read
		{
		
			
			//set the target open loop velocity
			if(car.ctrl.error < -cal.errorTol)
				car.ctrl.targetVelocity = (U16)(cal.maxSpeed + car.ctrl.error*cal.servGain);
			else if(car.ctrl.error > cal.errorTol)
				car.ctrl.targetVelocity = (U16)(cal.maxSpeed - car.ctrl.error*cal.servGain);
			else
				car.ctrl.targetVelocity = cal.maxSpeed;
			

			//aditional speed damping for turning events
			if(car.ctrl.targetServoPos>15)
				car.ctrl.targetVelocity = car.ctrl.targetVelocity * 4 / 5;
			else if(car.ctrl.targetServoPos<-15)
				car.ctrl.targetVelocity = car.ctrl.targetVelocity * 4 / 5;

			car.ctrl.biasVelocity	= constBiasCenter + car.ctrl.targetServoPos / 2;
		}
		else
		{
			car.ctrl.targetVelocity = cal.lostSpeed;			
			car.ctrl.biasVelocity	= constBiasCenter + car.ctrl.targetServoPos / 2;	
		}		
		
	}
	else if(car.ctrl.manualMode==0) // manual Mode
	{
		SIU.PGPDO[0].R = 0x00000000;
		car.ctrl.targetVelocity = 0;	
		car.ctrl.biasVelocity = constBiasCenter;	
		
		//set the position, P, and I term only here
		car.ctrl.targetServoPos = (S16)car.ctrl.error*cal.servGain;
		if(car.ctrl.targetServoPos<-constServoMax)
			car.ctrl.targetServoPos = -constServoMax;
		else if(car.ctrl.targetServoPos>constServoMax)
			car.ctrl.targetServoPos = constServoMax;
	}
	
	taskCTR_10msec=0;
}


void task_20msec()
{
	//set the servo PWM,
	EMIOS_0.CH[4].CBDR.R = constServoMiddle + car.ctrl.targetServoPos; 

	taskCTR_20msec=0;
}


void task_40msec()
{

	if((SIU.PGPDI[2].R & 0x10000000) == 0x0000000)
	{
		SIU.PGPDO[0].R = 0x0000C000;
		car.ctrl.autoTimer = cal.runTime;
		car.ctrl.manualMode = 2;		
	}
	taskCTR_40msec=0;
}

void task_100msec()
{
	SendStatusPacket();
	taskCTR_100msec=0;
}


void task_500msec()
{
	volatile U8 blinkLed = 0;
	if(blinkLed>=1)
		{
			SIU.GPDO[68].R = 0x00;		
			blinkLed=0;
		}
	else
		{
			SIU.GPDO[68].R = 0x01;		
			blinkLed++;
		}
	taskCTR_500msec=0;
}

void task_1000msec()
{
	if(car.ctrl.autoTimer>0)
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
  for(dly=0;dly<120;dly++)
  {  	
  };
}

void Delaylong(void)
{
  for(dly=0;dly<20000;dly++)
  {
  	
  };
}

void Delaylonglong(void)
{
  for(lly=0;lly<1;lly++) Delaylong();
}

void Delaycamera(void)
{
  for(lly=0;lly<40;lly++) Delay();
}

void Delaywait(void)
{
  for(lly=0;lly<500;lly++) Delaylong();
}

void Delayled(void)
{
  for(lly=0;lly<500;lly++) Delaylong();
}


void Pit1ISR(void) 
{
	Pit1Ctr++;              /* Increment interrupt counter */
  	if ((Pit1Ctr & 1)==0)
   		{ /* If PIT1Ctr is even*/
    	INTC.SSCIR[4].R = 2;      /*  then nvoke software interrupt 4 */
  		}
 	PIT.CH[1].TFLG.B.TIF = 1;    /* MPC56xxB/P/S: Clear PIT 1 flag by writing 1 */
}



void SwIrq4ISR(void)
{
	SWirq4Ctr++;		 		/* Increment interrupt counter */
	
	//task running
	task_1msec();
	if(taskCTR_2msec++>=2) task_2msec();
	if(taskCTR_5msec++>=5) task_5msec();
	if(taskCTR_10msec++>=10) task_10msec();	
	if(taskCTR_20msec++>=20) task_20msec();
	if(taskCTR_40msec++>=40) task_40msec();
	if(taskCTR_100msec++>=100) task_100msec();
	if(taskCTR_500msec++>=500) task_500msec();
	if(taskCTR_1000msec++>=1000) task_1000msec();

	
  	INTC.SSCIR[4].R = 1;		/* Clear channel's flag */  
}







