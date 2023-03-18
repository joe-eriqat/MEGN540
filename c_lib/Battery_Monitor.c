#include "Battery_Monitor.h"

// #define __AVR_ATmega32U4__

static const float BITS_TO_BATTERY_VOLTS = ( 2.0 * 5.0 ) / 1024.0;
static Filter_Data_t _filter;
static bool _init_first_time = true;

/**
 * Function Initialize_Battery_Monitor initializes the Battery Monitor to record the current battery voltages.
 */
void Initialize_Battery_Monitor()
{
    // *** MEGN540 LAB3 YOUR CODE HERE ***
    // CLR_BIT( DDRF, DDF6 );    // set data direction to input for Port F, bit 6
    SET_BIT( DIDR0, ADC6D );  // disable digital input for this port
    SET_BIT( ADCSRA, ADPS2 );
    SET_BIT( ADCSRA, ADPS1 );
    SET_BIT( ADCSRA, ADPS0 );  // enable adc, set prescaler to 128 (24.9.2)

    // set voltage reference to AVcc with external capacitor on AREF pin
    CLR_BIT( ADMUX, REFS1 );
    SET_BIT( ADMUX, REFS0 );

    // we don't set ADIE in ADCSRA and I-bit in SREG because we instead spin wait to check the ADIF bit...
    SET_BIT( ADCSRA, ADEN );

    // initailize butterworth filter
    uint8_t order      = 4;
    float num_coeffs[] = { 0.0000132937288987530, 0.0000531749155950119, 0.0000797623733925178, 0.0000531749155950119, 0.0000132937288987530 };
    float den_coeffs[] = { 1.000000000000000, -3.671729089161934, 5.067998386734185, -3.115966925201742, 0.719910327291870 };
    Filter_Init( &_filter, num_coeffs, den_coeffs, order );
    _init_first_time = true;
}

/**
 * Function Battery_Voltage initiates the A/D measurement and returns the result for the battery voltage.
 */
float Battery_Voltage()
{
    // A Union to assist with reading the LSB and MSB in the  16 bit register
    union {
        struct {
            uint8_t LSB;
            uint8_t MSB;
        } split;
        uint16_t value;
    } data = { .value = 0 };

    // configure mux to use ADC 6 input
    // set mux[5..0] to 000110
    CLR_BIT( ADMUX, MUX0 );
    SET_BIT( ADMUX, MUX1 );
    SET_BIT( ADMUX, MUX2 );
    CLR_BIT( ADMUX, MUX3 );
    CLR_BIT( ADMUX, MUX4 );
    CLR_BIT( ADCSRB, MUX5 );

    // *** MEGN540 LAB3 YOUR CODE HERE ***
    SET_BIT( ADCSRA, ADSC );  // initiate ADC read

    // loop until ADC is done
    while( BIT_IS_SET( ADCSRA, ADSC ) ) {};

    data.split.LSB = ADCL;
    data.split.MSB = ADCH & 0x03;  // only lowest two bits matter

    float voltage = data.value * BITS_TO_BATTERY_VOLTS;
    // return voltage;
    if( _init_first_time ) {
        Filter_SetTo( &_filter, voltage );
        _init_first_time = false;
    }
    return Filter_Value( &_filter, voltage );
}