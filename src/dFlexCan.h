#include "MPC5604B_M07N.h"

void initCAN_1 (void);
void initCAN_0 (void);

void TransmitMsg (void);
uint8_t RecieveMsgB0 ( uint8_t buffer);
uint8_t RecieveMsgB1 ( uint8_t buffer);

uint8_t canBufferSetupRx(uint8_t ide, uint16_t msgId, uint8_t busSel, uint8_t buffer);

void TransmitMsgRef (U8* data, U8 dlc, U8 buff, U16 txId);