/*
 * main.c
 *
 * Created: 8/12/2021 11:44:39 AM
 *  Author: AKASH
 */ 

#include <util/delay.h>					// Standard AVR Delay Library
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/io.h>				// Standard AVR IO Library					
#define		F_CPU							14745600

void motors_pin_config(void) {

	DDRL = 0xFF ; 
	PORTL = 0x00;					
}

void pwm_pin_config(void){

	DDRH |= (1<<3); 		
	PORTH |= (1<<3);     	
}
 void switch_config(void) {
	 DDRD = 0x00;
	 PORTD =0x00;
	 DDRE =0x00;
	 PORTE =0x00;
	 DDRF = 0X00;
	 PORTF = 0X00 ;
 }
  void interruptsw_conf(void) {
	  EIMSK = 0x1F ;
	  EICRA =0x00;
	  EICRB =0x01;
  }
void motors_move_forward(void) {
	PORTL =0x55 ; 
}
void motors_move_backward(void) {
	PORTL =0xAA ;
}
void motors_move_left(void) {
	PORTL =0x66 ;
}
void motors_move_right(void){
	PORTL =0x99 ;
}
void motors_stop(void){
	PORTL =0x00 ;
}

ISR(INT0_vect){
	motors_move_left() ;
	_delay_ms(2000);
	motors_stop() ;
	}
ISR(INT1_vect){
	motors_move_right() ;
	_delay_ms(2000);
	motors_stop() ;
}
ISR(INT2_vect){
	motors_move_forward() ;
}
ISR(INT3_vect){
	motors_move_backward() ;
}
ISR(INT4_vect){
	motors_stop() ;
}
void adc_init(void){
	DDRF = 0x00 ;
	PORTF =0x00 ;
	ADCSRA =(0x86) ;
	ADCSRB =0X00 ;
	ADMUX =0x40 ;
	ACSR =0X80 ;
 }
int adc_start(void){
	
	ADCSRA |= (1<<ADSC) ; 
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA |=(1<<ADIF);
	return ADCW ;
	_delay_ms(1000);
}
void timer_pwm_init(void) {
   DDRH |=(1<<3) ;
   PORTH |=(1<<3);
   TCNT4 = 0x00 ;
   TCCR4A =0x81;
   TCCR4B =0x03 ;
   OCR4AL = 0x00 ;
   }
	

int main(void)
{
	pwm_pin_config() ;
	motors_pin_config() ;
	switch_config() ;
	interruptsw_conf() ;
	adc_init() ;
	timer_pwm_init() ;	
	sei();
	
    while(1)
    {
       
	   OCR4A = (adc_start()/4); 
	   if(ADCW==0)
	    motors_stop();
	   _delay_ms(2000);
    }
}