
#ifndef __LCD1602A_H__
#define __LCD1602A_H__
#define RS  (*((volatile unsigned long*) 0x40024004))
#define RW  (*((volatile unsigned long*) 0x40024008))
#define E   (*((volatile unsigned long*) 0x40024010))
#define Data   (*((volatile unsigned long*) 0x400053FC))
#define BF  (*((volatile unsigned long*) 0x40005200))
#endif
//RS. 1: data register, 0: Intruction Register(for write), busy flag-address counter(for read)
//RW. 1: Read mode, 0: write mode
//E.  1: enable for writing and reading data, 0: disables writing and reading
//DB0-DB7, 8-bit data bus
	//PE0: RS
	//PE1: RW
	//PE2: E
	
	//PB0: D0
	//PB1: D1
	//PB2: D2
	//PB3: D3
	//PB4: D4
	//PB5: D5
	//PB6: D6
	//PB7: D7
void portinit_lcd(void); 
void write_data(uint8_t data);
void initialize_lcd(uint8_t lines, uint8_t font);
void clear(void);
void displayoff(void);
void displayon(void);
void cursoron(void);
void cursoroff(void);
void rethome(void);
void write_string(char *); 
void wait_1s(uint8_t);
void wait_halfs(uint8_t times);
void wait_quarts(uint8_t times); 
void lcdinit(uint8_t lines, uint8_t font); 
void set_address(uint8_t address); 
void next_line(void); 
void write_int(uint32_t); 
void shift_cursor(char LR, uint8_t amount);
void shift_display(char LR); 
void rolling_display(char LR, uint8_t amount, uint8_t rate);
