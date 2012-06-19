


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


#define noDerv 1

		
			
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
			
		if(car.sensor.teachDone>0)
		{
			car.sensor.array[iCamera] = (U16)(((ADC.CDR[0].B.CDATA ) * 512) / car.sensor.arrayTeach[iCamera]);
		}
		else		
		{		
			car.sensor.array[iCamera] = (U16)(ADC.CDR[0].B.CDATA );// + (U16)(iCamera-64)/2;
		}

		if((iCamera>5) & iCamera < 123)  //if we are in edgeless zone, lets find the min and max,
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
	U8 modeFind=0;
	U32 avgSum = 0;
	S8 derive = 0;
	
	PIT.CH[2].TCTRL.B.TEN = 0;    /* MPC56xxB/P/S: Clear PIT 1 flag by writing 1 */
	
	//disableIrq();		   	/* Ensure INTC current prority=0 & enable IRQ */
	car.sensor.threshold = (car.sensor.valMax - car.sensor.valMin)>>2;	
	car.sensor.cornLeft = minPosCamera;
	car.sensor.cornRight = minPosCamera;
	
	i=2;
	while(i<125)
	{
		if(modeFind==0)
		{
			if(car.sensor.array[i] < car.sensor.threshold)
			{
			car.sensor.cornLeft = i;
			modeFind =1;	
			}
		}
		else if(modeFind==1)
		{
			if(car.sensor.array[i] > car.sensor.threshold)
			{
			if(( (i - car.sensor.cornLeft) > car.TeachSenseWidthMin) && ((i - car.sensor.cornLeft) < car.TeachSenseWidthMax))	
				{				
				car.sensor.cornRight = i;
				modeFind = 2;
				}
			}
			
		}
		else
			break;
		
		i++;
		
	}


	#ifndef noDerv
	
	car.sensor.deriveValMax = -100;
	car.sensor.deriveValMin = 100;
	car.sensor.deriveCornLeft = 10;
	car.sensor.deriveCornRight = 118;
	
	//averaging Method
	for(i=10;i<118;i++)
	{		
		derive = (S8)(((S16)derive*5 +(S16)car.sensor.array[i] - (S16)car.sensor.array[i-1])/6); //averaged derivative
		car.sensor.arrayDerive[i] = derive;
			
		if(car.sensor.deriveValMin > car.sensor.arrayDerive[i]) //if a new min is found, latch it
			{
			car.sensor.deriveValMin = car.sensor.arrayDerive[i];
			car.sensor.deriveValMax = -100; //reset max find for RHS
			car.sensor.deriveCornLeft = i;								// set the mim pos
			}
		if(car.sensor.deriveValMax < car.sensor.arrayDerive[i])
			{
			car.sensor.deriveValMax = car.sensor.arrayDerive[i];
			car.sensor.deriveCornRight = i;				
			} 		
	}
	
	car.sensor.deriveWidth = car.sensor.deriveCornRight - car.sensor.deriveCornLeft;
	#endif
	car.sensor.width = car.sensor.cornRight - car.sensor.cornLeft;
	
	car.sensor.c4 = car.sensor.c3;
	car.sensor.c3 = car.sensor.c2;
	car.sensor.c2 = car.sensor.c1;
	car.sensor.c1 = car.sensor.center;
	

		#ifndef noDerv
	//A tap delay to help average the center measured position
	if((car.sensor.deriveWidth >= cal.senseWidthMinDer) && (car.sensor.deriveWidth <= cal.senseWidthMaxDer)) //derivOKay
	{
		if(modeFind==2) //stokay found both
		{
			
		
		car.sensor.center = (car.sensor.cornRight + car.sensor.cornLeft + car.sensor.deriveCornRight + car.sensor.deriveCornLeft)>>4;

		car.ctrl.error =  (S16)((car.ctrl.error + ((S16)car.sensor.center-car.ctrl.controlCenter)))>>1;
		
		if(car.ctrl.error<-cal.sensorMaxError)
			car.ctrl.error = (S16)-cal.sensorMaxError;
		else if(car.ctrl.error>cal.sensorMaxError)
			car.ctrl.error = (S16)cal.sensorMaxError;
		
		
		car.sensor.valid = 3;
		}
		else
		{
		car.sensor.center = (car.sensor.deriveCornRight + car.sensor.deriveCornLeft)>>1;

		car.ctrl.error =  (S16)((car.ctrl.error + ((S16)car.sensor.center-car.ctrl.controlCenter)))>>1;
		
		if(car.ctrl.error<-cal.sensorMaxError)
			car.ctrl.error = (S16)-cal.sensorMaxError;
		else if(car.ctrl.error>cal.sensorMaxError)
			car.ctrl.error = (S16)cal.sensorMaxError;
		
		
		car.sensor.valid = 2;	
			
		}
	
	}
	else if((modeFind==2) && (car.sensor.threshold >= car.sensor.TeachSensorMinDynRange))
	{
		car.sensor.center = (car.sensor.cornRight + car.sensor.cornLeft)>>1;

		car.ctrl.error =  (S16)((car.ctrl.error + ((S16)car.sensor.center-car.ctrl.controlCenter)))>>1;
		
		if(car.ctrl.error<-cal.sensorMaxError)
			car.ctrl.error = (S16)-cal.sensorMaxError;
		else if(car.ctrl.error>cal.sensorMaxError)
			car.ctrl.error = (S16)cal.sensorMaxError;
		
		
		car.sensor.valid = 1;
	}
	else
		car.sensor.valid = 0;
	
	#else
	
	if((car.sensor.threshold >= car.TeachSensorMinDynRange) && (modeFind==2))
		{
		car.sensor.center = (car.sensor.cornRight + car.sensor.cornLeft)>>1;

		car.ctrl.error =  (S16)((car.ctrl.error + ((S16)car.sensor.center-car.ctrl.controlCenter)))>>1;
		
		if(car.ctrl.error<-cal.sensorMaxError)
			car.ctrl.error = (S16)-cal.sensorMaxError;
		else if(car.ctrl.error>cal.sensorMaxError)
			car.ctrl.error = (S16)cal.sensorMaxError;
		
		
		car.sensor.valid = 1;
	}
	else
		car.sensor.valid = 0;
	#endif

 
	if(((SIU.PGPDI[2].R & 0x20000000) == 0x0000000) && car.sensor.teachDone==0) // button 3 pressed, teach
	{
	i=0;
		while(i<128)
		{
			car.sensor.arrayTeach[i] = 	car.sensor.array[i] ;
			i++;
		}
		car.sensor.teachDone = 1;
		
	}
	
	
	if(((SIU.PGPDI[2].R & 0x40000000) == 0x0000000) & car.sensor.teachDone==1) // button 2 pressed, teach line chars
	{
	if(car.sensor.width < 8)
		car.TeachSenseWidthMin = car.sensor.width/2;
	else
		car.TeachSenseWidthMin = car.sensor.width-7;
	car.TeachSenseWidthMax = car.sensor.width+7;
	car.TeachSensorMinDynRange = car.sensor.threshold * 9 / 16;
		car.sensor.teachDone = 2;
		
	}
	
	taskPIDupdate();
	//enableIrq();		   	/* Ensure INTC current prority=0 & enable IRQ */
	
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
	

	if((car.sensor.width >= car.TeachSenseWidthMin) && (car.sensor.width <= car.TeachSenseWidthMax) && (car.sensor.threshold >= car.TeachSensorMinDynRange))
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
  			
