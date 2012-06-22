void TransmitCharacter(uint8_t ch);
void TransmitData (char TransData[]);
void printlistall(void);


void TransmitCharacter(uint8_t ch);
void TransmitData (char TransData[]) ;
uint8_t ReadData (void);
void CAMERA(void);
void LEFT_MOTOR_CURRENT(void);
void RIGHT_MOTOR_CURRENT(void);
void MOTOR_LEFT(void);
void MOTOR_RIGHT(void);
void TRIM_POT_ADC(void);

//CAN log offload
void SendDataLog(U16 chunk);