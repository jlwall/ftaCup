/*!
	  \file     Driver_Servo.c
	  \brief    The driver for the servo motor, it includes initialization of
	  			counters and channels, and a function that sets the servo motor
	  			at a specified position.
	  \author   Cuauhtli Padilla
	  \author   Rodrigo Mendoza
	  \author   Marco Trujillo
	  \date     July 16 2010	  
*/

#include "MPC5604B_M07N.h"
#include "Driver_MPC5604B.h"
#include "dEMIOS.h"
#include "dSIU.h"

/****************** Definitions ******************************/
#define SERVO_CTRL			4					
#define SERVO_CTRL_PCR		PCR_EMIOS_0_4		/* PE6 */

#define SERVO_MIN_US		1200				/* Min val in microseconds */
#define SERVO_MAX_US		1800			/* Max val in microseconds */

#define SERVO_MCB_CHANNEL	16
#define MOTOR_MCB_CHANNEL	8
/*************************************************************/


/****************** Global Variables *************************/
S16 i16Pos;				/* The position of the servo from -1000 to 1000 */
/*************************************************************/


/****************** Functions ********************************/
void vfnSet_Servo(S16 u16Position)  /* Values are between u16MinVal and u16MaxVal*/
{   
	if(u16Position>-1000 & u16Position<1000)
	{
	
		U16 u16Duty;      
		u16Duty = (U16)(((u16Position) * (SERVO_MAX_US - SERVO_MIN_US)) / (1000) + SERVO_MIN_US); 
		
		i16Pos = u16Duty;//(int16_t)(((u16Position ) * 200) / (1000)) - (int16_t)(100);
		vfnSet_Duty_Opwm(SERVO_CTRL,u16Duty); 
	}
	else
		i16Pos = 0;
}

void vfnInit_Servo(void)
{
	vfnSetup_Emios_0();
	vfnInit_Emios_0();   	  					
	  					
  	vfnInit_Emios_0_Mcb(SERVO_MCB_CHANNEL,2000); 							/* Set channel as MCB  */	
  	
  	vfnInit_Emios_0_Opwm(SERVO_CTRL,0,800);				/* Set channel as OPWM */
	vfnInit_Emios_Output_Pad(28);				/* Set as output in SIU */
}
/*************************************************************/