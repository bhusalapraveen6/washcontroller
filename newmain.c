#include <xc.h>

// Configuration bits
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000  // 4MHz crystal

// Define washing machine states
typedef enum {
    IDLE,
    WASH,
    RINSE,
    SPIN
} WashState;

WashState currentState = IDLE;

// Function Prototypes
void initPWM(void);
void setMotorSpeed(unsigned int speed);
unsigned int readWaterLevel(void);
void handleUserInput(void);
void washingCycle(void);

void main() {
    TRISB = 0xFF;  // Set PORTB as input (User Buttons)
    TRISC = 0x00;  // Set PORTC as output (Motor Control)
    
    initPWM();
    
    while (1) {
        handleUserInput();
        washingCycle();
    }
}

// PWM Initialization for Motor Control
void initPWM(void) {
    TRISC2 = 0; // Set RC2 as output
    CCP1CON = 0x0C; // Configure CCP1 as PWM
    T2CON = 0x04;  // Enable Timer2
    PR2 = 255; // PWM Period
}

// Set motor speed using PWM
void setMotorSpeed(unsigned int speed) {
    CCPR1L = speed;
}

// Read water level using ADC
unsigned int readWaterLevel(void) {
    ADCON0 = 0x01; // Enable ADC
    __delay_ms(10);
    return (ADRESH << 8) | ADRESL; // Return 10-bit ADC result
}

// Handle user input
void handleUserInput() {
    if (RB0 == 1) {
        currentState = WASH;
    } else if (RB1 == 1) {
        currentState = RINSE;
    } else if (RB2 == 1) {
        currentState = SPIN;
    }
}

// Washing cycle state logic
void washingCycle() {
    switch (currentState) {
        case WASH:
            setMotorSpeed(200);
            __delay_ms(5000);
            currentState = RINSE;
            break;
        case RINSE:
            setMotorSpeed(100);
            __delay_ms(5000);
            currentState = SPIN;
            break;
        case SPIN:
            setMotorSpeed(255);
            __delay_ms(7000);
            currentState = IDLE;
            break;
        default:
            setMotorSpeed(0);
            break;
    }
}