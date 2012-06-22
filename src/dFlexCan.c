#include "MPC5604B_M07N.h"

uint32_t  RxCODE;              /* Received message buffer code */
uint32_t RxID;                 /* Received message ID */
uint32_t  RxLENGTH;            /* Recieved message number of data bytes */
uint8_t  RxDATA[8];            /* Received message data string*/
uint32_t RxTIMESTAMP;          /* Received message time */                         

void initCAN_1 (void) {
    uint8_t   i;

  CAN_1.MCR.R = 0x5000003F;       /* Put in Freeze Mode & enable all 64 msg bufs */
  CAN_1.CR.R = 0x04DB0006;        /* Configure for 8MHz OSC, 100KHz bit time */
  for (i=0; i<64; i++) {
    CAN_1.BUF[i].CS.B.CODE = 0;   /* Inactivate all message buffers */
  } 
  
  CAN_1.BUF[0].CS.B.CODE = 8;     /* Message Buffer 0 set to TX INACTIVE */
  
  
  SIU.PCR[42].R = 0x0624;         /* MPC56xxB: Config port B0 as CAN0TX, open drain */
  SIU.PCR[43].R = 0x0100;         /* MPC56xxB: Configure port B1 as CAN0RX */
  
  CAN_1.MCR.R = 0x0000003F;       /* Negate FlexCAN 0 halt state for 64 MB */
}

uint8_t canBufferSetupRx(uint8_t ide, uint16_t msgId, uint8_t busSel, uint8_t buffer)
{
	if(buffer>64)
		return 0;
	
	if(msgId>0x7FF)
		return 0;
	
	if(msgId==0)
		return 0;
	
	if(busSel>2)
		return 0;
	
	if(busSel==0)
	{
	CAN_0.BUF[buffer].CS.B.IDE = ide;      /* MB 4 will look for a standard ID */
  	CAN_0.BUF[buffer].ID.B.STD_ID = msgId; /* MB 4 will look for ID = 555 */
  	CAN_0.BUF[buffer].CS.B.CODE = 4;     /* MB 4 set to RX EMPTY */	  
	}
	else
	{	
	CAN_1.BUF[buffer].CS.B.IDE = ide;      /* MB 4 will look for a standard ID */
  	CAN_1.BUF[buffer].ID.B.STD_ID = msgId; /* MB 4 will look for ID = 555 */
  	CAN_1.BUF[buffer].CS.B.CODE = 4;     /* MB 4 set to RX EMPTY */
	}
	return 1;
}

void initCAN_0 (void) {
  uint8_t   i;

  CAN_0.MCR.R = 0x5000003F;       /* Put in Freeze Mode & enable all 64 msg bufs */
  CAN_0.CR.R = 0x04DB0006;        /* Configure for 8MHz OSC, 100KHz bit time */
  for (i=0; i<64; i++) {
    CAN_0.BUF[i].CS.B.CODE = 0;   /* Inactivate all message buffers */
  } 
  
  CAN_0.BUF[0].CS.B.CODE = 8;     /* Message Buffer 0 set to TX INACTIVE */
  
  
  SIU.PCR[42].R = 0x0624;         /* MPC56xxB: Config port B0 as CAN0TX, open drain */
  SIU.PCR[43].R = 0x0100;         /* MPC56xxB: Configure port B1 as CAN0RX */
  
  CAN_0.MCR.R = 0x0000003F;       /* Negate FlexCAN 0 halt state for 64 MB */
}


uint8_t RecieveMsgB0 ( uint8_t buffer) {
  uint8_t j;
  uint32_t dummy;

  if(CAN_0.IFRL.B.BUF04I == 1)
  {
	  RxCODE   = CAN_0.BUF[buffer].CS.B.CODE;    /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
	  RxID     = CAN_0.BUF[buffer].ID.B.STD_ID;
	  RxLENGTH = CAN_0.BUF[buffer].CS.B.LENGTH;
	  for (j=0; j<RxLENGTH; j++) { 
	    RxDATA[j] = CAN_0.BUF[buffer].DATA.B[j];
	  }
	  RxTIMESTAMP = CAN_0.BUF[buffer].CS.B.TIMESTAMP; 
	  dummy = CAN_0.TIMER.R;                /* Read TIMER to unlock message buffers */    
	  CAN_0.IFRL.R = 0x00000010;            /* Clear CAN 1 MB 4 flag */
	  return 1;
  }
  else
  return 0;
}

uint8_t RecieveMsgB1 ( uint8_t buffer) {
  uint8_t j;
  uint32_t dummy;

  if(CAN_1.IFRL.B.BUF04I == 1)
  {
	  RxCODE   = CAN_1.BUF[buffer].CS.B.CODE;    /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
	  RxID     = CAN_1.BUF[buffer].ID.B.STD_ID;
	  RxLENGTH = CAN_1.BUF[buffer].CS.B.LENGTH;
	  for (j=0; j<RxLENGTH; j++) { 
	    RxDATA[j] = CAN_1.BUF[buffer].DATA.B[j];
	  }
	  RxTIMESTAMP = CAN_1.BUF[buffer].CS.B.TIMESTAMP; 
	  dummy = CAN_1.TIMER.R;                /* Read TIMER to unlock message buffers */    
	  CAN_1.IFRL.R = 0x00000010;            /* Clear CAN 1 MB 4 flag */
	  return 1;
  }
  else
  return 0;
}

void TransmitMsgRef (U8* data, U8 dlc, U8 buff, U16 txId)
 {
  uint8_t	i;
                               
  CAN_1.BUF[buff].CS.B.IDE = 0;           /* Use standard ID length */
  CAN_1.BUF[buff].ID.B.STD_ID = txId;      /* Transmit ID is 555 */
  CAN_1.BUF[buff].CS.B.RTR = 0;           /* Data frame, not remote Tx request frame */
  CAN_1.BUF[buff].CS.B.LENGTH = dlc -1 ; /* # bytes to transmit w/o null */
  for (i=0; i<dlc; i++) {
    CAN_1.BUF[buff].DATA.B[i] = *data++;      /* Data to be transmitted */
  }
  CAN_1.BUF[buff].CS.B.SRR = 1;           /* Tx frame (not req'd for standard frame)*/
  CAN_1.BUF[buff].CS.B.CODE =0xC;         /* Activate msg. buf. to transmit data frame */ 
}