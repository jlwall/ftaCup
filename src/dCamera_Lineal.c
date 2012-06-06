


#include "MPC5604B_M07N.h"
#include "Driver_MPC5604B.h"
#include "dEMIOS.h"
#include "dCamera_Lineal.h"
#include "main.h"


extern struct CAR car;
extern const struct CAR_CAL cal;
/********************* Definitions **************************/
#define	CHANNEL_CK			5
#define CHANNEL_SI    		3
#define CHANNEL_TRIG   		10
#define CAM_CNT_CHAN   		8
#define CAM_ADC_CHAN   		0
/*************************************************************/


/****************** Global Variables *************************/

/*************************************************************/




		
			
/**************************************************************************************************************/
/*          		   		FUNCTIONS USING ADC MODULE TO READ CAMERA OUTPUT   		                          */
/**************************************************************************************************************/
						


 
void u8Capture_Pixel_Values(void)
{
	
	U8 i;	
	U8	u8minpos=64;

	car.sensor.valMax = 0;
	car.sensor.valMin = 255;
	
	SIU.PCR[27].R = 0x0200;				/* Program the Sensor read start pin as output*/
	SIU.PCR[29].R = 0x0200;				/* Program the Sensor Clock pin as output*/
	SIU.PGPDO[0].R &= ~0x00000014;		/* All port line low */
	SIU.PGPDO[0].R |= 0x00000010;		/* Sensor read start High */
	Delay();
	SIU.PGPDO[0].R |= 0x00000004;		/* Sensor Clock High */
	Delay();
	SIU.PGPDO[0].R &= ~0x00000010;		/* Sensor read start Low */ 
	Delay();
	SIU.PGPDO[0].R &= ~0x00000004;		/* Sensor Clock Low */
	Delay();
	for (i=0;i<128;i++)
		{	
		Delay();
		SIU.PGPDO[0].R |= 0x00000004;	/* Sensor Clock High */	
		ADC.MCR.B.NSTART = 1;   	
		while(	ADC.MCR.B.NSTART ==1)
			{			
			};
		car.sensor.array[i] = (U8)(ADC.CDR[0].B.CDATA >> 2);
		if((i>10) & i < 118)  //if we are in edgeless zone, lets find the min and max,
			{	
			if(car.sensor.valMin > car.sensor.array[i]) //if a new min is found, latch it
				{
				car.sensor.valMin = car.sensor.array[i];
				u8minpos = i;								// set the mim pos
				}
			if(car.sensor.valMax < car.sensor.array[i])
				{
				car.sensor.valMax = car.sensor.array[i];			
				} 	
			}			
		Delay();
		SIU.PGPDO[0].R &= ~0x00000004;	/* Sensor Clock Low */		
		}	
		
		
	car.sensor.threshold = (car.sensor.valMax - car.sensor.valMin)/2;	
	car.sensor.cornLeft = u8minpos;
	car.sensor.cornRight = u8minpos;
	
  	while(((car.sensor.array[car.sensor.cornLeft])<(car.sensor.valMin+car.sensor.threshold)) && (car.sensor.cornLeft>5))
		{
		car.sensor.cornLeft--;	
		}
		
		
	while(((car.sensor.array[car.sensor.cornRight])<(car.sensor.valMin+car.sensor.threshold)) && (car.sensor.cornRight<123))
		{
		car.sensor.cornRight++;	
		}

	car.sensor.width = car.sensor.cornRight - car.sensor.cornLeft;
	
	car.sensor.c4 = car.sensor.c3;
	car.sensor.c3 = car.sensor.c2;
	car.sensor.c2 = car.sensor.c1;
	car.sensor.c1 = car.sensor.center;
	
	car.sensor.center = (car.sensor.cornRight + car.sensor.cornLeft)/2;
	
	if((car.sensor.width >= cal.senseWidthMin) && (car.sensor.width <= cal.senseWidthMax) && (car.sensor.threshold > cal.sensorMinDynRange))
	{
		car.ctrl.error =  (S16)((car.ctrl.error + ((S16)car.sensor.center-64)))/2;
		
		if(car.ctrl.error<-cal.sensorMaxError)
			car.ctrl.error = (S16)-cal.sensorMaxError;
		else if(car.ctrl.error>cal.sensorMaxError)
			car.ctrl.error = (S16)cal.sensorMaxError;
		
		car.sensor.valid = 1;
	}
	else
		car.sensor.valid = 0;

}
  			