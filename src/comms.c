#include "MPC5604B_M07N.h"
#include "comms.h"

volatile uint8_t Result[128];                	/* Read converstion result from ADC input ANS0 */

uint8_t rx_data[4],pt;

void printserialhex(uint16_t innum) {
  uint16_t j1,in;
  uint8_t p1,p2;
  in = innum;
   
  j1 = (in & 0x0f);
  if (j1 > 9) p1 = (uint8_t)(j1 + 0x41 - 10);
  else p1 = (uint8_t)(j1 +0x30);
  j1 = (in & 0xf0) >> 4;
  if (j1 > 9) p2 = (uint8_t)(j1 +0x41 - 10);
  else p2 = (uint8_t)(j1 +0x30);
  TransmitCharacter(p2);
  TransmitCharacter(p1);  
}

void printserialsingned(uint16_t innum) {
  uint16_t j1,k1,l1,m1,in;
  uint8_t p1,p2,p3,p4,p5;
 
  if(innum < 0x8000) {
    in = innum;
  	TransmitCharacter('+');    
  } 
  else {
    in = (uint16_t)(~innum);
    //in = 0x7fff - in;
    TransmitCharacter('-');     
  }
  
  j1 = (in / 10);
  p1 = (uint8_t)(in - j1*10 +0x30);
  k1 = (j1 / 10);
  p2 = (uint8_t)(j1 - k1*10 +0x30);
  l1 = (k1 / 10);
  p3 = (uint8_t)(k1 - l1*10 +0x30);
  m1 = (l1 / 10);
  p4 = (uint8_t)(l1 - m1*10 +0x30);
  p5 = (uint8_t)m1 +0x30;
  TransmitCharacter(p5);
  TransmitCharacter(p4);
  TransmitCharacter(p3);
  TransmitCharacter(p2);
  TransmitCharacter(p1);  
  TransmitCharacter(0x09);
}


void printlistall(void) {
   TransmitCharacter(0x0a);   
   TransmitCharacter(0x0d);  
   for(pt=0;pt<120;pt++){
      printserialhex(Result[pt]);
   }
   TransmitCharacter(0x0a);   
   TransmitCharacter(0x0d);   
}


void TransmitCharacter(uint8_t ch)
{
	LINFLEX_0.BDRL.B.DATA0 = ch;  			/* write character to transmit buffer */
	while (1 != LINFLEX_0.UARTSR.B.DTF) {}  /* Wait for data transmission completed flag */
	LINFLEX_0.UARTSR.R = 0x0002; 			/* clear the DTF flag and not the other flags */	
}

void TransmitData (char TransData[]) 
{
	uint8_t	j,k;                                 /* Dummy variable */
	k = strlen (TransData);
	for (j=0; j< k; j++) 
	{  /* Loop for character string */

		TransmitCharacter(TransData[j]);  		/* Transmit a byte */		

	}
}

/* This functions polls UART receive buffer. when it is full, it moves received data from the buffer to the memory */
uint8_t ReadData (void)
{
	uint8_t ch;
	/* wait for DRF */
	while (1 != LINFLEX_0.UARTSR.B.DRF) {}  /* Wait for data reception completed flag */
		
	/* wait for RMB */
	while (1 != LINFLEX_0.UARTSR.B.RMB) {}  /* Wait for Release Message Buffer */
	
	/* get the data */
	ch = (uint8_t)LINFLEX_0.BDRM.B.DATA4;
		
	/* clear the DRF and RMB flags by writing 1 to them */
	LINFLEX_0.UARTSR.R = 0x0204;
	
	return ch;
	
}

void RIGHT_MOTOR_CURRENT(void)
{
	uint8_t i;
	uint32_t curdata;
	TransmitData("****Right Motor Current****\n\r");
	SIU.PGPDO[0].R = 0x00004000;			/* Enable Right the motors */
	Delaywait();
	for (i=0;i <10;i++)
	{
		ADC.MCR.B.NSTART=1;     			/* Trigger normal conversions for ADC0 */
		while (ADC.MSR.B.NSTART == 1) {};
		curdata = ADC.CDR[2].B.CDATA;
		printserialsingned(curdata);		
	}
	SIU.PGPDO[0].R = 0x00000000;		/* Disable Right the motors */
}

void TRIM_POT_ADC(void)
{
	uint8_t i;
	uint32_t curdata;
	TransmitData("****Trim Pot****\n\r");
		for (i=0;i <2;i++)
	{
		ADC.MCR.B.NSTART=1;     			/* Trigger normal conversions for ADC0 */
		while (ADC.MSR.B.NSTART == 1) {};
		curdata = ADC.CDR[0].B.CDATA;
		printserialsingned(curdata);
		curdata = ADC.CDR[1].B.CDATA;
		printserialsingned(curdata);
		curdata = ADC.CDR[2].B.CDATA;
		printserialsingned(curdata);
		curdata = ADC.CDR[3].B.CDATA;
		printserialsingned(curdata);
		curdata = ADC.CDR[4].B.CDATA;
		printserialsingned(curdata);
		curdata = ADC.CDR[5].B.CDATA;
		printserialsingned(curdata);
		curdata = ADC.CDR[6].B.CDATA;
		printserialsingned(curdata);
		curdata = ADC.CDR[7].B.CDATA;
		printserialsingned(curdata);		
	}

}

void LEFT_MOTOR_CURRENT(void)
{
	uint8_t i;
	uint32_t curdata;
	
	TransmitData("****Left Motor Current****\n\r");
	SIU.PGPDO[0].R = 0x00008000;			/* Enable Right the motors */
	Delaywait();
	for (i=0;i <10;i++)
	{
		ADC.MCR.B.NSTART=1;     			/* Trigger normal conversions for ADC0 */
		while (ADC.MSR.B.NSTART == 1) {};
		curdata = ADC.CDR[1].B.CDATA;
		printserialsingned(curdata);		
	}
	SIU.PGPDO[0].R = 0x00000000;		/* Disable Right the motors */
}



void CAMERA(void)
{
	uint8_t i,j;	
	uint32_t adcdata;
	
	TransmitData("****Line Sensor Test****\n\r");
	SIU.PCR[27].R = 0x0200;				/* Program the Sensor read start pin as output*/
	SIU.PCR[29].R = 0x0200;				/* Program the Sensor Clock pin as output*/
	for(j=0;j<2;j++)
	//for(;;)
		{
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
			ADC.MCR.B.NSTART=1;     		/* Trigger normal conversions for ADC0 */
			while (ADC.MCR.B.NSTART == 1) {};
			adcdata = ADC.CDR[0].B.CDATA;
			Delay();
			SIU.PGPDO[0].R &= ~0x00000004;	/* Sensor Clock Low */
			Result[i] = (uint8_t)(adcdata >> 2);		
		}
		Delaycamera();
		//printlistall();
	}
	printlistall();
}

void LED(void)
{
	SIU.PCR[68].R = 0x0200;				/* Program the drive enable pin of LED1 (PE4) as output*/
	SIU.PCR[69].R = 0x0200;				/* Program the drive enable pin of LED2 (PE5) as output*/
	SIU.PCR[70].R = 0x0200;				/* Program the drive enable pin of LED3 (PE6) as output*/
	SIU.PCR[71].R = 0x0200;				/* Program the drive enable pin of LED4 (PE7) as output*/
	TransmitData("****Led Test****\n\r");
	TransmitData("All Led ON\n\r");
	Delayled();
	SIU.PGPDO[2].R |= 0x0f000000;		/* Disable LEDs*/
	SIU.PGPDO[2].R &= 0x07000000;		/* Enable LED1*/
	TransmitData("Led 1 ON\n\r");
	Delayled();
	SIU.PGPDO[2].R |= 0x08000000;		/* Disable LED1*/
	SIU.PGPDO[2].R &= 0x0b000000;		/* Enable LED2*/
	TransmitData("Led 2 ON\n\r");
	Delayled();
	SIU.PGPDO[2].R |= 0x04000000;		/* Disable LED2*/
	SIU.PGPDO[2].R &= 0x0d000000;		/* Enable LED3*/
	TransmitData("Led 3 ON\n\r");
	Delayled();
	SIU.PGPDO[2].R |= 0x02000000;		/* Disable LED3*/
	SIU.PGPDO[2].R &= 0x0e000000;		/* Enable LED4*/
	TransmitData("Led 4 ON\n\r");
	Delayled();
	SIU.PGPDO[2].R |= 0x01000000;		/* Disable LED4*/
}

void SWITCH(void)
{
	SIU.PCR[64].R = 0x0100;				/* Program the drive enable pin of S1 (PE0) as input*/
	SIU.PCR[65].R = 0x0100;				/* Program the drive enable pin of S2 (PE1) as input*/
	SIU.PCR[66].R = 0x0100;				/* Program the drive enable pin of S3 (PE2) as input*/
	SIU.PCR[67].R = 0x0100;				/* Program the drive enable pin of S4 (PE3) as input*/
	TransmitData("****Switch Test****\n\r");
	TransmitData("Press S1 Switch\n\r");
	while((SIU.PGPDI[2].R & 0x80000000) == 0x80000000); /*Wait until S1 switch is pressed*/
	TransmitData("Switch S1 Pressed \n\r");
	TransmitData("Press S2 Switch\n\r");
	while((SIU.PGPDI[2].R & 0x40000000) == 0x40000000); /*Wait until S2 switch is pressed*/
	TransmitData("Switch S2 Pressed \n\r");
	TransmitData("Press S3 Switch\n\r");
	while((SIU.PGPDI[2].R & 0x20000000) == 0x20000000); /*Wait until S3 switch is pressed*/
	TransmitData("Switch S3 Pressed \n\r");
		TransmitData("Press S4 Switch\n\r");
	while((SIU.PGPDI[2].R & 0x10000000) == 0x10000000); /*Wait until S4 switch is pressed*/
	TransmitData("Switch S4 Pressed \n\r");
}

void SERVO(void)
{
	TransmitData("****Steering Servo Test****\n\r");
	EMIOS_0.CH[4].CBDR.R = 1500;      	/* 1500 Middle */
	TransmitData("Middle\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = 1750;        /* 1750 Right Max,*/
	TransmitData("Right\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = 1250;        /* 1250 Left Max*/
	TransmitData("Left\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = 1500;      	/* 1500 Middle */
}


void MOTOR_LEFT(void)
{
	TransmitData("****Left Drive Motor Test****\n\r");
	SIU.PCR[16].R = 0x0200;				/* Program the drive enable pin of Left Motor as output*/
	SIU.PGPDO[0].R = 0x00008000;		/* Enable Left the motors */
	Delaywait();
	SIU.PGPDO[0].R = 0x00000000;		/* Disable Left the motors */
}

void MOTOR_RIGHT(void)
{
	TransmitData("****Right Drive Motor Test****\n\r");
	SIU.PCR[17].R = 0x0200;				/* Program the drive enable pin of Right Motor as output*/
	SIU.PGPDO[0].R = 0x00004000;		/* Enable Right the motors */
	Delaywait();
	SIU.PGPDO[0].R = 0x00000000;		/* Disable Right the motors */
}

