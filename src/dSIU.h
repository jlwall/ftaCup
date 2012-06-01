
/* Function Prototypes */

/**
  \brief	Initialize an Emios Channel as an output pad.
  \param  	u8PcrVal: Pad Configuration Register Value defined in Driver_MPC5604B.h as PCR_EMIOS_0_tag
  \return	nule.
*/
void vfnInit_Emios_Output_Pad(uint8_t u8PcrVal);

/**
  \brief	Initialize an Emios Channel as an input pad.
  \param  	u8PcrVal: Pad Configuration Register Value defined in Driver_MPC5604B.h as PCR_EMIOS_0_tag
  \return	nule.
*/
void vfnInit_Emios_Intput_Pad(uint8_t u8PcrVal);

/**
  \brief	Initialize an Adc Channel pad.
  \param  	u8PcrVal: Pad Configuration Register Value defined in Driver_MPC5604B.h as PCR_EMIOS_0_tag
  \return	nule.
*/
void vfnInit_Adc_Pad(uint8_t u8PcrVal);
