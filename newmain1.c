// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
//#include "usart_pic16.h"
#include <pic.h>
#include <stdint.h>
#define _XTAL_FREQ 4000000 //Specify the XTAL crystal FREQ

#define delay 500

unsigned char seconds=0;
unsigned char minutes=18;
unsigned char hours=16;
uint8_t PORTDTEMP=0;
uint8_t C =0;   //Declare some overflow bit parts.
/*This program now seems to work okay. So it sets a pin on and off on certain times.
 * Can be used to control a pump for daily irrigation. The timing is approximately one second.
 * If everything is working fine, this could be also implemented on a 10f200, since a 16f877 is an overkill.
 * TODO:
 * -Check for powerline. If there's no power, there's no need to switch on the pump.
 * -If possible, include a button, which bypasses the timer, and just turns on the pump. 
 *  If it's pushed again, the timer should control things again.
 * 
 * Also, for this 16F version:
 * -Screen  to show information about the irrigation.(Next time, current time, status.)
 * -Buttons to change the irrigation time.
 * -Ability to change the irrigation length.
 * -Serial port connection.
 * -Add a light sensor and a temperature sensor.
 * -Store data on EEPROM to make irrigation adjustments.
 * -Maybe low water warning.
 */



void portDled0();
void portDled1();
void portDled2();
void portDled3();
void lampon();
void lampoff();
void portDled7();
//void interrupt ISR();

void main()
{
    TMR1=0; //Clear Timer.
    TMR1CS=0;   //Clock source.
    T1CKPS0=0;
    T1CKPS1=0;  //The two prescalers.
    TMR1ON=1;   //Switch on the Timer.
    TMR1IE=1;
    TMR1IF=0;
    PEIE=1;
    GIE=1;
    //-------------Setting up timer and interrupts.
  TRISD=0;
  TRISA=0;
  TRISB=0;
  TRISC=0;
  TRISE=0b000;
  PORTA=0;
  PORTB=0;
  PORTC=0;
  PORTE=0;
  //T1CON=0x01;   //Prescale value = 1:1, It using Internal clock, Timer 1 ON
  //Setup all ports as outputs and put them in a known state.
  while(1) {
/*The timer for seconds and showing the system working is running as an interrupt.
 */
            if(seconds>59){   //More than a minute.
          seconds=0;    //Reset seconds.
          minutes++;    //Increase minutes.
          PORTDTEMP^=0b00000100;
          PORTD=PORTDTEMP;}
            if(minutes>59){
                minutes=0;
                hours++;
                PORTDTEMP^=0b00001000;
                PORTD=PORTDTEMP;}
                
  if(hours>23){
      hours=0;
                      PORTDTEMP^=0b00010000;
                PORTD=PORTDTEMP;

  }

      
      if(hours==7&minutes==0&seconds==1){
          lampon();
      }
      if(hours==7&minutes==59&seconds==59){
          lampoff();
      }
      if(hours==19&minutes==0&seconds==1){
          lampon();
      }
      if(hours==19&minutes==59&seconds==59){
          lampoff();
      }
      
  }
}
void portDled7(){
    PORTDTEMP^=0b00000001;
    PORTD=PORTDTEMP;   
}
void portDled1(){
    PORTDTEMP^=0b01000010;
    PORTD=PORTDTEMP;
}
void lampon(){
    PORTDTEMP|=0b10000001;  //Same as the 10f200 part.
    PORTD=PORTDTEMP;
}
void lampoff(){
    PORTDTEMP&=0b01111110;  //Same as the 10f200 part.
    PORTD=PORTDTEMP;    
}
void __interrupt() ISR(void){
if (TMR1IF){
    C++;
    if(C==15)
    {
    //TOGGLE led and account for seconds.
        seconds++;
       portDled1();
       C=0;
//----------main timer
       
       
       
    }
    TMR1IF =0;}

}
