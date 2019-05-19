#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "asdf.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "inc/tm4c123gh6pm.h"

#include "LCD1602A.h"

#include "driverlib/interrupt.h"
#include "driverlib/uart.h"




void uart2_terminal_setup(void){
		//SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
		//system clock was set during the LCD initialization
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
		UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 115200,
        (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
		IntMasterEnable(); //enable processor interrupts
    IntEnable(INT_UART2); //enable the UART interrupt
	
    UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT); //only enable RX and TX interrupts
}

void UARTIntHandler(void)
{
    uint32_t ui32Status;

    ui32Status = UARTIntStatus(UART2_BASE, true); //get interrupt status

    UARTIntClear(UART2_BASE, ui32Status); //clear the asserted interrupts
		set_address(0x00); 
		char recieved; 
    while(UARTCharsAvail(UART2_BASE)) //loop while there are chars
    {
			recieved = UARTCharGet(UART2_BASE);
			if(recieved == 'A') {set_address(0x00);}
			else if(recieved == ' ') set_address(0x40); 
			else if (recieved == 'Z') break; 
			else{
			write_data(recieved); 
			 }
    }
		wait_quarts(1); 
		
}
int main(void) {
	
	  lcdinit(2, 0); 
		displayon(); 
		write_string("Initializing"); 
		next_line();
		write_string("Radar System"); 
		wait_1s(2); 
		clear();
		write_string("   cm");
		next_line();
		write_string("   "); 
		write_data(0xDF); 
	  uart2_terminal_setup(); 

		
		while(1){
    }

}


