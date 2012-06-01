#include "typedefs.h"
#include "MPC5604B_M07N.h"
#include "comms.h"

void task_1msec(void);
void task_2msec(void);
void task_5msec(void);
void task_10msec(void);
void task_40msec(void);
void task_100msec(void);
void task_500msec(void);
void task_1000msec(void);

void Pit1ISR(void);
void SwIrq4ISR(void);

struct CAR_CTRL {

S16 targetServoPos;
S16 targetVelocity;
} ;