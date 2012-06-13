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
#include "main.h"

/****************** Definitions ******************************/
#define SERVO_CTRL			4					
#define SERVO_CTRL_PCR		PCR_EMIOS_0_4		/* PE6 */

#define SERVO_MIN_US		1200				/* Min val in microseconds */
#define SERVO_MAX_US		1800			/* Max val in microseconds */

#define SERVO_MCB_CHANNEL	16
#define MOTOR_MCB_CHANNEL	8
/*************************************************************/


/****************** Functions ********************************/
void vfnSet_Servo(S16 s16Position)  /* Values are between u16MinVal and u16MaxVal*/
{   
	if(s16Position > -constServoMax & s16Position<constServoMax)
	{
		EMIOS_0.CH[4].CBDR.R = constServoMiddle + s16Position; 
	}
	
}
