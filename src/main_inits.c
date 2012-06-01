#include "MPC5604B_M07N.h"
#include "main_inits.h"
#include "dServo.h"
#include "dCamera_Lineal.h"
#include "dMotor.h"
#include "dFlexCan.h"
#include "main.h"


extern INTCInterruptsHandlerTable[];
extern IVOR4Handler();
extern uint32_t __IVPR_VALUE; /* Interrupt Vector Prefix vaue from link file*/
extern const vuint32_t IntcIsrVectorTable[];

void initMainHardware(void)
{	
	disableIrq();		   	/* Ensure INTC current prority=0 & enable IRQ */
	initPads ();
	initADC();
	initCTU();

	initModesAndClock();
	initPeriClkGen() ;
	disableWatchdog();
	init_LinFLEX_0_UART ();
	initCAN_0();             /* Initialize FLEXCAN 0*/

	initINTC();			/* Initialize INTC for software vector mode */
	initPIT();		  	/* Initialize PIT1 for 1KHz IRQ, priority 2 */
	initSwIrq4();			/* Initialize software interrupt 4 */
	
	vfnInit_CamLin_Ipwm();
	vfnInit_Servo();
	vfnInit_Motor();
	
	enableIrq();		   	/* Ensure INTC current prority=0 & enable IRQ */
	
}

void initPads (void) {
	SIU.PCR[2].R = 0x0503;           	/* MPC56xxB: Initialize PA[2] as eMIOS[2] input */
	SIU.PCR[3].R = 0x0600;           	/* MPC56xxB: Initialize PA[3] as eMIOS[3] output */
	SIU.PCR[20].R = 0x2000;          	/* MPC56xxB: Initialize PB[4] as ANP0 */
	SIU.PCR[21].R = 0x2000;          	/* MPC56xxB: Initialize PB[5] as ANP1 */
	SIU.PCR[22].R = 0x2000;          	/* MPC56xxB: Initialize PB[6] as ANP2 */
	
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
}

void initADC(void) {
	ADC.MCR.R = 0x20020000;         	/* Initialize ADC scan mode*/
	//ADC.MCR.R = 0x00000000;         	/* Initialize ADC one shot mode*/
	ADC.NCMR[0].R = 0x0000000F;      	/* Select ANP1:2 inputs for normal conversion */
	ADC.CTR[0].R = 0x00008606;       	/* Conversion times for 32MHz ADClock */
}

void initCTU(void) {
  	CTU.EVTCFGR[2].R = 0x00008000;  	 /* Config event on eMIOS Ch 2 to trig ANP[0] */
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
	ME.PCTL[16].R = 0x01;           	/* MPC56xxB/P/S FlexCAN0: select ME.RUNPC[1] */	
    ME.PCTL[17].R = 0x01;           	/* MPC56xxB/S FlexCAN1:  select ME.RUNPC[1] */	
	ME.PCTL[32].R = 0x01;       		/* MPC56xxB ADC 0: select ME.RUNPC[1] */
  	ME.PCTL[57].R = 0x01;       		/* MPC56xxB CTUL: select ME.RUNPC[1] */
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
  PIT.CH[1].LDVAL.R = 64000;       /* PIT1 timeout = 64000 sysclks x 1sec/64M sysclks = 1 msec */
  PIT.CH[1].TCTRL.R = 0x000000003; /* Enable PIT1 interrupt and make PIT active to count */ 
 
  INTC_InstallINTCInterruptHandler((void *)&Pit1ISR,60,0x01);
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
  INTC.CPR.B.PRI = 0;          /* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	    	   /* Enable external interrupts */
}