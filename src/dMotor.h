
/*!
	  \file     Driver_Motor.h
	  \brief    The header file of Driver_Motor.c which includes all the function
	  			prototypes and an explanation of each function.
	  \author   Cuauhtli Padilla
	  \author   Rodrigo Mendoza
	  \author   Marco Trujillo
	  \date     July 16 2010	  
*/

/* Function Prototypes */

/**
  \brief	Sends pulses to the motor equivalent to the percentage u16Perc, with 
  			100 being 100% power (Vcc), 0% is GND, and 50% is equivalent to a 
  			clock signal with 50% duty.  It is polarized normal (forward).
  \param  	u16Perc: The percentage of power for the motor. (100% is maximum
  					 and 0% is minimum).
  \return	Null.
*/
void vfnSet_Motor_Forward(uint16_t u16Perc);

/**
  \brief	Sends pulses to the motor equivalent to the percentage u16Perc, with 
  			100 being 100% power (Vcc), 0% is GND, and 50% is equivalent to a 
  			clock signal with 50% duty.  It is inverse polarized (reverse).
  \param  	u16Perc: The percentage of power for the motor. (100% is maximum
  					 and 0% is minimum).
  \return	Null.
*/
void vfnSet_Motor_Reverse(uint16_t u16Perc);

/**
  \brief	Sends pulses to the motor to stop the motor (which is HIGH to both
  			ends).
  \param  	None.
  \return	Null.
*/
void vfnSet_Motor_Stop(void);

/**
  \brief	Returns the current consumed by the motor by reading an ADC port.
  \param  	None.
  \return	u16Current: An ADC representing the current consumed by the motor.
*/
uint16_t u16Get_Feedback_Current(void);

/**
  \brief	Sets the motor forward to a desired velocity by increasing the PWM
  			signal (more duty) to the motor if the measured velocity is too slow
  			or by decreasing the PWM signal if the measured velocity is too fast.
  			It works with a proportional integral algorithm.
  \param  	i32RegulatedVel: The desired velocity the car must go at, expressed
  			in mm/s.
  \return	Null.
*/
void vfnSet_Motor_Velocity(int32_t i32RegulatedVel);

/**
  \brief	If a SAIC scan is done, this function replaces the last value of
  			i32Period with the period measured, else it does nothing.
  			i32Period is in micro seconds and represents the time it takes
  			to the back wheels to give one revolution because the signal the 
  			SAIC scans has a rising edge each time the wheels give a complete
  			revolution. From this variable velocity can be	measured: 
  			Velocity = Circumference of wheel / i32Period.
  			This function must be called frequently in case the SAIC scan
  			is done, and if it is called long after the SAIC scan is done,
  			it will lose precision.
  			It also checks if an overflow occurs in the MCB so that the i32Period
  			variable is increased by the time to overflow (period of MCB).
  \param  	None.
  \return	Null.
*/
void vfnRead_Period(void);

/**
  \brief	Sums 1 to the variable i16Overflows, which represents the number
  			of times the MCB has an overflow, in order to measure the period
  			accurately.
  			This function must be called each time the MCB has an overflow
  			and is reset.
  \param  	None.
  \return	Null.
*/
void vfnSum_i16Overflows(void);

/**
  \brief	Initializes the GPIO pins for the direction of the motor, the
  			OPWM channel for the enable pin of the motor (and its respective
  			SIU pin initialization), the analog pin for the ADC scan of the 
  			current, and the SAIC channel for the velocimeter (and its respective
  			SIU pin initialization).
  \param  	None.
  \return	Null.
*/
void vfnInit_Motor(void);
/*****************************************************/