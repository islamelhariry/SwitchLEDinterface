// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
// Constant declarations to access port registers using 
// symbolic names instead of addresses
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
// 2. Declarations Section
//   Global Variables
#define PE7   (*((volatile unsigned long *)0x40024200))
#define PE6   (*((volatile unsigned long *)0x40024100))
#define PE5   (*((volatile unsigned long *)0x40024080))
#define PE4   (*((volatile unsigned long *)0x40024040))
#define PE3   (*((volatile unsigned long *)0x40024020))
#define PE2   (*((volatile unsigned long *)0x40024010))
#define PE1   (*((volatile unsigned long *)0x40024008))
#define PE0   (*((volatile unsigned long *)0x40024004))
// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PORTE_Init(void);
void Delay1ms(unsigned long msec);
void Delay100ms(unsigned long time);
void LED_On(void);
void Flash_LED(void);
// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  PORTE_Init();
	
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    LED_On();
		Delay100ms(1);
		Flash_LED();
  }
  
}

void PORTE_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x10;						 // Port E clock
  delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize     
  GPIO_PORTE_AMSEL_R &= 0x00;        // 2) disable analog function
  GPIO_PORTE_PCTL_R &= 0x00000000;   // 3) GPIO clear bit PCTL  
  GPIO_PORTE_DIR_R &= ~0x01;         // 4.1) PE0 input,
  GPIO_PORTE_DIR_R |= 0x02;          // 4.2) PE1 output  
  GPIO_PORTE_AFSEL_R &= 0x00;        // 5) no alternate function
  GPIO_PORTE_PUR_R |= 0x10;          // 6) enable pullup resistor on PF4       
  GPIO_PORTE_DEN_R |= 0x03;          // 7) enable digital pins PE1-PE0
}

void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}

void Delay1ms(unsigned long msec){
// write this function
unsigned long i;
  while(msec > 0){
    i = 13333;  // this number means 1ms
    while(i > 0){
      i = i - 1;
    }
    msec = msec - 1; // decrements every 100 ms
  }
}

void LED_On(void)
{
	while(PE0==0x00)
	{
		PE1 =0x02;
	}
}

void Flash_LED(void)
{
	while(PE0==0x01)
	{
		Delay100ms(1);
		PE1 ^=0x02;
	}
}