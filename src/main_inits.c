#include "MPC5604B_M07N.h"
#include "main_inits.h"
#include "dServo.h"
#include "dCamera_Lineal.h"
#include "dMotor.h"
#include "dFlexCan.h"
#include "main.h"
#include "Driver_MPC5604B.h"



extern INTCInterruptsHandlerTable[];
extern IVOR4Handler();
extern uint32_t __IVPR_VALUE; /* Interrupt Vector Prefix vaue from link file*/
extern const vuint32_t IntcIsrVectorTable[];

void initMainHardware(void)
{	
//U16 temp;
	disableIrq();		   	/* Ensure INTC current prority=0 & enable IRQ */


	//initCTU();

	initModesAndClock();
	initPeriClkGen() ;
		initPads ();
	disableWatchdog();
	init_LinFLEX_0_UART ();
	//initCAN_1();             /* Initialize FLEXCAN 0*/
	
		initADC();

	initINTC();			/* Initialize INTC for software vector mode */
	initPIT();		  	/* Initialize PIT1 for 1KHz IRQ, priority 2 */
	initSwIrq4();			/* Initialize software interrupt 4 */
	
	SIU.PSMI[0].R = 1;  //can1rxd=43
	SIU.PSMI[7].R = 2;  //dspi1_sscl=114
	SIU.PSMI[8].R = 2;  //dspi1_sscl=114
	SIU.PSMI[9].R = 3;  //dspi1_sscl=114


EMIOS_0.MCR.B.GPRE= 63;   			/* Divide 64 MHz sysclk by 63+1 = 64 for 1MHz eMIOS clk*/
	EMIOS_0.MCR.B.GPREN = 1;			/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;  			/* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    			/* Enable stopping channels when in debug mode */

	EMIOS_0.CH[0].CADR.R = 14999;   	/* Period will be 19999+1 = 20000 clocks (20 msec)*/
	EMIOS_0.CH[0].CCR.B.MODE = 0x50; 	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;   	/* Use internal counter */
	EMIOS_0.CH[0].CCR.B.UCPRE=0;     	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;   	/* Enable prescaler; uses default divide by 1*/
	EMIOS_0.CH[0].CCR.B.FREN = 1;   	/* Freeze channel counting when in debug mode*/

	EMIOS_0.CH[23].CADR.R = 999;      	/* Period will be 999+1 = 1000 clocks (1 msec)*/
	EMIOS_0.CH[23].CCR.B.MODE = 0x50; 	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[23].CCR.B.BSL = 0x3;   	/* Use internal counter */
	EMIOS_0.CH[23].CCR.B.UCPRE=0;     	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[23].CCR.B.UCPEN = 1;   	/* Enable prescaler; uses default divide by 1*/
	EMIOS_0.CH[23].CCR.B.FREN = 1;   	/* Freeze channel counting when in debug mode*/
	
	EMIOS_0.CH[4].CADR.R = 0;     		/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[4].CBDR.R = 1500;      	/* Trailing edge when channel counter bus=1400 Middle, 1650 Right Max, 1150 Left Max*/
	EMIOS_0.CH[4].CCR.B.BSL = 0x01;  	/* Use counter bus B */
	EMIOS_0.CH[4].CCR.B.EDPOL = 1;  	/* Polarity-leading edge sets output */
	EMIOS_0.CH[4].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[28].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 6 to pad */
	
	
	EMIOS_0.CH[6].CADR.R = 000;     	/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[6].CBDR.R = 950;     	/* Trailing edge when channel counter bus=500*/
	EMIOS_0.CH[6].CCR.B.BSL = 0x0;  	/* Use counter bus A (default) */
	EMIOS_0.CH[6].CCR.B.EDPOL = 1;  	/* Polarity-leading edge sets output */
	EMIOS_0.CH[6].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[30].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 6 to pad */
	
	EMIOS_0.CH[7].CADR.R = 0;    		/* Leading edge when channel counter bus=0*/
	EMIOS_0.CH[7].CBDR.R = 950;     	/* Trailing edge when channel's counter bus=999*/
	EMIOS_0.CH[7].CCR.B.BSL = 0x0; 		/* Use counter bus A (default) */
	EMIOS_0.CH[7].CCR.B.EDPOL = 1; 		/* Polarity-leading edge sets output*/
	EMIOS_0.CH[7].CCR.B.MODE = 0x60; 	/* Mode is OPWM Buffered */
	SIU.PCR[31].R = 0x0600;           	/* MPC56xxS: Assign EMIOS_0 ch 7 to pad */
	
	EMIOS_0.CH[3].CADR.R = 250;      	/* Ch 3: Match "A" is 250 */
	EMIOS_0.CH[3].CBDR.R = 500;      	/* Ch 3: Match "B" is 500 */
	EMIOS_0.CH[3].CCR.R= 0x000000E0; 	/* Ch 3: Mode is OPWMB, time base = ch 23 */
	EMIOS_0.CH[2].CCR.R= 0x01020082; 	/* Ch 2: Mode is SAIC, time base = ch 23 */


    SIU.PCR[17].R = 0x0200;				/* Program the drive enable pin of Right Motor as output*/
	SIU.PCR[16].R = 0x0200;				/* Program the drive enable pin of Left Motor as output*/
	SIU.PGPDO[0].R = 0x00000000;		/* Disable the motors */
	
	
	
	//init SBC for CAN
	//DSPI_1.MCR.R = 0x80010001; /* Configure DSPI_0 as master */
//DSPI_1.CTAR[0].R = 0x780A7727; /* Configure CTAR0 */
//DSPI_1.MCR.B.HALT = 0x0; /* Exit HALT mode: go from STOPPED to RUNNING state*/


//DSPI_1.PUSHR.R = 0x08011D00; /* Transmit data from master to slave SPI with EOQ */
//while(DSPI_1.SR.B.EOQF==0)
//{
	
//};
//DSPI_1.SR.B.EOQF=1;


//temp = (U16)DSPI_1.POPR.R;



//DSPI_1.PUSHR.R = 0x080160C1; /* Transmit data from master to slave SPI with EOQ */
//while(DSPI_1.SR.B.EOQF==0)
//{
	
//};
//DSPI_1.SR.B.EOQF=1;



//DSPI_1.PUSHR.R = 0x080140C0; /* Transmit data from master to slave SPI with EOQ */
//while(DSPI_1.SR.B.EOQF==0)
//{
	
//};
//DSPI_1.SR.B.EOQF=1;


//temp = (U16)DSPI_1.POPR.R;


//DSPI_1.PUSHR.R = 0x08015E18; /* Transmit data from master to slave SPI with EOQ */
//while(DSPI_1.SR.B.EOQF==0)
//{
	
//};
//DSPI_1.SR.B.EOQF=1;
	
	
INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	    	   /* Enable external interrupts */
	
}

void initPads (void) 
{
	SIU.PCR[2].R = 0x0503;           	/* MPC56xxB: Initialize PA[2] as eMIOS[2] input */
	SIU.PCR[3].R = 0x0600;           	/* MPC56xxB: Initialize PA[3] as eMIOS[3] output */
	SIU.PCR[20].B.APC = 1;          	/* MPC56xxB: Initialize PB[8] as ANP0 */
	SIU.PCR[21].B.APC = 1;          	/* MPC56xxB: Initialize PB[8] as ANP1 */
	SIU.PCR[22].B.APC = 1;          	/* MPC56xxB: Initialize PB[8] as ANP2 */
	SIU.PCR[23].B.APC = 1;          	/* MPC56xxB: Initialize PB[8] as ANP3 */
	SIU.PCR[48].B.APC = 1;          	/* MPC56xxB: Initialize PF[0] as ANP4 */
	
	
	//LED OUTS
	SIU.PCR[68].R = 0x0200;				/* Program the drive enable pin of LED1 (PE4) as output*/
	SIU.PCR[69].R = 0x0200;				/* Program the drive enable pin of LED2 (PE5) as output*/
	SIU.PCR[70].R = 0x0200;				/* Program the drive enable pin of LED3 (PE6) as output*/
	SIU.PCR[71].R = 0x0200;				/* Program the drive enable pin of LED4 (PE7) as output*/

	//SWITCH IN
	SIU.PCR[64].R = 0x0100;				/* Program the drive enable pin of S1 (PE0) as input*/
	SIU.PCR[65].R = 0x0100;				/* Program the drive enable pin of S2 (PE1) as input*/
	SIU.PCR[66].R = 0x0100;				/* Program the drive enable pin of S3 (PE2) as input*/
	SIU.PCR[67].R = 0x0100;				/* Program the drive enable pin of S4 (PE3) as input*/
	
		//DIP SWITCH IN
	SIU.PCR[102].R = 0x0100;				/* Program the drive enable pin of S1 (PE0) as input*/
	SIU.PCR[103].R = 0x0100;				/* Program the drive enable pin of S2 (PE1) as input*/
	SIU.PCR[104].R = 0x0100;				/* Program the drive enable pin of S3 (PE2) as input*/
	SIU.PCR[105].R = 0x0100;				/* Program the drive enable pin of S4 (PE3) as input*/
	
	SIU.PCR[28].R = 0x0600;           	/* MPC56xxB: Initialize PB[12] as eMIOS[4] output */
	
	SIU.PCR[113].R = 0x0A04; /* MPC56xxP: Config pad as DSPI_0 SOUT output */
	SIU.PCR[112].R = 0x0903; /* MPC56xxP: Config pad as DSPI_0 SIN input */
	SIU.PCR[114].R = 0x0A04; /* MPC56xxP: Config pad as DSPI_0 SCK output */
	SIU.PCR[115].R = 0x0A04; /* MPC56xxP: Config pad as DSPI_0 PCS0 output */
}

void initADC(void) {
	ADC.MCR.R = 0x80000000;         	/* Initialize ADC scan mode*/
	ADC.NCMR[0].R = 0x00000011;      	/* Select ANP1:2 inputs for normal conversion */
	ADC.CTR[0].R = 0x00008606;       	/* Conversion times for 32MHz ADClock */
    
}

void initCTU(void) {
  //	CTU.EVTCFGR[2].R = 0x00008000;  	 /* Config event on eMIOS Ch 2 to trig ANP[0] */
}




void initModesAndClock(void) {
	ME.MER.R = 0x0000001D;          	/* Enable DRUN, RUN0, SAFE, RESET modes */
	                              		/* Initialize PLL before turning it on: */
										/* Use 1 of the next 2 lines depending on crystal frequency: */
	CGM.FMPLL_CR.R = 0x02400100;    	/* 8 MHz xtal: Set PLL0 to 64 MHz */   
	/*CGM.FMPLL_R = 0x12400100;*/     	/* 40 MHz xtal: Set PLL0 to 64 MHz */   
	ME.RUN[0].R = 0x001F0074;       	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL */
	
	//ME.RUNPC[0].R = 0x00000010; 	  	/* Peri. Cfg. 0 settings: only run in RUN0 mode */
   										/* Use the next lines as needed for MPC56xxB/S: */  	    	
	//ME.PCTL[48].R = 0x0000;         	/* MPC56xxB LINFlex0: select ME.RUNPC[0] */	
	//ME.PCTL[68].R = 0x0000;         	/* MPC56xxB/S SIUL:  select ME.RUNPC[0] */	
	
	ME.RUNPC[1].R = 0x00000010;     	/* Peri. Cfg. 1 settings: only run in RUN0 mode */
	//ME.PCTL[4].R = 0x01; /* MPC56xxB/P/S DSPI0: select ME.RUNPC[1] */
	//ME.PCTL[5].R = 0x01; /* MPC56xxB/P/S DSPI1: select ME.RUNPC[1] */
	//ME.PCTL[16].R = 0x01;           	/* MPC56xxB/P/S FlexCAN0: select ME.RUNPC[1] */	
	//ME.PCTL[17].R = 0x01;           	/* MPC56xxB/P/S FlexCAN0: select ME.RUNPC[1] */	
	ME.PCTL[32].R = 0x01;       		/* MPC56xxB ADC 0: select ME.RUNPC[1] */
  	//ME.PCTL[57].R = 0x01;       		/* MPC56xxB CTUL: select ME.RUNPC[1] */
  	ME.PCTL[48].R = 0x01;           	/* MPC56xxB/P/S LINFlex 0: select ME.RUNPC[1] */
	ME.PCTL[68].R = 0x01;           	/* MPC56xxB/S SIUL:  select ME.RUNPC[1] */
	ME.PCTL[72].R = 0x01;           	/* MPC56xxB/S EMIOS 0:  select ME.RUNPC[1] */
	ME.PCTL[92].R = 0x01;          		/* PIT, RTI: select ME_RUN_PC[1] */	    	
                                  		/* Mode Transition to enter RUN0 mode: */

	                              		/* Mode Transition to enter RUN0 mode: */
	ME.MCTL.R = 0x40005AF0;         	/* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x4000A50F;         	/* Enter RUN0 Mode & Inverted Key */  
	while (ME.GS.B.S_MTRANS) {}     	/* Wait for mode transition to complete */    
	                          			/* Note: could wait here using timer and/or I_TC IRQ */
	while(ME.GS.B.S_CURRENTMODE != 4) {}/* Verify RUN0 is the current mode */
	
	//while (ME.IS.B.I_MTC != 1) {}    /* Wait for mode transition to complete */    
	//ME.IS.R = 0x00000001;           /* Clear Transition flag */ 
}

void initPeriClkGen(void) {
	CGM.SC_DC[0].R = 0x80; 				/* MPC56xxB/S: Enable peri set 1 sysclk divided by 1 */
	CGM.SC_DC[1].R = 0x80;     /* MPC56xxB/S: Enable peri set 2 sysclk divided by 1 */
  	CGM.SC_DC[2].R = 0x80;         		/* MPC56xxB: Enable peri set 3 sysclk divided by 1*/
}

void disableWatchdog(void) {
	SWT.SR.R = 0x0000c520;     			/* Write keys to clear soft lock bit */
  	SWT.SR.R = 0x0000d928; 
  	SWT.CR.R = 0x8000010A;     			/* Clear watchdog enable (WEN) */
}



void init_LinFLEX_0_UART (void) 
{	

	/* enter INIT mode */
	LINFLEX_0.LINCR1.R = 0x0081; 		/* SLEEP=0, INIT=1 */
	
	/* wait for the INIT mode */
	while (0x1000 != (LINFLEX_0.LINSR.R & 0xF000)) {}
		
	/* configure pads */
	SIU.PCR[18].R = 0x0604;     		/* Configure pad PB2 for AF1 func: LIN0TX */
	SIU.PCR[19].R = 0x0100;     		/* Configure pad PB3 for LIN0RX */	
	
	/* configure for UART mode */
	LINFLEX_0.UARTCR.R = 0x0001; 		/* set the UART bit first to be able to write the other bits */
	LINFLEX_0.UARTCR.R = 0x0033; 		/* 8bit data, no parity, Tx and Rx enabled, UART mode */
								 		/* Transmit buffer size = 1 (TDFL = 0 */
								 		/* Receive buffer size = 1 (RDFL = 0) */
	
	/* configure baudrate 115200 */
	/* assuming 64 MHz peripheral set 1 clock */		
	LINFLEX_0.LINFBRR.R = 12;
	LINFLEX_0.LINIBRR.R = 34;
		
	/* enter NORMAL mode */
	LINFLEX_0.LINCR1.R = 0x0080; /* INIT=0 */	
}


asm void initIrqVectors(void) {
  lis	   r3, __IVPR_VALUE@h   /* IVPR value is passed from link file */
  ori      r3, r3, __IVPR_VALUE@l 
  mtivpr   r3									 
}

void initINTC(void) {	
  //INTC.MCR.B.HVEN = 0;       /* Single core: initialize for SW vector mode */
  //INTC.MCR.B.VTES = 0;       /* Single core: Use default vector table 4B offsets */
  //INTC.IACKR.R = (uint32_t) &INTCInterruptsHandlerTable[0];    /* MPC555x: INTC ISR table base */
}




void initPIT(void) {
                            /* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
  PIT.PITMCR.R = 0x00000001;       /* Enable PIT and configure timers to stop in debug mode */
  PIT.CH[1].LDVAL.R = 0x7800;       /* PIT1 timeout = 64000 sysclks x 1sec/64M sysclks = 1 msec */
  PIT.CH[1].TCTRL.R = 0x000000003; /* Enable PIT1 interrupt and make PIT active to count */ 
 
  INTC_InstallINTCInterruptHandler((void *)&Pit1ISR,60,0x03);
  
                              /* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
  PIT.CH[2].LDVAL.R = 300;       /* PIT1 timeout = 64000 sysclks x 1sec/64M sysclks = 1 msec */
  PIT.CH[2].TCTRL.R = 0x000000002; /* Enable PIT1 interrupt and leave PIT inactive to count */ 
 
  INTC_InstallINTCInterruptHandler((void *)&Pit2ISR,61,0x01);
}

void initSwIrq4(void) {
  INTC.PSR[4].R = 2;		/* Software interrupt 4 IRQ priority = 2 */
  INTC_InstallINTCInterruptHandler((void *)&SwIrq4ISR,04,0x02);
}

void enableIrq(void) {
  INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	    	   /* Enable external interrupts */
}

void disableIrq(void) {
  INTC.CPR.B.PRI = 15;          /* Single Core: Lower INTC's current priority */
  asm(" wrteei 0");	    	   /* Enable external interrupts */
}