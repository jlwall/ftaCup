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
	U8  senseWidthMinDer;
	U8  senseWidthMaxDer;
	U8  runTime;
	
	U8  sensorMinDynRange;
	S8  sensorMaxError;
	U8 apexModError
*/
	7.1,
	0.2,
	0.0,
	
	190,
	270,
	
	25,
	20,
	0.65, 
	
	50, 
	
	4,
	370,
	200,
	
	5,
	20,
	7,
	21,
	60,
	
	40,50,0
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
	
	car.adjust.adjPgain = 0;
	car.adjust.adjIgain = 0;
	car.adjust.adjDgain = 0;
	car.adjust.adjSpeedgain = 0;
	car.sensor.teachDone=0;
	
	car.sensor.TeachSensorMinDynRange = cal.sensorMinDynRange;
	car.sensor.TeachSenseWidthMin = cal.senseWidthMin;
	car.sensor.TeachSenseWidthMax = cal.senseWidthMax;
	
	
	
		//Set the active Gains
		car.ctrl.pGain =  cal.servGain + car.adjust.adjPgain;
		car.ctrl.iGain =  cal.servGainIgain + car.adjust.adjIgain;
		car.ctrl.dGain =  cal.servGainDTerm + car.adjust.adjDgain;
		car.ctrl.speedGain =  cal.maxSpeed + car.adjust.adjSpeedgain;

	initMainHardware();
  


  for (;;) {
  }

/*
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
				SIU.PGPDO[0].R = 0x0000C000;	
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
				 	SIU.PGPDO[0].R = 0x00000000;	
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
  */
}



void task_2msec()
{
 	U32 motLeft;
  	U32 motRight;
 
 	//Smoothen the velocity command target from where the App set it
	car.ctrl.velTarget = (U16)((U32)car.ctrl.velTarget * 5 + car.ctrl.targetVelocity)/6;
		
	if(car.ctrl.velTarget > 900) //limit the applied velocity Target
		car.ctrl.velTarget = 900;
	

	//Apply velocities to left and right based off bias
	motLeft = ((U32)car.ctrl.velTarget * (U32)car.ctrl.biasVelocity)/constBiasCenter;
	motRight = ((U32)car.ctrl.velTarget * (constBiasCenter*2-(U32)car.ctrl.biasVelocity))/constBiasCenter;

	vfnSet_Duty_Opwm(6,motLeft);
	vfnSet_Duty_Opwm(7,motRight);

	taskCTR_2msec=0;
}

void taskPIDupdate()
{	
	if(car.ctrl.manualMode==2)	//auto pilot Mode ==2
	{
	
	
	
		//set the steering position, iTerm
		if((car.ctrl.error>cal.errorTol) || (car.ctrl.error < -cal.errorTol))
			{					
			car.ctrl.iTerm += (float)car.ctrl.error * car.ctrl.iGain;
			
			//Limit the iTerm
			if(car.ctrl.iTerm > cal.servGainIgainLimit)
				car.ctrl.iTerm = cal.servGainIgainLimit;
			else if(car.ctrl.iTerm < -cal.servGainIgainLimit)
				car.ctrl.iTerm = -cal.servGainIgainLimit;
			}
		
			//apply the dTerm
			car.ctrl.dterm = (car.sensor.center - car.sensor.c2) * car.ctrl.dGain;
		
			
			//set the position, P, and I term only here
			car.ctrl.targetServoPos = (S16)((float)car.ctrl.error*car.ctrl.pGain + car.ctrl.iTerm + car.ctrl.dterm);
		
			//limit servo position
			if(car.ctrl.targetServoPos<-constServoMax)
				car.ctrl.targetServoPos = -constServoMax;
			else if(car.ctrl.targetServoPos>constServoMax)
				car.ctrl.targetServoPos = constServoMax;
			
			//set the control center Apex seeking portion
			car.ctrl.controlCenter = (U8)(64 + car.ctrl.targetServoPos * (S16)cal.apexModError / constServoMax);		
			//car.ctrl.controlCenter = 64;
	
		if(car.sensor.valid >=1) //sensor has a valid read
		{
			
			//set the target open loop velocity
			car.ctrl.targetVelocity = (U16)car.ctrl.speedGain;
			

			//aditional speed damping for turning events
			if(car.ctrl.targetServoPos>200)
				car.ctrl.targetVelocity = car.ctrl.targetVelocity * 4 / 5;
			else if(car.ctrl.targetServoPos<-200)
				car.ctrl.targetVelocity = car.ctrl.targetVelocity * 4 / 5;

			car.ctrl.biasVelocity	= constBiasCenter - car.ctrl.targetServoPos / 2;
		}
		else
		{
			car.ctrl.targetVelocity = cal.lostSpeed;			
			car.ctrl.biasVelocity	= constBiasCenter - car.ctrl.targetServoPos / 2;	
		}		
		
	}
	else if(car.ctrl.manualMode==0) // manual Mode
	{
		SIU.PGPDO[0].R = 0x00000000;
		car.ctrl.targetVelocity = 0;	
		car.ctrl.biasVelocity = constBiasCenter;	
		
		
		if((car.ctrl.error>cal.errorTol) || (car.ctrl.error < -cal.errorTol))
		{					
			car.ctrl.iTerm += (float)car.ctrl.error * car.ctrl.iGain;
			
			//Limit the iTerm
			if(car.ctrl.iTerm > cal.servGainIgainLimit)
				car.ctrl.iTerm = cal.servGainIgainLimit;
			else if(car.ctrl.iTerm < -cal.servGainIgainLimit)
				car.ctrl.iTerm = -cal.servGainIgainLimit;
		}
		
		car.ctrl.dterm = (car.sensor.center - car.sensor.c2) * car.ctrl.dGain;
		
		
		//set the position, P, and I term only here
		car.ctrl.targetServoPos = (S16)((float)car.ctrl.error*car.ctrl.pGain + car.ctrl.iTerm + car.ctrl.dterm);
		//car.ctrl.targetServoPos = (S16)(float)car.ctrl.error*car.ctrl.pGain;
		
		//set the position, P, and I term only here
		if(car.ctrl.targetServoPos<-constServoMax)
			car.ctrl.targetServoPos = -constServoMax;
		else if(car.ctrl.targetServoPos>constServoMax)
			car.ctrl.targetServoPos = constServoMax;
		
	
		
		car.ctrl.controlCenter = 64;
	
	}
	
/*	i=0;
	while(i<16)
	{
	TransmitMsgRef((U8*)&car.sensor.array[i*8],8,8+i,0x400+i);
	i++;
	}
	*/		   
}

void task_5msec()
{
 	vfnSet_Servo(car.ctrl.targetServoPos);
 	
	taskCTR_5msec=0;
}

void task_10msec()
{	


 	taskCTR_10msec=0;
}


void task_20msec()
{

		taskUpdateCameraStart();

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
		car.ctrl.pGain =  cal.servGain + car.adjust.adjPgain;
		car.ctrl.iGain =  cal.servGainIgain + car.adjust.adjIgain;
		car.ctrl.dGain =  cal.servGainDTerm + car.adjust.adjDgain;
		car.ctrl.speedGain =  cal.maxSpeed + car.adjust.adjSpeedgain;
		 
		car.ctrl.autoTimer = cal.runTime;
		car.ctrl.manualMode = 2;		
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
				car.ctrl.pGain =  cal.servGain + car.adjust.adjPgain;
			}
			else if(SIU.GPDI[103].R== 0x01) // P gain Mod
			{	
			car.adjust.adjIgain = ((float)tempADC - 512) * cal.servGainIgain / 512;
				car.ctrl.iGain =  cal.servGainIgain + car.adjust.adjIgain;
			}
			else if(SIU.GPDI[104].R== 0x01) // P gain Mod
			{	
			car.adjust.adjDgain = ((float)tempADC - 512) * cal.servGainDTerm / 512;
			
		car.ctrl.dGain =  cal.servGainDTerm + car.adjust.adjDgain;
			}
			else if(SIU.GPDI[105].R== 0x01) // P gain Mod
			{	
			car.adjust.adjSpeedgain = ((float)tempADC - 512) * cal.maxSpeed / 512;			
		car.ctrl.speedGain =  cal.maxSpeed + car.adjust.adjSpeedgain;
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
	
	if(car.sensor.width>11 & car.sensor.width<40)
		SIU.GPDO[70].R = 0;
	else
		SIU.GPDO[70].R = 1;
	
	if(car.sensor.threshold > car.sensor.TeachSensorMinDynRange)
		SIU.GPDO[71].R = 0;
	else
		SIU.GPDO[71].R = 1;
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
	
	if((SIU.PGPDI[2].R & 0x20000000) == 0x00)
	{
		car.ctrl.targetServoPos += 50;	
	//DO SOME SWITCH CAL MODDING HERE
	}
	else 	if((SIU.PGPDI[2].R & 0x40000000) == 0x00)
	{
		car.ctrl.targetServoPos -= 50;	
	//DO SOME SWITCH CAL MODDING HERE
	}
	else
	{
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
  	if ((Pit1Ctr & 1)==0)
   		{ /* If PIT1Ctr is even*/
    	INTC.SSCIR[4].R = 2;      /*  then nvoke software interrupt 4 */
  		}
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
	
	//task running
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







