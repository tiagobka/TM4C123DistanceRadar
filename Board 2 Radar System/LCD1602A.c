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
void portinit_lcd(void){
	//initialize Port B, E as GPIO ports
		//as digital outputs
		SYSCTL_RCGCGPIO_R |= 0x12; 
		 
		GPIO_PORTB_AMSEL_R &= ~0xFF; 
		GPIO_PORTE_AMSEL_R &= ~0x07;
		
		
		GPIO_PORTB_AFSEL_R &= ~0xFF; 
		GPIO_PORTE_AFSEL_R &= ~0x07;	
		
		GPIO_PORTB_DIR_R |= 0xFF; 
		GPIO_PORTE_DIR_R |= 0x07;
	
		GPIO_PORTB_DEN_R |= 0xFF; 
		GPIO_PORTE_DEN_R |= 0x07;
	
		GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF;
		GPIO_PORTE_PCTL_R &= ~0x00000FFF;
	
		GPIO_PORTB_DR8R_R |= 0xFF;      // enable 8 mA drive
		GPIO_PORTE_DR8R_R |= 0x07;
}
void write_data(uint8_t data){ //write character in 8-bit code
		RS = 0x01; 
		RW = 0x00; 
		SysCtlDelay(120); 
		E = 0x04; 
		SysCtlDelay(120); 
		Data = data; 
		SysCtlDelay(120); 
		E = 0x00; 
		SysCtlDelay(700); 
}

void initialize_lcd(uint8_t lines, uint8_t font){  
	//lines: 1- 1 line, 2- 2 line
	//font: 0- 5x8 dots, 1- 5x10 dots
	SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //50MHz sys clk
	volatile uint32_t i = SysCtlClockGet(); 
	E = 0x00;
	RS = 0x00; RW = 0x00; 
	
	//wake up 1
	SysCtlDelay(251000); //15ms delay
	E = 0x04; 
	SysCtlDelay(120);
	Data = 0x30; SysCtlDelay(700); 
	E= 0x00; 
	//wake up 2
	SysCtlDelay(70000); //4.1ms delay
	E = 0x04; 
	SysCtlDelay(120);
	Data = 0x30; SysCtlDelay(700);	
	E= 0x00;
	//wake up 3
	SysCtlDelay(1800); //100us delay
	E = 0x04;
	SysCtlDelay(120);	
	Data = 0x30; 
	SysCtlDelay(120);
	E= 0x00;
	
	//function set, 2 lines,
	if(lines == 2){
	SysCtlDelay(700); 
	E = 0x04; 
	SysCtlDelay(120);
	Data = 0x38;
	SysCtlDelay(120);
	E = 0x00; 
	}
	else{
		if(font == 1){
			SysCtlDelay(700); 
			E = 0x04; 
			SysCtlDelay(120);
			Data = 0x34;
			SysCtlDelay(120);
			E = 0x00;
		}
		else{
			SysCtlDelay(700); 
			E = 0x04; 
			SysCtlDelay(120);
			Data = 0x30;
			SysCtlDelay(120);
			E = 0x00;
	}
	}

	SysCtlDelay(700); //>37us delay
	E = 0x04;
	SysCtlDelay(120);
	Data = 0x08; 
	SysCtlDelay(120); 
	E = 0x00;
	
	SysCtlDelay(700);	
	E = 0x04;
	SysCtlDelay(120);
	Data = 0x06;
	SysCtlDelay(120);	
	E = 0x00; 
	SysCtlDelay(700);
	rethome(); 
		clear();
		
}

void clear(void){ //clears display data, sets cursor to 00 address
	RS = 0x00; RW = 0x00; 
	
	E = 0x04; SysCtlDelay(120); 
	Data = 0x01;SysCtlDelay(120); 
	E = 0x00; SysCtlDelay(50000); 
}


void displayoff(void){ //turns off display, keeps data and current data address
	RS = 0x00; RW = 0x00;
	E = 0x04; 
	SysCtlDelay(120); 
	Data = 0x08;  
	SysCtlDelay(120);
	E = 0x00;
	SysCtlDelay(1000);
}
void displayon(void){ //turns on display
	RS = 0x00; RW = 0x00;
	
	E = 0x04; 
	SysCtlDelay(120); 
	Data = 0x0C;
	SysCtlDelay(120);
	E = 0x00;
	SysCtlDelay(1000);
}
void EMS(void){  //entry mode set
	//todo: modify this one
	//options: increment when write, decriment when write, shift display when write, dont shift display when write
	RS = 0x00; RW = 0x00; 
	
	E = 0x04; 
	Data = 0x06;  //0x06, increment when write, no shift when write
	E = 0x00; 
}
void cursoron(void){ //turns cursor on, with blinking
	//todo: modify, for option: blinking cursor, not blinking cursor
	RS= 0x00; RW = 0x00; 
	SysCtlDelay(700);
	E = 0x04; 
	SysCtlDelay(120);
	Data = 0x0F;
	SysCtlDelay(120);
	E = 0x00; 
	SysCtlDelay(700);
	
}
void cursoroff(void){  //tursn cursor off
	displayon(); 
}
void rethome(void){   //on line, return cursor to address 00
	RS = 0x00; RW = 0x00; 
	
	E = 0x04;	SysCtlDelay(120); 
	Data = 0x02;SysCtlDelay(120); 
	E = 0x00; SysCtlDelay(2850); 
}

void write_string(char *string){  //writes string on one line, 1602a has 40 spaces to write in each line

	while(*string){
		write_data(*string);
		string++; 
	}
}

void write_int(uint32_t num){  //writes a 32-bit integer 
	char *str; 
	sprintf(str, "%d", num);
	write_string(str); 
}

void wait_1s(uint8_t times){ //wait times * 1 second 
	while(times > 0){
		SysCtlDelay(16666667);
		times--; 
	}
}
void wait_halfs(uint8_t times){  //wait times * 0.5 secondes
	while(times > 0){
		SysCtlDelay(8333334); 
		times--; 
	}
}
void wait_quarts(uint8_t times){  //wait times * 0.25 seconds
	while(times > 0){
		SysCtlDelay(4166666); 
		times--; 
	}
}
void lcdinit(uint8_t lines, uint8_t font){  //initialize entire display, 
	//lines: 1- 1 line, 2- 2 lines.
	//font: 0- 5x8, 1- 5x10
	//uses PORTB 7-0, PORTE 0,1,2  
		portinit_lcd();  //initialize GPIO ports and 1602a wake up sequence 
		initialize_lcd(lines, font); 
		displayon(); 
}

void set_address(uint8_t address){
	RS = 0; RW = 0; 
	E = 0x04; 
	SysCtlDelay(700); 
	Data = address & 0x7F; 
	Data |= 0x80; 
	SysCtlDelay(120); 
	E = 0x00; 
	SysCtlDelay(700); 
}

void next_line(void){
	set_address(0x40); 
}

void shift_cursor(char LR, uint8_t amount){
	RS = 0; RW = 0; 
	
	if(LR == 'L'){
		while (amount > 0){
		E = 0x04; 
		SysCtlDelay(700); 
		Data = 0x10; 
		SysCtlDelay(120); 
		E = 0x00; 
		SysCtlDelay(700);
		amount--;
		}
	}
	if(LR == 'R'){
		while (amount > 0){
		E = 0x04; 
		SysCtlDelay(700); 
		Data = 0x14; 
		SysCtlDelay(120); 
		E = 0x00; 
		SysCtlDelay(700);
		amount--;
		}
	}
}

void shift_display(char LR){
	RS = 0; RW = 0; 
	
	if(LR == 'L'){ //shift left once
		E = 0x04; 
		SysCtlDelay(700); 
		Data = 0x18; 
		SysCtlDelay(120); 
		E = 0x00; 
		SysCtlDelay(700);		
	}
	if(LR == 'R'){ //shift right once
		
		E = 0x04; 
		SysCtlDelay(700); 
		Data = 0x1C; 
		SysCtlDelay(120); 
		E = 0x00; 
		SysCtlDelay(700);
	}
}

void rolling_display(char LR, uint8_t amount, uint8_t rate){
	while (amount > 0){
		shift_display(LR);
		wait_quarts(rate);
		amount--; 
	}
}
