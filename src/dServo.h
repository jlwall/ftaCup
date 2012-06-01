/*!
	  \file     Driver_Servo.h
	  \brief    The header file of Driver_Servo.c which includes all the function
	  			prototypes and an explanation of each function.
	  \author   Cuauhtli Padilla
	  \author   Rodrigo Mendoza
	  \author   Marco Trujillo
	  \date     July 16 2010	  
*/

/* Function Prototypes */

/**
  \brief	Sets the servo motor to a position relative to the maximum and minimum
  			values established. For example, if a reading in SAIC has a value of
  			10 as a minimum because less is invalid, and a maximum of 510, then a
  			call to vfnSet_Servo(260,10,510) would set the servo motor exactly at
  			the middle, and vfnSet_Servo(10,10,510) would set the servo motor
  			to the left (all left possible).
  \param  	u16Position: The desired position of the servo motor, with relative
  				values to the minimum and maximum values.
  			u16MinVal: The minimum value possible for u16Position (if u16Position
  				is equal to u16MinVal then the servo motor turns all the way
  				left).
  			u16MaxVal: The maximum value possible for u16Position (if u16Position
  				is equal to u16MaxVal then the servo motor turns all the way
  				right).
  \return	Null.
*/
void vfnSet_Servo(uint16_t u16Position, uint16_t u16MinVal, uint16_t u16MaxVal);

/**
  \brief	Initializes the MCB counters for both the servo motor and the
  			main motor modules and the OPWM channel for the direction pin of 
  			the servo motor (and its respective SIU pin initialization).
  \param  	None.
  \return	Null.
*/
void vfnInit_Servo(void);