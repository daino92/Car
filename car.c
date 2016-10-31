#include <xc.h>
#include <plib.h>	//peripheral libraries
#include <stdio.h>
#include <stdlib.h>
#include "config_bits.h"

#define _XTAL_FREQ 12000000
#define USE_AND_MASKS

//input bridge
#define A1_TRIS TRISA0
#define A2_TRIS TRISA1
#define A3_TRIS TRISA2
#define A4_TRIS TRISA3

#define A1 LATA0
#define A2 LATA1
#define A3 LATA2
#define A4 LATA3

//PWM 9 states of speed
#define DC_0 0
#define DC_68 700
#define DC_75 767	//1st mob speed
#define DC_78 800
#define DC_83 850	//2nd mob speed
#define DC_88 900
#define DC_93 950	//3rd mob speed
#define DC_97 1000
#define DC_100 1023	//4rth mob speed

void usart_init ( void );
void pwm_init ( void );

int main(void) {
    unsigned int duty1 = DC_83, duty2 = DC_83;

    usart_init();
    pwm_init();

    A1_TRIS = A2_TRIS = A3_TRIS = A4_TRIS = 0;

    for(;;) {
        char data;

        if (DataRdyUSART()) {
            data = ReadUSART();
            if (data == 'F' || data == 'f') {
                putrsUSART("Forward");
                A1 = A3 = 1;
                A2 = A4 = 0;
                SetDCPWM1(duty1);
                SetDCPWM2(duty2);
            }
            else if (data == 'B' || data == 'b') {
                putrsUSART("Backwards");
                A1 = A3 = 0;
                A2 = A4 = 1;
                SetDCPWM1(duty1);
                SetDCPWM2(duty2);
            }
            else if (data == 'L' || data == 'l') {
                putrsUSART("Left Turn");
                A1 = A4 = 1;
                A2 = A3 = 0;
                SetDCPWM1(duty1);
                SetDCPWM2(duty2);
            }
            else if (data == 'R' || data == 'r') {
                putrsUSART("Right Turn");
                A1 = A4 = 0;
                A2 = A3 = 1;
                SetDCPWM1(duty1);
                SetDCPWM2(duty2);
            }
            else if (data == 'S' || data == 's') {
                putrsUSART("Stop");
                SetDCPWM1(DC_0);
                SetDCPWM2(DC_0);
            }
            else if (data == '1') {
                putrsUSART("Duty Cycle 68%");
                duty1 = DC_68;
                duty2 = DC_68;
            }
            else if (data == '2') {
                putrsUSART("Duty Cycle 75%");
                duty1 = DC_75;
                duty2 = DC_75;
            }
            else if (data == '3') {
                putrsUSART("Duty Cycle 78%");
                duty1 = DC_78;
                duty2 = DC_78;
            }
            else if (data == '4') {
                putrsUSART("Duty Cycle 83%");
                duty1 = DC_83;
                duty2 = DC_83;
            }
            else if (data == '5') {
                putrsUSART("Duty Cycle 88%");
                duty1 = DC_88;
                duty2 = DC_88;
            }
            else if (data == '6') {
                putrsUSART("Duty Cycle 93%");
                duty1 = DC_93;
                duty2 = DC_93;
            }
            else if (data == '7') {
                putrsUSART("Duty Cycle 97%");
                duty1 = DC_97;
                duty2 = DC_97;
            }
            else if (data == '8') {
                putrsUSART("Duty Cycle 100%");
                duty1 = DC_100;
                duty2 = DC_100;
            }
        }
    }
    return (EXIT_SUCCESS);
}

void pwm_init () {
    ClosePWM1();	//In case of PIC's reset
    ClosePWM2(); 
    CloseTimer2(); 

    TRISC2 = 0;	//CCP1
    TRISC1 = 0;	//CCP2

    //Timer2 Configuration flags
    OpenTimer2(TIMER_INT_ON & T2_PS_1_1 & T2_POST_1_1);

    OpenPWM1(255);
    OpenPWM2(255);

    SetDCPWM1(DC_0);
    SetDCPWM2(DC_0);
}

void usart_init () {
    unsigned char usart_config;
    unsigned int spbrg;

    CloseUSART();

    TRISC6 = 0; // Tx
    TRISC7 = 1; // Rx

    //USART Configuration Flags
    usart_config = USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE
       & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH & USART_ADDEN_OFF;

    //Bitrate = Fosc/(16*(spbrg+1))
    spbrg = 77;//About 9615 @12MHz (9600bps)

    OpenUSART(usart_config, spbrg);
}