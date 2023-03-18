#include "MotorPWM.h"

#include "Bit_Operations.h"

void Initialize_MotorPWM( uint16_t MAX_PWM )
{

    // CLR_BIT( PRR0, PRTIM1 );

    // // DDRD |= ( 1 << PORTD3 ) | ( 1 << PORTD5 ) | ( 1 << PORTD6 );
    // DDRB |= ( 1 << PORTB2 );  // ddb1, 2, 5, 6
    SET_BIT( DDRB, DDB1 );
    SET_BIT( DDRB, DDB2 );
    SET_BIT( DDRB, DDB5 );
    SET_BIT( DDRB, DDB6 );

    CLR_BIT( TCCR1B, CS12 );
    CLR_BIT( TCCR1B, CS11 );
    SET_BIT( TCCR1B, CS10 );
    // clr clr set cs 12 11 10

    CLR_BIT( TCCR1A, WGM10 );
    CLR_BIT( TCCR1A, WGM11 );
    CLR_BIT( TCCR1B, WGM12 );
    SET_BIT( TCCR1B, WGM13 );

    MotorPWM_Set_Max( MAX_PWM );

    MotorPWM_Enable( false );

    MotorPWM_Set_Left( 0 );
    MotorPWM_Set_Right( 0 );

    return;
}

void MotorPWM_Enable( bool enable )
{
    if( enable ) {
        SET_BIT( TCCR1A, COM1B1 );
        SET_BIT( TCCR1A, COM1A1 );
        CLR_BIT( TCCR1A, COM1B0 );
        CLR_BIT( TCCR1A, COM1A0 );
    } else {
        CLR_BIT( TCCR1A, COM1B1 );
        CLR_BIT( TCCR1A, COM1A1 );
        CLR_BIT( TCCR1A, COM1B0 );
        CLR_BIT( TCCR1A, COM1A0 );
    }
}

bool MotorPWM_Is_Enabled()
{
    return BIT_IS_SET( TCCR1A, COM1B1 ) || BIT_IS_SET( TCCR1A, COM1A1 );
}

void MotorPWM_Set_Left( int16_t pwm )
{
    if( pwm < 0 ) {
        SET_BIT( PORTB, PORTB2 );
    } else {
        CLR_BIT( PORTB, PORTB2 );
    }
    OCR1B = abs( pwm );
}

void MotorPWM_Set_Right( int16_t pwm )
{
    if( pwm < 0 ) {
        SET_BIT( PORTB, PORTB1 );
    } else {
        CLR_BIT( PORTB, PORTB1 );
    }
    OCR1A = abs( pwm );
}

int16_t MotorPWM_Get_Left()
{
    return OCR1B;
}

int16_t MotorPWM_Get_Right()
{
    return OCR1A;
}

uint16_t MotorPWM_Get_Max()
{
    return ICR1;
}

void MotorPWM_Set_Max( uint16_t MAX_PWM )
{
    ICR1 = MAX_PWM;
}