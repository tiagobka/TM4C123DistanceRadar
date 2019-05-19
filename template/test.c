#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#define PWM_FREQUENCY 55
void delayMS(uint32_t ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}

int
main(void)
{    
		unsigned long ulPeriod;
		volatile uint32_t ui32Load;
		volatile uint32_t ui32PWMClock;
		volatile uint8_t ui8Adjust;
		ui8Adjust = 70;
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
		SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	
		SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		
	//Set PD0 as PWM
		GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
		GPIOPinConfigure(GPIO_PD0_M1PWM0);
	
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	
	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);
	
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);
	
	while(1)
	{
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
		{
		ui8Adjust--;
		if (ui8Adjust < 23)
		{
		ui8Adjust = 23;
		}
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
		delayMS(50);
		}
		
		if(GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
		{
		ui8Adjust++;
		if (ui8Adjust > 120)
		{
		ui8Adjust = 120;
		}
		PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 1000);
		delayMS(50);
		}
	}
}