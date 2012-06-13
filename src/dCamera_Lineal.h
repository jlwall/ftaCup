/* Function Prototypes */



void vfnInit_CamLin_Adc(void);
/**
  \brief	Setup configuration for Camera, define modulus counters for clock and start pulse, as well as 
  			Opwm channel for each one.
  \param	None
  \return	Nule
*/
void vfnSetup_CamLin(void);

/**
  \brief	Set configurations to use Ipwm mode of Emios to read output from the camera. Set global variables 
  			to 0, initialize Emios, define modulus counter in case it is not defined yet, and define an Emios 
  			channel as Ipwm with selected polarity.
  \param	None
  \return	Nule
*/
void vfnInit_CamLin_Ipwm(void);												

/**
  \brief	Define the value of an average Pulse Width (line width) from many samples, define the actual 
  			Delta X value as a result of an average of the same samples. 
  \param	u8Channel: Ipwm channel to be analized. 
  			u8Frames: Number of samples the user wish to be done.
  \return	Nule
*/
void Set_Pulse_Width (uint8_t u8Channel, uint8_t u8Frames);

/**
  \brief	Captures or not the value from the edge to the line, as well as the line width depending on the 
  			filter done by TolWidth and TolDelta (percentage) 
  \param	u8Channel: Ipwm channel to be analized. 
  			u8TolDelta: Tolerance (percentage) admitted for the next delta x value in comparision to the 
  					previous in order to be considered as a valid value.
  			u8TolWidth: Tolerance (percentage) admitted for the next line width value in comparision to the 
  					stablished value in order to be considered as a valid value.
  \return	1 for succesfull value recognition. 0 for unsuccesfull value recognition.
*/
uint8_t u8Capture_Line_Values(uint8_t u8Channel, uint8_t u8TolDelta, uint8_t u8TolWidth);

/**
  \brief	Return the last valid value of i16PulseWidth (line width) captured by i8Capture_Line_Values.
  \param	None.
  \return	Last valid value of i16PulseWidth.
*/
uint16_t u16Pulse_Width(void);

/**
  \brief	Return the last valid value of i16Delta_X (delta x) captured by i8Capture_Line_Values.
  \param	None.
  \return	Last valid value of i16Delta_X.
*/
uint16_t u16Delta_X(void);
void u8Capture_Pixel_Values(void);

void taskUpdateCamera(void);
void taskUpdateCameraStart(void);
void taskUpdateCameraEnd(void);


