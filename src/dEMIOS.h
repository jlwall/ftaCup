


/* Function Prototypes */


/**
  \brief	Enable Emios clock, configure prescaler to generate 4 MHz Emios clock, enable global time base,
  			enable Freezing channel to freeze them when in debug mode. 
  \param  	None
  \return	Nule
*/
void vfnSetup_Emios_0(void);

/**
  \brief	Enable Emios counters to start pulse generation and processing. 
  \param  	None.
  \return	Nule
*/
void vfnInit_Emios_0(void);

/**
  \brief	Define Emios channel as Modulus up counter buffered with the selected period, configure preescaler
  			to produce 1 MHz time base. 
  \param  	u8Channel: Emios channel to be configured as Counter (0,8,16,23,24).
  			u16Period: Sets A register to stablish period in clock pulses.
  \return	Nule
*/
void vfnInit_Emios_0_Mcb(uint8_t u8Channel, uint16_t u16Period);

/**
  \brief	Define Emios channel as positive Opwm with time base corresponding to the counter bus B,C,D,or E
  			and stablish its raising and falling edge. 
  \param  	u8Channel: Emios channel to be configured as Opwm.
  			u16A: Sets A register to stablish leading edge. 
  			u16B: Sets B register to stablish trailing edge.
  \return	Nule
*/
void vfnInit_Emios_0_Opwm(uint8_t u8Channel, uint16_t u16A, uint16_t u16B);

/**
  \brief	Define Emios channel as Ipwm with time base corresponding to the counter bus B,C,D,or E, allow 
  			channel freezing  and set wished polarity. 
  \param  	u8Channel: Emios channel to be configured as Opwm.
  			u8Pol: selects polarity the Ipwm must capture. 
  \return	Nule
*/
void vfnInit_Emios_0_Ipwm(uint8_t u8Channel, uint8_t u8Pol);

/**
  \brief	Define Emios channel as Ipwm for Ctu purpose with time base corresponding to the counter bus B,C,D,
  			or E, allow channel freezing  and set wished polarity. 
  \param  	u8Channel: Emios channel to be configured as Opwm.
  			u8Pol: selects polarity the Ipwm must capture. 
  \return	Nule
*/
void vfnInit_Emios_0_Ipwm_Ctu(uint8_t u8Channel, uint8_t u8Pol); 

/**
  \brief	Define Emios channel as Saic with time base corresponding to the counter bus B,C,D, or E, allow 
  			channel freezing  and set wished polarity. 
  \param  	u8Channel: Emios channel to be configured as Opwm.
  			u8Pol: selects polarity the Ipwm must capture. 
  \return	Nule
*/
void vfnInit_Emios_0_Saic(uint8_t u8Channel, uint8_t u8Pol);

/**
  \brief	Define Emios channel as Saic for Ctu purpose with time base corresponding to the counter bus B,C,D,
  			or E, allow channel freezing  and set wished polarity. 
  \param  	u8Channel: Emios channel to be configured as Opwm.
  			u8Pol: selects polarity the Ipwm must capture. 
  \return	Nule
*/
void vfnInit_Emios_0_Saic_Ctu (uint8_t u8Channel, uint8_t u8Pol);

/**
  \brief	Stablish Duty Cicle in counter pulses for an Emios channel.
  \param  	u8Channel: Emios channel configured as Opwm.
  			u16Duty: Sets B register to stablish trailing edge. 
  \return	Nule
*/
void vfnSet_Duty_Opwm(uint8_t u8Channel, uint16_t u16Duty);

/**
  \brief	Stablish Duty Cicle in a percentage for an Emios channel.
  \param  	u8Channel: Emios channel configured as Opwm.
  			u16DutyPerc: Duty cycle in a percentage (0-100). 
  			u16McbChannel: Emios channel used as counter bus time base.
  \return	Nule
*/
void vfnSet_Duty_Perc_Opwm(uint8_t u8Channel, uint16_t u16DutyPerc, uint16_t u16McbChannel);

/**
  \brief	Limits and returns the value of counter when Saic flag occur for a channel of Emios module.
  \param	u8Channel: Duty cycle in a percentage (0-100). 
  			u16MinVal: Minimum accepted read value.
  			u16MaxVal: Maximum accepted read value.
  \return	Value of counter when Saic event occured.
*/
uint16_t u16Read_Saic(uint8_t u8Channel,uint16_t u16MinVal,uint16_t u16MaxVal);

/**
  \brief	Count the time a Saic event occur in a Emaios Saic channel. 
  \param	u8Channel: Select channel stablished as Saic. 
  \return	Total of times that the Saic event occured.
*/
uint16_t u16Pulse_Counter(uint8_t u8Channel);

/**
  \brief	Sets to 0 the value of the pulse counter. 
  \param	None. 
  \return	Nule.
*/
void Restart_Pulse_Counter(void);

/**
  \brief	Restart counter from selected modulus counter channel.
  \param	u8Channel: Modulus counter channel to be restarted.
  \return	Nule.
*/
void Restart_Modulus_Counter(uint8_t u8Channel);