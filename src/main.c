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
struct CAR_CTRL car;


    uint32_t Pit1Ctr = 0;   /* Counter for PIT 1 interrupts */
    uint32_t SWirq4Ctr = 0;	/* Counter for software interrupt 4 */

//**function Prototypes*//


int main(void) 
{
  volatile int i = 0;
  char option = 0;
	
  initMainHardware();
  
  

  /* Loop forever */
  for (;;) {
  		TransmitData("\n\r**The Freescale Cup**");
		TransmitData("\n\r*********************");
		TransmitData("\n\r1.Led\n\r");
		TransmitData("2.Switch\n\r");
		TransmitData("3.Servo\n\r");
		TransmitData("4.Motor Left\n\r");
		TransmitData("5.Motor Right\n\r");
		TransmitData("6.Camera\n\r");
		TransmitData("7.Left Motor Current\n\r");
		TransmitData("8.Right Motor Current\n\r");
		TransmitData("9.Trim Pot");
		TransmitData("\n\r**********************");
		
		option = ReadData();
		
		switch(option)
		{
			case '1':
				LED();
			break;
			case '2':
				SWITCH();
			break;
			case '3':
				SERVO();
			break;
			case '4':
				MOTOR_LEFT();
			break;
			case '5':
				MOTOR_RIGHT();
			break;
			case '6':
				CAMERA();
			break;
			case '7':
				LEFT_MOTOR_CURRENT();
			break;
			case '8':
				RIGHT_MOTOR_CURRENT();
			break;
			case '9':
				TRIM_POT_ADC();
			break;
			case '0':
				
				TransmitData("\n\r**Manual Mode**");
				TransmitData("\n\r*********************");
				 for (;;) 
				 {
					option = ReadData();
			
					switch(option)
					{
						case 'f':	car.targetServoPos -= 4; break;
						case 'h':	car.targetServoPos += 4; break;
						case 't':	car.targetVelocity += 5; break;
						case 'g':	car.targetVelocity -= 5; break;

					}
				 }
			break;
			
			
			default:
			TransmitData((char*)&option);
			break;
		}

  }
}



void task_1msec()
{
	
/*	if(SIU.GPDI[65].R>=1)
	{
			SIU.GPDO[69].R = 0x00;	
	}

	else
	{
			SIU.GPDO[69].R = 0x01;	
	}
	*/
}

void task_2msec()
{
	
	taskCTR_2msec=0;
}

void task_5msec()
{
	
	taskCTR_5msec=0;
}

void task_10msec()
{
	
	taskCTR_10msec=0;
}


void task_20msec()
{
	vfnSet_Servo(car.targetServoPos);
	taskCTR_20msec=0;
}


void task_40msec()
{
	
	taskCTR_40msec=0;
}

void task_100msec()
{

	taskCTR_100msec=0;
}

int r = 0;
void task_500msec()
{

if(r>=1)
	{
			SIU.GPDO[68].R = 0x00;		
			r=0;
	}

	else
	{
			SIU.GPDO[68].R = 0x01;		
			r++;
	}


	taskCTR_500msec=0;
}

void task_1000msec()
{
	
	
	taskCTR_1000msec=0;
}


void Delay(void)
{
  for(dly=0;dly<250;dly++);
}

void Delaylong(void)
{
  for(dly=0;dly<20000;dly++);
}

void Delaylonglong(void)
{
  for(lly=0;lly<1;lly++) Delaylong();
}

void Delaycamera(void)
{
  for(lly=0;lly<10;lly++) Delaylong();
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







