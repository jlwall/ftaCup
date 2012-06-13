


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


	vU8 iCamera;
	vU8 cameraTaskState;
	vU8 minPosCamera;						

void taskUpdateCamera(void)
{
	if(cameraTaskState==1)
	{
		
	
		SIU.PGPDO[0].R |= 0x00000004;	/* Sensor Clock High */	
		ADC.MCR.B.NSTART = 1;   	
		while(	ADC.MCR.B.NSTART ==1)
			{			
			};
			
        if(iCamera<64)
			car.sensor.array[iCamera] = (U16)(ADC.CDR[0].B.CDATA );// + (64-i)/8;
		else
			car.sensor.array[iCamera] = (U16)(ADC.CDR[0].B.CDATA );// + (i-64)/8;

		if((iCamera>2) & iCamera < 126)  //if we are in edgeless zone, lets find the min and max,
			{	
			if(car.sensor.valMin > car.sensor.array[iCamera]) //if a new min is found, latch it
				{
				car.sensor.valMin = car.sensor.array[iCamera];
				minPosCamera = iCamera;								// set the mim pos
				}
			if(car.sensor.valMax < car.sensor.array[iCamera])
				{
				car.sensor.valMax = car.sensor.array[iCamera];			
				} 	
			}	
		cameraTaskState=0;
		iCamera += 1;
		if(iCamera >127)
			taskUpdateCameraEnd();	
	}
	else
	{
		
	SIU.PGPDO[0].R &= ~0x00000004;	/* Sensor Clock Low */	
	cameraTaskState=1;
	}
	
}

void taskUpdateCameraStart(void)
{
	car.sensor.valMax = 0;
	car.sensor.valMin = 0xFFFF;
	
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
	
	iCamera = 0;
	cameraTaskState = 0;
	
	PIT.CH[2].TCTRL.B.TEN = 1;    /* MPC56xxB/P/S: Clear PIT 1 flag by writing 1 */
}

void taskUpdateCameraEnd(void)
{
	U8 i;
	U32 avgSum = 0;
	S16 derive = 0;
	
	PIT.CH[2].TCTRL.B.TEN = 0;    /* MPC56xxB/P/S: Clear PIT 1 flag by writing 1 */
	
	car.sensor.threshold = (car.sensor.valMax - car.sensor.valMin)/2;	
	car.sensor.cornLeft = minPosCamera;
	car.sensor.cornRight = minPosCamera;
	
  	while(((car.sensor.array[car.sensor.cornLeft])<(car.sensor.valMin+car.sensor.threshold)) && (car.sensor.cornLeft>2))
		{
		car.sensor.cornLeft--;	
		}
		
		
	while(((car.sensor.array[car.sensor.cornRight])<(car.sensor.valMin+car.sensor.threshold)) && (car.sensor.cornRight<126))
		{
		car.sensor.cornRight++;	
		}

	car.sensor.width = car.sensor.cornRight - car.sensor.cornLeft;
	
	car.sensor.c4 = car.sensor.c3;
	car.sensor.c3 = car.sensor.c2;
	car.sensor.c2 = car.sensor.c1;
	car.sensor.c1 = car.sensor.center;
	
	car.sensor.center = (car.sensor.cornRight + car.sensor.cornLeft)/2;

	
	car.sensor.deriveValMax = -30000;
	car.sensor.deriveValMin = 30000;
	car.sensor.deriveCornLeft = 0;
	car.sensor.deriveCornRight = 127;
	//averaging Method
	for(i=1;i<126;i++)
	{		
		derive = (derive + (S16)car.sensor.array[i] - (S16)car.sensor.array[i-1])/2; //averaged derivative
		car.sensor.arrayDerive[i] = derive;
			
		if(car.sensor.deriveValMin > car.sensor.arrayDerive[i]) //if a new min is found, latch it
			{
			car.sensor.deriveValMin = car.sensor.arrayDerive[i];
			car.sensor.deriveCornLeft = i;								// set the mim pos
			}
		if(car.sensor.deriveValMax < car.sensor.arrayDerive[i])
			{
			car.sensor.deriveValMax = car.sensor.arrayDerive[i];
			car.sensor.deriveCornRight = i;				
			} 		
	}
	
	//A tap delay to help average the center measured position
	//car.sensor.center = (car.sensor.center * 4 + car.sensor.c1 * 3 + car.sensor.c2 * 2 + car.sensor.c3)/10;
	

	if((car.sensor.width >= cal.senseWidthMin) && (car.sensor.width <= cal.senseWidthMax) && (car.sensor.threshold >= cal.sensorMinDynRange))
	{
		car.ctrl.error =  (S16)((car.ctrl.error + ((S16)car.sensor.center-car.ctrl.controlCenter)))/2;
		
		if(car.ctrl.error<-cal.sensorMaxError)
			car.ctrl.error = (S16)-cal.sensorMaxError;
		else if(car.ctrl.error>cal.sensorMaxError)
			car.ctrl.error = (S16)cal.sensorMaxError;
		
		
		car.sensor.valid = 1;
	}
	else
		car.sensor.valid = 0;

	



	
}

 
void u8Capture_Pixel_Values(void)
{
	
	U8 i;	
	U8	u8minpos=64;
	S32 	avgSum=0;

	car.sensor.valMax = 0;
	car.sensor.valMin = 0xFFFF;
	
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
           if(i<64)
			car.sensor.array[i] = (U16)(ADC.CDR[0].B.CDATA );// + (64-i)/8;
		else
			car.sensor.array[i] = (U16)(ADC.CDR[0].B.CDATA );// + (i-64)/8;

		if((i>2) & i < 126)  //if we are in edgeless zone, lets find the min and max,
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
	
  	while(((car.sensor.array[car.sensor.cornLeft])<(car.sensor.valMin+car.sensor.threshold)) && (car.sensor.cornLeft>2))
		{
		car.sensor.cornLeft--;	
		}
		
		
	while(((car.sensor.array[car.sensor.cornRight])<(car.sensor.valMin+car.sensor.threshold)) && (car.sensor.cornRight<126))
		{
		car.sensor.cornRight++;	
		}

	car.sensor.width = car.sensor.cornRight - car.sensor.cornLeft;
	
	car.sensor.c4 = car.sensor.c3;
	car.sensor.c3 = car.sensor.c2;
	car.sensor.c2 = car.sensor.c1;
	car.sensor.c1 = car.sensor.center;
	
	car.sensor.center = (car.sensor.cornRight + car.sensor.cornLeft)/2;

	//A tap delay to help average the center measured position
	//car.sensor.center = (car.sensor.center * 4 + car.sensor.c1 * 3 + car.sensor.c2 * 2 + car.sensor.c3)/10;
	

	if((car.sensor.width >= cal.senseWidthMin) && (car.sensor.width <= cal.senseWidthMax) && (car.sensor.threshold >= cal.sensorMinDynRange))
	{
		car.ctrl.error =  (S16)((car.ctrl.error + ((S16)car.sensor.center-car.ctrl.controlCenter)))/2;
		
		if(car.ctrl.error<-cal.sensorMaxError)
			car.ctrl.error = (S16)-cal.sensorMaxError;
		else if(car.ctrl.error>cal.sensorMaxError)
			car.ctrl.error = (S16)cal.sensorMaxError;
		
		
		car.sensor.valid = 1;
	}
	else
		car.sensor.valid = 0;

	//averaging Method
	for(i=0;i<64;i++)
	{		
		avgSum += car.sensor.array[i] - car.sensor.array[127-i];
	}


	//cross validate the sensor reading, checking the weighting average
/*	if(car.sensor.valid == 1)
	{
		if((avgSum > 20) & (car.ctrl.error < -2))
			car.sensor.valid = 0;
		else if((avgSum < -20) & (car.ctrl.error >2))
			car.sensor.valid = 0;
	}*/

}
  			
