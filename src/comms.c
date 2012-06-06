#include "MPC5604B_M07N.h"
#include "comms.h"
#include "main.h"


extern struct CAR car;

U8 rx_data[4],pt;

void printserialhex(U16 innum) {
  U16 j1,in;
  U8 p1,p2;
  in = innum;
   
  j1 = (in & 0x0f);
  if (j1 > 9) p1 = (U8)(j1 + 0x41 - 10);
  else p1 = (U8)(j1 +0x30);
  j1 = (in & 0xf0) >> 4;
  if (j1 > 9) p2 = (U8)(j1 +0x41 - 10);
  else p2 = (U8)(j1 +0x30);
  TransmitCharacter(p2);
  TransmitCharacter(p1);  
}

void printserialsingned(U16 innum) {
  U16 j1,k1,l1,m1,in;
  U8 p1,p2,p3,p4,p5;
 
  if(innum < 0x8000) {
    in = innum;
  	TransmitCharacter('+');    
  } 
  else {
    in = (U16)(~innum);
    //in = 0x7fff - in;
    TransmitCharacter('-');     
  }
  
  j1 = (in / 10);
  p1 = (U8)(in - j1*10 +0x30);
  k1 = (j1 / 10);
  p2 = (U8)(j1 - k1*10 +0x30);
  l1 = (k1 / 10);
  p3 = (U8)(k1 - l1*10 +0x30);
  m1 = (l1 / 10);
  p4 = (U8)(l1 - m1*10 +0x30);
  p5 = (U8)m1 +0x30;
  TransmitCharacter(p5);
  TransmitCharacter(p4);
  TransmitCharacter(p3);
  TransmitCharacter(p2);
  TransmitCharacter(p1);  
  TransmitCharacter(0x09);
}


void printlistall(void) {
   TransmitCharacter(0x0c); 
   TransmitCharacter(0x0a);   
   TransmitCharacter(0x0d);  
   for(pt=0;pt<120;pt+=3){

	if(car.sensor.array[pt]>car.sensor.valMin + car.sensor.threshold)
      TransmitCharacter('_');
	else
		TransmitCharacter('-');
   }
   TransmitCharacter(0x0a);   
   TransmitCharacter(0x0d);   
   
   	TransmitData("\r\nLeft = ");
   printserialsingned(car.sensor.cornLeft);
   
   	TransmitData("\r\nRight = ");
   printserialsingned(car.sensor.cornRight);
   
   TransmitData("\r\nCenter = ");
   printserialsingned(car.sensor.center);
   
   TransmitData("\r\nWidth = ");
   printserialsingned(car.sensor.width);
}


void TransmitCharacter(U8 ch)
{
	LINFLEX_0.BDRL.B.DATA0 = ch;  			/* write character to transmit buffer */
	while (1 != LINFLEX_0.UARTSR.B.DTF) {}  /* Wait for data transmission completed flag */
	LINFLEX_0.UARTSR.R = 0x0002; 			/* clear the DTF flag and not the other flags */	
}

void TransmitData (char TransData[]) 
{
	U8	j,k;                                 /* Dummy variable */
	k = (U8)strlen (TransData);
	for (j=0; j< k; j++) 
	{  /* Loop for character string */

		TransmitCharacter(TransData[j]);  		/* Transmit a byte */		
	}
}

/* This functions polls UART receive buffer. when it is full, it moves received data from the buffer to the memory */
U8 ReadData (void)
{
	U8 ch;
	/* wait for DRF */
	while (1 != LINFLEX_0.UARTSR.B.DRF) {}  /* Wait for data reception completed flag */
		
	/* wait for RMB */
	while (1 != LINFLEX_0.UARTSR.B.RMB) {}  /* Wait for Release Message Buffer */
	
	/* get the data */
	ch = (U8)LINFLEX_0.BDRM.B.DATA4;
		
	/* clear the DRF and RMB flags by writing 1 to them */
	LINFLEX_0.UARTSR.R = 0x0204;
	
	return ch;
	
}

void TRIM_POT_ADC(void)
{
	U16 curdata;
	TransmitData("****Trim Pot****\n\r");
	

	curdata = (U16)ADC.CDR[0].B.CDATA;
	TransmitData("\n\r ADC0 * ");
	printserialsingned(curdata);
	

}


void CAMERA(void)
{
	u8Capture_Pixel_Values();
	printlistall();
}


void SWITCH(void)
{
	SIU.PCR[64].R = 0x0100;				/* Program the drive enable pin of S1 (PE0) as input*/
	SIU.PCR[65].R = 0x0100;				/* Program the drive enable pin of S2 (PE1) as input*/
	SIU.PCR[66].R = 0x0100;				/* Program the drive enable pin of S3 (PE2) as input*/
	SIU.PCR[67].R = 0x0100;				/* Program the drive enable pin of S4 (PE3) as input*/
	TransmitData("****Switch Test****\n\r");
	TransmitData("Press S1 Switch\n\r");
	while((SIU.PGPDI[2].R & 0x80000000) == 0x80000000)
	{
		
	}; /*Wait until S1 switch is pressed*/
	TransmitData("Switch S1 Pressed \n\r");
	TransmitData("Press S2 Switch\n\r");
	while((SIU.PGPDI[2].R & 0x40000000) == 0x40000000)
	{
		
	}; /*Wait until S2 switch is pressed*/
	TransmitData("Switch S2 Pressed \n\r");
	TransmitData("Press S3 Switch\n\r");
	while((SIU.PGPDI[2].R & 0x20000000) == 0x20000000)
	{
		
	}; /*Wait until S3 switch is pressed*/
	TransmitData("Switch S3 Pressed \n\r");
		TransmitData("Press S4 Switch\n\r");
	while((SIU.PGPDI[2].R & 0x10000000) == 0x10000000)
	{
		
	}; /*Wait until S4 switch is pressed*/
	TransmitData("Switch S4 Pressed \n\r");
}

void SERVO(void)
{
	TransmitData("****Steering Servo Test****\n\r");
	EMIOS_0.CH[4].CBDR.R = constServoMiddle;      	/* 1500 Middle */
	TransmitData("Middle\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = constServoMiddle + constServoMax;        /* 1750 Right Max,*/
	TransmitData("Right\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = constServoMiddle - constServoMax;        /* 1250 Left Max*/
	TransmitData("Left\n\r");
	Delaywait();
	EMIOS_0.CH[4].CBDR.R = constServoMiddle;      	/* 1500 Middle */
}

void SendStatusPacket(void)
{
	TransmitCharacter(0x01);   
	TransmitCharacter(0x02);
	TransmitCharacter(15);   //DLC = 15 packets
	
	TransmitCharacter(0x50);  //packet 55
	
	TransmitCharacter(car.sensor.cornLeft);
	TransmitCharacter(car.sensor.cornRight);
	TransmitCharacter(car.sensor.width);
	TransmitCharacter(car.sensor.center);
	
	TransmitCharacter((U8)(car.ctrl.velTarget>>8));
	TransmitCharacter((U8)car.ctrl.velTarget);

	TransmitCharacter((U8)(car.ctrl.targetServoPos>>8));
	TransmitCharacter((U8)car.ctrl.targetServoPos);

	TransmitCharacter((U8)(car.ctrl.biasVelocity>>8));
	TransmitCharacter((U8)car.ctrl.biasVelocity);

	TransmitCharacter((U8)(car.ctrl.error>>8));
	TransmitCharacter((U8)car.ctrl.error);

	TransmitCharacter(car.ctrl.manualMode);
	TransmitCharacter(car.sensor.valid);

	TransmitCharacter(0x55);
	
}

