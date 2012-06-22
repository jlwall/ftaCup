#include "MPC5604B_M07N.h"
#include "comms.h"


void initMainHardware(void);

void initPads (void);
void initADC(void);
void initSBC (void);



void initModesAndClock(void);
void initPeriClkGen(void) ;
void disableWatchdog(void);
void init_LinFLEX_0_UART (void);


asm void initIrqVectors(void);

void initPeriClkGen(void);

void initINTC(void);

void initPIT(void);

void enableIrq(void);

void disableIrq(void);