#include "driverlib.h"
#include "mymsp430.h"

volatile uint32_t MCLK=0,SMCLK=0;
void Timer_Init(void);


void main( void )
{
	/* Stop watchdog timer */
	WDT_A_hold(WDT_A_BASE);

	Clk_Using_DCO_Init(16000,4000,SMCLK_CLOCK_DIVIDER_2);
	MCLK=UCS_getMCLK();
	SMCLK=UCS_getSMCLK();

	GPIO_setAsOutputPin(GPIO_PORT_P4,GPIO_PIN1);

	Timer_Init();

	__enable_interrupt();

	while(1)
	{

	}
}


void Timer_Init(void)
{
	Timer_A_clearTimerInterrupt(TIMER_A0_BASE);

	Timer_A_initUpModeParam param;
	param.clockSource = TIMER_A_CLOCKSOURCE_SMCLK;
	param.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_64;
        
        /* fcpu=fMCLK=16E6Hz, ftimer=fSMCLK/64= (fMCLK/2)/64 */
        
	param.timerPeriod = 24; //0.2ms
	param.timerInterruptEnable_TAIE= TIMER_A_TAIE_INTERRUPT_ENABLE;
	param.timerClear = TIMER_A_DO_CLEAR;
	param.startTimer= true;
	Timer_A_initUpMode(TIMER_A0_BASE,&param);
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void timer_isr(void)
{
	GPIO_toggleOutputOnPin(GPIO_PORT_P4,GPIO_PIN1);
	Timer_A_clearTimerInterrupt(TIMER_A0_BASE);
}

