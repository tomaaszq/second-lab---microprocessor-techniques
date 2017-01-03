/**
 * @file				6_main.c
 *								- czestotliwosc taktowania rdzenia i ukladow peryferyjnych Fcore = 48 MHz
 *								- zastosowany 16-bitowy Counter/Timer CT16B0 z komparatorem MAT1
*/

#include <LPC11xx.h>

	#define TimePWMPeriod_us 50000
	#define TimePWMLow_us		 15000

int main (void) {

	//odblokowanie/zablokowanie zegara dla bloku IO => u nas odblokowanie(bit 16 ustawiam 1)
	//oraz bit 7 ustawiam 1=> czyli wlaczam zegar do licznika CT16B0
	LPC_SYSCON->SYSAHBCLKCTRL |= (1UL<<16)|(1UL<<7);
	
	//podlaczenie MAT1 do wyjscia PIO0_9
	LPC_IOCON->PIO0_9 = 0x00000002;

	//ustawienie takiej wartosci dzielnika wstepnego, zeby niezaleznie od PCLK timer counter byl rowny 1MHz
	LPC_TMR16B0->PR	= (SystemCoreClock / 1000000) - 1;
	
	//przypisanie wartosci do MATCH REGISTER1(rejestr MR1) oraz MATCH REGISTER3(rejestr MR3)
	LPC_TMR16B0->MR1 = (TimePWMLow_us) - 1;		
	LPC_TMR16B0->MR3 = (TimePWMPeriod_us) - 1;	
	
	//jesli TIMER COUNTER=MR3 ,to reset licznika TIMER COUNTER, wyjscie LOW
	LPC_TMR16B0->MCR = 1UL<< 0x00000400;
	//jesli TIMER COUNTER=MR1 ,to wyjscie MATn1 na stan HIGH
	LPC_TMR16B0->PWMC = 1UL<<1;
	
	//wlaczenie licznika TIMER COUNTER
	LPC_TMR16B0->TCR = 1UL;
	
	while(1);
}
