
#include "MPC5604B_M07N.h"
#include "Driver_MPC5604B.h"

/****************** Global Variables *************************/
uint16_t u16Counter;					/* Used to count Saic events in u16Pulse_Counter() */
/*************************************************************/


/**************************************************************************************************************/
/*            							 	SETUP FUNCTIONS								                       */
/**************************************************************************************************************/

/** Main Configuration Register to setup Emios **/
void vfnSetup_Emios_0(void)
{
	EMIOS_0.MCR.B.GPRE= 63;   							/* Divide 64 MHz sysclk by 15+1 = 16 for 4MHz eMIOS clk*/
  	EMIOS_0.MCR.B.GTBE = 1;   							/* Enable global time base */
  	EMIOS_0.MCR.B.FRZ = 1;    							/* Enable stopping channels when in debug mode */
}

/** Main Configuration Register to initialize Emios **/
void vfnInit_Emios_0(void)
{
  	EMIOS_0.MCR.B.GPREN = 1;  							/* Enable eMIOS clock, start counting */
}

/** Define Emios Channel as Modulus Counter **/
void vfnInit_Emios_0_Mcb(uint8_t u8Channel, uint16_t u16Period)
{
  	EMIOS_0.CH[u8Channel].CADR.R      = u16Period;	    /* Period will be u16Period clocks (usec) */
  	EMIOS_0.CH[u8Channel].CCR.B.MODE  = 0x50; 			/* Set as Modulus Up Counter Buffered (MCB) */
 	EMIOS_0.CH[u8Channel].CCR.B.UCPRE = 3;    			/* Set channel prescaler divide by 3+1=4 (to 1Mhz) */
  	EMIOS_0.CH[u8Channel].CCR.B.UCPEN = 1;    			/* Enable prescaler*/
  	EMIOS_0.CH[u8Channel].CCR.B.FREN  = 1; 				/* Freeze channel counting when in debug mode */
}

/** Define Emios Channel as Opwm **/
void vfnInit_Emios_0_Opwm(uint8_t u8Channel, uint16_t u16A, uint16_t u16B)
{								
  	EMIOS_0.CH[u8Channel].CADR.R = u16A;        		/* Leading edge when channel counter bus=u16A */
  	EMIOS_0.CH[u8Channel].CBDR.R = u16B;        		/* Trailing edge when channel's counter bus=u16B */
  	EMIOS_0.CH[u8Channel].CCR.B.BSL = 0x1;				/* Use counter bus B,C,D,or E */
  	EMIOS_0.CH[u8Channel].CCR.B.EDPOL = 1;				/* Polarity-leading edge sets output/trailing clears */
  	EMIOS_0.CH[u8Channel].CCR.B.MODE = 0x60; 			/* Mode is OPWM Buffered */
}

/** Define Emios Channel as Ipwm **/
void vfnInit_Emios_0_Ipwm(uint8_t u8Channel, uint8_t u8Pol)
{
	EMIOS_0.CH[u8Channel].CCR.B.BSL = 0x1;				/* Use counter bus B,C,D,or E */
	EMIOS_0.CH[u8Channel].CCR.B.EDPOL = u8Pol;			/* u8Pol selects polarity the Ipwm must capture */
//  EMIOS_0.CH[u8Channel].CCR.B.FCK = 0;	    		/* Input filter bit clock reference is the system clock*/
//  EMIOS_0.CH[u8Channel].CCR.B.IF = 1;				    /* Input filter bit of 2 clock cycles*/ 
	EMIOS_0.CH[u8Channel].CCR.B.MODE = 0b0000100; 		/* Mode is IPWM Input Pulse Width Measurement */
	EMIOS_0.CH[u8Channel].CCR.B.FREN = 1;	    		/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[u8Channel].CCR.B.DMA = 0;	  			/* Flag/overrun assigned to interrupt request */
  	EMIOS_0.CH[u8Channel].CCR.B.FEN = 0;	    		/* FLAG does not generate an interrupt request or a CTU trigger*/
}

/** Define Emios Channel as Ipwm for Ctu purpose **/
void vfnInit_Emios_0_Ipwm_Ctu(uint8_t u8Channel, uint8_t u8Pol)
{
	EMIOS_0.CH[u8Channel].CCR.B.BSL = 0x1;				/* Use counter bus B,C,D,or E */
	EMIOS_0.CH[u8Channel].CCR.B.EDPOL = u8Pol;			/* u8Pol selects polarity the Ipwm must capture */
//  EMIOS_0.CH[u8Channel].CCR.B.FCK = 0;	    		/* Input filter bit clock reference is the system clock*/
//  EMIOS_0.CH[u8Channel].CCR.B.IF = 1;				    /* Input filter bit of 2 clock cycles*/ 
	EMIOS_0.CH[u8Channel].CCR.B.MODE = 0b0000100; 		/* Mode is IPWM Input Pulse Width Measurement */
	EMIOS_0.CH[u8Channel].CCR.B.FREN = 1;	    		/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[u8Channel].CCR.B.DMA = 1;	  			/* Flag/overrun assigned to CTU trigger */
  	EMIOS_0.CH[u8Channel].CCR.B.FEN = 1;	    		/* Flag will generate an interrupt request or a Ctu trigger */
}  

/** Define Emios Channel as Saic **/
void vfnInit_Emios_0_Saic(uint8_t u8Channel, uint8_t u8Pol)
{
	u16Counter=0;										/* Sets pulse counter to 0*/

	EMIOS_0.CH[u8Channel].CCR.B.BSL = 0x1; 				/* Use counter bus A which is eMIOS Ch 23 */
	EMIOS_0.CH[u8Channel].CCR.B.EDPOL = u8Pol; 			/* u8Pol selects polarity that Saic must capture */
	EMIOS_0.CH[u8Channel].CCR.B.EDSEL = 0; 				/* Edge Select- Single edge trigger (count) */
//	EMIOS_0.CH[u8Channel].CCR.B.FCK = 1; 				/* Input filter will use main clock */
//	EMIOS_0.CH[u8Channel].CCR.B.IF = 1; 				/* Input filger uses 2 clock periods */
	EMIOS_0.CH[u8Channel].CCR.B.MODE = 2; 				/* Mode is Saic */
}


/** Define Emios Channel as Saic for Ctu purpose **/
void vfnInit_Emios_0_Saic_Ctu (uint8_t u8Channel, uint8_t u8Pol)
{
	u16Counter=0;										/* Sets pulse counter to 0*/

	EMIOS_0.CH[u8Channel].CCR.B.BSL = 0x1; 				/* Use counter bus A which is eMIOS Ch 23 */
	EMIOS_0.CH[u8Channel].CCR.B.EDPOL = u8Pol;			/* u8Pol selects polarity that Saic must capture */
//	EMIOS_0.CH[u8Channel].CCR.B.FCK = 1; 				/* Input filter will use main clock */
//	EMIOS_0.CH[u8Channel].CCR.B.IF = 1; 				/* Input filger uses 2 clock periods */
	EMIOS_0.CH[u8Channel].CCR.B.MODE = 2; 				/* Mode is Saic */
	EMIOS_0.CH[u8Channel].CCR.B.DMA = 1; 				/* Flag/overrun assigned to CTU trigger */
	EMIOS_0.CH[u8Channel].CCR.B.FEN = 1; 				/* Flag will generate an interrupt request or a Ctu trigger */
}


/**************************************************************************************************************/
/*           								  	USEFUL FUNCTIONS						                      */
/**************************************************************************************************************/

/** Stablish Duty Cicle in clock pulses for an Emios channel **/
void vfnSet_Duty_Opwm(uint8_t u8Channel, uint16_t u16Duty) 
{
	EMIOS_0.CH[u8Channel].CBDR.R = u16Duty;     					/* Trailing edge when channel counter bus = u16Duty */
}

/** Stablish Duty Cicle in percentage for an Emios channel **/
void vfnSet_Duty_Perc_Opwm(uint8_t u8Channel, uint16_t u16DutyPerc, uint16_t u16McbChannel)
{
	uint16_t u16Period;
	u16Period = (uint16_t)(EMIOS_0.CH[u16McbChannel].CADR.R + 1);	/* Takes the period from the channel used as MCB */
	vfnSet_Duty_Opwm(u8Channel, (uint16_t)(u16DutyPerc * u16Period / 100));
}

/** Limits and returns the value of counter when Saic flag occur for a channel of Emios module **/
uint16_t u16Read_Saic(uint8_t u8Channel,uint16_t u16MinVal,uint16_t u16MaxVal)
{
	uint16_t u16Saic;												/* Define local variable */
	if (EMIOS_0.CH[u8Channel].CSR.B.FLAG == 1) 						/* Check flag activation */
	{
		u16Saic = (uint16_t)EMIOS_0.CH[u8Channel].CADR.R; 			/* Read Captured input*/
		EMIOS_0.CH[u8Channel].CSR.B.FLAG = 1; 						/* Clear flag */
			
		if((u16Saic<u16MaxVal) & (u16Saic>u16MinVal))				/* Apply limits to filter the capture */
		{
			return u16Saic; 										/* Return filtered value of the counter */
		}
		else
		{
			return 0;												/* Return 0 if captured value didn't pass the filter*/
		}
		
	}
	else
	{
		return 0;													/* Return 0 if flag still unactivated */
	}
}

/** Returns the number of times a pulse has been detected in a Saic channel */
uint16_t u16Pulse_Counter(uint8_t u8Channel)						/* Function that returns the number of times a pulse has been detected */
{
	if (EMIOS_0.CH[u8Channel].CSR.B.FLAG == 1) 						/* If flag at end of window */
	{
		u16Counter++; 												/* Read Captured input*/
		EMIOS_0.CH[u8Channel].CSR.B.FLAG = 1; 						/* Clear flag */
		
	}
	return 	u16Counter;												/* Return the value of u16Counter */
}

/** Sets to 0 the value of the pulse counter */
void Restart_Pulse_Counter(void)									
{
	u16Counter=0;													/* Sets to 0 the value of the pulse counter */
}

/** Restart counter from selected modulus counter channel **/
void Restart_Modulus_Counter(uint8_t u8Channel)											
{
	EMIOS_0.CH[u8Channel].CCNTR.B.CCNTR=0;							/* Set modulus counter to 0 */
}

/** Returns the counter value of the MCB of the channel selected */
uint16_t u16Get_Counter(uint8_t u8Channel)
{
	uint16_t u16Cntr;
	u16Cntr = (uint16_t)(EMIOS_0.CH[u8Channel].CCNTR.B.CCNTR);
	return u16Cntr;
}

/** Returns the period of the MCB of the channel selected */
uint16_t u16Get_Period_Mcb(uint16_t u16McbChannel)
{
	uint16_t u16Period;
	u16Period = (uint16_t)(EMIOS_0.CH[u16McbChannel].CADR.R + 1);	/* Takes the period from the channel used as MCB */
	return u16Period;
}