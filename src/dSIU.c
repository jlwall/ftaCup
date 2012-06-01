#include "MPC5604B_M07N.h"
#include "Driver_MPC5604B.h"

/** Initialize an Emios Channel as an output pad */
void vfnInit_Emios_Output_Pad(uint8_t u8PcrVal)
{
	SIU.PCR[u8PcrVal].R = 0x0600;      	/* Assign PCR_EMIOS_0_channel as an output emios pad */
}

/** Initialize an Emios Channel as an input pad */
void  vfnInit_Emios_Input_Pad(uint8_t u8PcrVal)
{
	SIU.PCR[u8PcrVal].R = 0x0500;      	/* Assign PCR_EMIOS_0_channel as an input emios pad */
}

/** Initialize an Adc Channel pad */
void vfnInit_Adc_Pad(uint8_t u8PcrVal)
{
	SIU.PCR[u8PcrVal].R = 0x2000;      	/* Assign PCR_ADC_channel as an input adc pad */
}
///////////////////////////////////////////
void vfnInit_Gpio_Out(uint8_t u8PcrVal)
{
	SIU.PCR[u8PcrVal].R = 0x0200;		/* Init GPIO as output */
}
void vfnSet_Gpio(uint8_t u8PcrVal, uint8_t u8Val)
{
	SIU.GPDO[u8PcrVal].B.PDO = u8Val;	/* Set the value of the GPIO (0 or 1) */
}