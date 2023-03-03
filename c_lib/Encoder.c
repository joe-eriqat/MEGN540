#include "Encoder.h"

#include "Bit_Operations.h"

#define COUNTS_TO_RAD 0.006906876230823

/**
 * Internal counters for the Interrupts to increment or decrement as necessary.
 */
static volatile bool _last_right_A = 0;  // Static limits it's use to this file
static volatile bool _last_right_B = 0;  // Static limits it's use to this file

static volatile bool _last_left_A   = 0;  // Static limits it's use to this file
static volatile bool _last_left_B   = 0;  // Static limits it's use to this file
static volatile bool _last_left_XOR = 0;  // Necessary to check if PB4 triggered the ISR or not

static volatile int32_t _left_counts  = 0;  // Static limits it's use to this file
static volatile int32_t _right_counts = 0;  // Static limits it's use to this file

/** Helper Funcions for Accessing Bit Information */
// *** MEGN540 Lab 3 TODO ***
// Hint, use avr's bit_is_set function to help
static inline bool Right_XOR()
{
    // return BIT_IS_SET( PORTE, PORTE6 );
    return BIT_IS_SET( PINE, PINE6 );
}  // MEGN540 Lab 3
static inline bool Right_B()
{
    // return BIT_IS_SET( PORTF, PORTF0 );
    return BIT_IS_SET( PINF, PINF0 );
}  // MEGN540 Lab 3
static inline bool Right_A()
{
    return Right_XOR() ^ Right_B();
}  // MEGN540 Lab 3

static inline bool Left_XOR()
{
    // return BIT_IS_SET( PORTB, PORTB4 );
    return BIT_IS_SET( PINB, PINB4 );
}  // MEGN540 Lab 3
static inline bool Left_B()
{
    // return BIT_IS_SET( PORTE, PORTE2 );
    return BIT_IS_SET( PINE, PINE2 );
}  // MEGN540 Lab 3
static inline bool Left_A()
{
    return Left_XOR() ^ Left_B();
}  // MEGN540 Lab 3

/**
 * Function Encoders_Init initializes the encoders, sets up the pin change interrupts, and zeros the initial encoder
 * counts.
 */
void Initialize_Encoders()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE

    // Left encoder uses PB4 and PE2 pins as digital inputs. External interrupt PCINT4 is necessary to detect
    // the change in XOR flag. You'll need to see Section 11.1.5 - 11.1.7 for setup and use.
    // Note that the PCINT interrupt is trigered by any PCINT pin. In the ISR you should check to make sure
    // the interrupt triggered is the one you intend on processing.

    // Right encoder uses PE6 adn PF0 as digital inputs. External interrupt INT6 is necessary to detect
    // the changes in XOR flag. You'll need to see Sections 11.1.2-11.1.4 for setup and use.
    // You'll use the INT6_vect ISR flag.

    // Initialize static file variables. These probably need to be updated.
    _last_right_A = 0;  // MEGN540 Lab 3 TODO
    _last_right_B = 0;  // MEGN540 Lab 3 TODO

    _last_left_A   = 0;  // MEGN540 Lab 3 TODO
    _last_left_B   = 0;  // MEGN540 Lab 3 TODO
    _last_left_XOR = 0;  // MEGN540 Lab 3 TODO

    _left_counts  = 0;  // MEGN540 Lab 3 TODO
    _right_counts = 0;  // MEGN540 Lab 3 TODO

    // right side encoder
    CLR_BIT( DDRF, DDF0 );
    // SET_BIT( PORTF, PORTF0 );
    SET_BIT( PINF, PINF0 );
    CLR_BIT( DDRE, DDE6 );
    // SET_BIT( PORTE, PORTE6 );
    SET_BIT( PINE, PINE6 );

    // left encoder
    CLR_BIT( DDRE, DDE2 );
    // SET_BIT( PORTE, PORTE2 );
    SET_BIT( PINE, PINE2 );
    CLR_BIT( DDRB, DDB4 );
    // SET_BIT( PORTB, PORTB4 );
    SET_BIT( PINB, PINB4 );

    SET_BIT( PCICR, PCIE0 );
    SET_BIT( PCIFR, PCIF0 );
    SET_BIT( PCMSK0, PCINT4 );

    CLR_BIT( EIMSK, INT6 );
    SET_BIT( EICRB, ISC60 );
    SET_BIT( EIMSK, INT6 );
}

/**
 * Function Encoder_Counts_Left returns the number of counts from the left encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    cli();
    int32_t count = _left_counts;
    //_left_counts  = 0;
    sei();
    return count;
}

/**
 * Function Encoder_Counts_Right returns the number of counts from the right encoder.
 * @return [int32_t] The count number.
 */
int32_t Encoder_Counts_Right()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE
    // Note: Interrupts can trigger during a function call and an int32 requires
    // multiple clock cycles to read/save. You may want to stop interrupts, copy the value,
    // and re-enable interrupts to prevent this from corrupting your read/write.
    cli();
    int32_t count = _right_counts;
    //_right_counts = 0;
    sei();
    return count;
}

/**
 * Function Encoder_Rad_Left returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Left()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE.  How many counts per rotation???
    return Encoder_Counts_Left() * COUNTS_TO_RAD;
}

/**
 * Function Encoder_Rad_Right returns the number of radians for the left encoder.
 * @return [float] Encoder angle in radians
 */
float Encoder_Rad_Right()
{
    // *** MEGN540 Lab3 ***
    // YOUR CODE HERE.  How many counts per rotation???
    return Encoder_Counts_Right() * COUNTS_TO_RAD;
}

/**
 * Interrupt Service Routine for the left Encoder. Note: May need to check that it is actually PCINT4 that triggered, as
 * the Pin Change Interrupts can trigger for multiple pins.
 * @return
 */
ISR( PCINT0_vect )
{
    if( _last_left_XOR != Left_XOR() ) {
        _left_counts += ( Left_A() ^ _last_left_B ) - ( _last_left_A ^ Left_B() );
        _last_left_A   = Left_A();
        _last_left_B   = Left_B();
        _last_left_XOR = Left_XOR();
    }
}

/**
 * Interrupt Service Routine for the right Encoder.
 * @return
 */
ISR( INT6_vect )
{
    _right_counts += ( Right_A() ^ _last_right_B ) - ( _last_right_A ^ Right_B() );
    _last_right_A = Right_A();
    _last_right_B = Right_B();
}