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
			
		if(car.sensor.teachDone>=10)
		{
			car.sensor.array[iCamera] = (U16)(((ADC.CDR[0].B.CDATA ) * 512) / car.sensor.arrayTeach[iCamera]);
		}
		else		
		{		
			car.sensor.array[iCamera] = (U16)(ADC.CDR[0].B.CDATA );// + (U16)(iCamera-64)/2;
		}

		if((iCamera>5) & iCamera < 123)  //if we are in edgeless zone, lets find the min and max,
			{	
			if(car.sensor.valMinTemp > car.sensor.array[iCamera]) //if a new min is found, latch it
				{
				car.sensor.valMinTemp = car.sensor.array[iCamera];
				minPosCamera = iCamera;								// set the mim pos
				}
			if(car.sensor.valMaxTemp < car.sensor.array[iCamera])
				{
				car.sensor.valMaxTemp = car.sensor.array[iCamera];			
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
	car.sensor.valMaxTemp = 0;
	car.sensor.valMinTemp = 0xFFFF;
	
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
	U8 centerTemp;
	U8 modeTemp;
	
	PIT.CH[2].TCTRL.B.TEN = 0;    /* MPC56xxB/P/S: Clear PIT 1 flag by writing 1 */
	
	//disableIrq();		   	/* Ensure INTC current prority=0 & enable IRQ */
	car.sensor.valMin = car.sensor.valMinTemp;
	car.sensor.valMax = car.sensor.valMaxTemp;
	car.sensor.threshold = (car.sensor.valMaxTemp - car.sensor.valMinTemp)>>2;	
//	car.sensor.cornLeft = 0;
//	car.sensor.cornRight = 127;
	
	if(car.sensor.threshold >= car.TeachSensorMinDynRange || car.sensor.teachDone==0)
	{
		
	
		i=3;
		while(i<124)
		{
			if(modeFind==0)
			{
				if(car.sensor.array[i] < (car.sensor.threshold + car.sensor.valMin))
				{
				car.sensor.cornLeft = i;
				modeFind =1;	
				}
			}
			else if(modeFind==1)
			{
				if(car.sensor.array[i] > (car.sensor.threshold + car.sensor.valMin))
				{
				if(( (i - car.sensor.cornLeft) >= car.TeachSenseWidthMin) && ((i - car.sensor.cornLeft) <= car.TeachSenseWidthMax))	
					{				
					car.sensor.cornRight = i;
					car.sensor.width = car.sensor.cornRight - car.sensor.cornLeft;	
					modeFind = 2;
					}
				else
					modeFind = 0;
				}
				
			}
			else
				break;
			
			i++;
			
	}
	}



	car.sensor.c4 = car.sensor.c3;
	car.sensor.c3 = car.sensor.c2;
	car.sensor.c2 = car.sensor.c1;
	car.sensor.c1 = car.sensor.center;
	


	
	if(modeFind==2)
		{
		centerTemp = (car.sensor.cornRight + car.sensor.cornLeft)>>1;
		
		modeTemp = 0;
		
		if(centerTemp <=59)
			modeTemp = camLeft;
		else if(centerTemp>=69)
			modeTemp = camRight;
		else
			modeTemp = camCenter;
		
		switch(car.sensor.valid)
		{
			case camLostLeft:  //cam currently lost, trying to find left
				if(modeTemp == camLeft)
					{
					car.sensor.valid = camLeft;
					car.sensor.center = centerTemp;
					}			
			break;
			case camLostRight: //cam currently lost, trying to find right
				if(modeTemp == camRight)
					{
					car.sensor.valid = camRight;
					car.sensor.center = centerTemp;
					}			
			break;			
			case camCenter:	// cam was Center, change to left or right is needed
				car.sensor.center = centerTemp;
				car.sensor.valid = modeTemp;
			break;
			case camLeft:	// cam was Left, move to LRC if needed
				car.sensor.center = centerTemp;
				car.sensor.valid = modeTemp;
			break;
			case camRight:	// cam was Right, move to LRC if needed
				car.sensor.center = centerTemp;
				car.sensor.valid = modeTemp;
			break;
			default: //called on first entry, or when valid has been corrupted outside of 1<->5
				
				car.sensor.valid = modeTemp;
			break;			
		}
		
		//we have cleaned the center Error, and mode, calculate error on current valid center

		if(car.sensor.errorCounter>0) //if Error is Greater than one, decrement it
			car.sensor.errorCounter--;
		
		car.ctrl.error =  (S16)(car.sensor.center-car.ctrl.controlCenter);

		//Limit the error to calibrated outer window		
		if(car.ctrl.error<-cal.sensorMaxError)
			car.ctrl.error = (S16)-cal.sensorMaxError;
		else if(car.ctrl.error>cal.sensorMaxError)
			car.ctrl.error = (S16)cal.sensorMaxError;
	}
	else
	{
		if(car.sensor.errorCounter<20) //if Error is less than max increment it
			car.sensor.errorCounter++;
		
		// we have recieved a bad frame
		switch(car.sensor.valid)
		{			
			case camCenter:	// cam was Center, we have an invalid frame
				if(car.sensor.errorCounter>5)
				{
					
				}
			break;
			case camLeft:	// cam was Left, move to LostLeft if needed
				if(car.sensor.errorCounter>5)
					{
					car.sensor.valid = camLostLeft;					
					}				
				
			break;
			case camRight:	// cam was Right, move to LostRight if needed
				if(car.sensor.errorCounter>5)
					{
					car.sensor.valid = camLostRight;
					}
				
			break;
			default: //called on first entry, or when valid has been corrupted outside of 1<->5
				
			break;			
		}
	}

 
	if(((SIU.PGPDI[2].R & 0x20000000) == 0x0000000) && car.sensor.teachDone<10) // button 3 pressed, teach
	{
	i=0;
	
		while(i<128)
		{
			if(car.sensor.teachDone==0)
				car.sensor.arrayTeach[i] = 	car.sensor.array[i] ;
			else
				car.sensor.arrayTeach[i] = 	(car.sensor.arrayTeach[i] + car.sensor.array[i])>>1 ;
			i++;
		}
		car.sensor.teachDone += 1;
		
	}	
	
	else if(((SIU.PGPDI[2].R & 0x40000000) == 0x0000000) && car.sensor.teachDone==10) // button 2 pressed, teach line chars
	{
	if(car.sensor.width < 8)
		car.TeachSenseWidthMin = car.sensor.width/3;
	else
		car.TeachSenseWidthMin = car.sensor.width-7;
	car.TeachSenseWidthMax = car.sensor.width+7;
	
	car.TeachSensorMinDynRange = car.sensor.threshold * 7 / 16;
		car.sensor.teachDone = 11;
		
	}
	
	taskPIDupdate();
	//enableIrq();		   	/* Ensure INTC current prority=0 & enable IRQ */
	
}
 			
