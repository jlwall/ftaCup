
/* Saic and Ipwm functions polarity parameters */

#define RISING_EDGE 	0
#define FALLING_EDGE 	1

#define LOW				0
#define HIGH			1


/* ADC channel selection masks */

#define ADC_CHANNEL_0  (0x0001)
#define ADC_CHANNEL_1  (0x0002)
#define ADC_CHANNEL_2  (0x0004)
#define ADC_CHANNEL_3  (0x0008)
#define ADC_CHANNEL_4  (0x0010)


/* Pad Configuration Register value for Adc Channels */

#define PCR_ADC_ANP0 			20 /*PB4*/
#define PCR_ADC_ANP1 			21 /*PB5*/
#define PCR_ADC_ANP2 			22 /*PB6*/
#define PCR_ADC_ANP3 			23 /*PB7*/
#define PCR_ADC_ANP4 			48 /*PD0*/
#define PCR_ADC_ANP5 			49 /*PD1*/
#define PCR_ADC_ANP6 			50 /*PD2*/
#define PCR_ADC_ANP7 			51 /*PD3*/
#define PCR_ADC_ANP8 			52 /*PD4*/
#define PCR_ADC_ANP9 			53 /*PD5*/
#define PCR_ADC_ANP10 			54 /*PD6*/
#define PCR_ADC_ANP11 			55 /*PD7*/
#define PCR_ADC_ANP12 			56 /*PD8*/
#define PCR_ADC_ANP13 			57 /*PD9*/
#define PCR_ADC_ANP14 			58 /*PD10*/
#define PCR_ADC_ANP15 			59 /*PD11*/

#define PCR_ADC_ANS0 			24 /*PB8*/
#define PCR_ADC_ANS1 			25 /*PB9*/
#define PCR_ADC_ANS2 			26 /*PB10*/
#define PCR_ADC_ANS3 			27 /*PB11*/
#define PCR_ADC_ANS4 			60 /*PD12*/
#define PCR_ADC_ANS5 			61 /*PD13*/
#define PCR_ADC_ANS6 			62 /*PD14*/
#define PCR_ADC_ANS7 			63 /*PD15*/

#define PCR_ADC_ANX0 			28 /*PB12*/
#define PCR_ADC_ANX1 			29 /*PB13*/
#define PCR_ADC_ANX2 			30 /*PB14*/
#define PCR_ADC_ANX3 			31 /*PB15*/


/* Pad Configuration Register value for Emios Channels */

#define PCR_EMIOS_0_0		0  /*PA0*/
#define PCR_EMIOS_0_1		1  /*PA1*/
#define PCR_EMIOS_0_2		2  /*PA2*/
#define PCR_EMIOS_0_3		3  /*PA3*/
#define PCR_EMIOS_0_4		4  /*PA4*/
#define PCR_EMIOS_0_5		5  /*PA5*/
#define PCR_EMIOS_0_6		6  /*PA6*/
#define PCR_EMIOS_0_7		7  /*PA7*/

#define PCR_EMIOS_0_8		8  /*PA8*/
#define PCR_EMIOS_0_9		9  /*PA9*/
#define PCR_EMIOS_0_10		10 /*PA10*/
#define PCR_EMIOS_0_11		11 /*PA11*/
#define PCR_EMIOS_0_12		44 /*PC12*/
#define PCR_EMIOS_0_13		45 /*PC13*/
#define PCR_EMIOS_0_14		46 /*PC14*/
#define PCR_EMIOS_0_15		47 /*PC15*/

#define PCR_EMIOS_0_16		64 /*PE0*/
#define PCR_EMIOS_0_17		65 /*PE1*/
#define PCR_EMIOS_0_18		66 /*PE2*/
#define PCR_EMIOS_0_19		67 /*PE3*/
#define PCR_EMIOS_0_20		68 /*PE4*/
#define PCR_EMIOS_0_21		69 /*PE5*/
#define PCR_EMIOS_0_22		70 /*PE6*/
#define PCR_EMIOS_0_23		71 /*PE7*/


/* Ctu channel values for Adc channels */

#define CTU_CHANNEL_ANP0		0  	/*Adc Channel 0*/
#define CTU_CHANNEL_ANP1		1  	/*Adc Channel 1*/
#define CTU_CHANNEL_ANP2		2  	/*Adc Channel 2*/
#define CTU_CHANNEL_ANP3		3  	/*Adc Channel 3*/
#define CTU_CHANNEL_ANP4		4  	/*Adc Channel 4*/
#define CTU_CHANNEL_ANP5		5  	/*Adc Channel 5*/
#define CTU_CHANNEL_ANP6		6  	/*Adc Channel 6*/
#define CTU_CHANNEL_ANP7		7  	/*Adc Channel 7*/
#define CTU_CHANNEL_ANP8		8  	/*Adc Channel 8*/
#define CTU_CHANNEL_ANP9		9  	/*Adc Channel 9*/
#define CTU_CHANNEL_ANP10		10  /*Adc Channel 10*/
#define CTU_CHANNEL_ANP11		11  /*Adc Channel 11*/
#define CTU_CHANNEL_ANP12		12	/*Adc Channel 12*/
#define CTU_CHANNEL_ANP13		13 	/*Adc Channel 13*/
#define CTU_CHANNEL_ANP14		14 	/*Adc Channel 14*/
#define CTU_CHANNEL_ANP15		15 	/*Adc Channel 15*/

#define CTU_CHANNEL_ANS0		16 	/*Adc Channel 32*/
#define CTU_CHANNEL_ANS1		17 	/*Adc Channel 33*/
#define CTU_CHANNEL_ANS2		18 	/*Adc Channel 34*/
#define CTU_CHANNEL_ANS3		19 	/*Adc Channel 35*/
#define CTU_CHANNEL_ANS4		20 	/*Adc Channel 36*/
#define CTU_CHANNEL_ANS5		21 	/*Adc Channel 37*/
#define CTU_CHANNEL_ANS6		22 	/*Adc Channel 38*/
#define CTU_CHANNEL_ANS7		23 	/*Adc Channel 39*/


/* Adc channel types values for NCMR and CTR Channel Type parameter */

#define ADC_PRECISION_CHANNEL			0 	/* Precision channel */
#define ADC_EXTENDEDINT_CHANNEL			1 	/* Extended Internal channel */
#define ADC_EXTERNAL_CHANNEL			2 	/* External channel */


/* Adc channel values for NCMR or JCMR Channel Register */

/* Values for Precision Channels */
#define ADC_CHANNEL_ANP0		0x00000001  	/*Adc Channel 0*/
#define ADC_CHANNEL_ANP1		0x00000002  	/*Adc Channel 1*/
#define ADC_CHANNEL_ANP2		0x00000004  	/*Adc Channel 2*/
#define ADC_CHANNEL_ANP3		0x00000008  	/*Adc Channel 3*/
#define ADC_CHANNEL_ANP4		0x00000010  	/*Adc Channel 4*/
#define ADC_CHANNEL_ANP5		0x00000020  	/*Adc Channel 5*/
#define ADC_CHANNEL_ANP6		0x00000040  	/*Adc Channel 6*/
#define ADC_CHANNEL_ANP7		0x00000080  	/*Adc Channel 7*/
#define ADC_CHANNEL_ANP8		0x00000100  	/*Adc Channel 8*/
#define ADC_CHANNEL_ANP9		0x00000200  	/*Adc Channel 9*/
#define ADC_CHANNEL_ANP10		0x00000400  	/*Adc Channel 10*/
#define ADC_CHANNEL_ANP11		0x00000800  	/*Adc Channel 11*/
#define ADC_CHANNEL_ANP12		0x00001000 		/*Adc Channel 12*/
#define ADC_CHANNEL_ANP13		0x00002000 		/*Adc Channel 13*/
#define ADC_CHANNEL_ANP14		0x00004000 		/*Adc Channel 14*/
#define ADC_CHANNEL_ANP15		0x00008000 		/*Adc Channel 15*/

/* Values fo Extended Internal Channels */
#define ADC_CHANNEL_ANS0		0x00000001  	/*Adc Channel 32*/
#define ADC_CHANNEL_ANS1		0x00000002  	/*Adc Channel 33*/
#define ADC_CHANNEL_ANS2		0x00000004  	/*Adc Channel 34*/
#define ADC_CHANNEL_ANS3		0x00000008  	/*Adc Channel 35*/
#define ADC_CHANNEL_ANS4		0x00000010  	/*Adc Channel 36*/
#define ADC_CHANNEL_ANS5		0x00000020  	/*Adc Channel 37*/
#define ADC_CHANNEL_ANS6		0x00000040  	/*Adc Channel 38*/
#define ADC_CHANNEL_ANS7		0x00000080  	/*Adc Channel 39*/

 /* PCR for GPIO */
#define GPIO_PIN_A0				0
#define GPIO_PIN_A1				1
#define GPIO_PIN_A2				2
#define GPIO_PIN_A3				3
#define GPIO_PIN_A4				4
#define GPIO_PIN_A5				5
#define GPIO_PIN_A6				6
#define GPIO_PIN_A7				7
#define GPIO_PIN_A8				8
#define GPIO_PIN_A9				9
#define GPIO_PIN_A10			10

#define GPIO_PIN_D0				48
#define GPIO_PIN_D1				49
#define GPIO_PIN_D2				50
#define GPIO_PIN_D3				51
#define GPIO_PIN_D4				52
#define GPIO_PIN_D5				53
#define GPIO_PIN_D6				54

#define GPIO_PIN_E0				64
#define GPIO_PIN_E1				65
#define GPIO_PIN_E2				66
#define GPIO_PIN_E3				67
#define GPIO_PIN_E4				68
#define GPIO_PIN_E5				69
#define GPIO_PIN_E6				70
