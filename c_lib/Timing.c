/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

*/

#include "../c_lib/Timing.h"

/** These define the internal counters that will be updated in the ISR to keep track of the time
 *  The volatile keyword is because they are changing in an ISR, the static means they are not
 *  visible (not global) outside of this file.
 */
static volatile uint32_t _count_ms = 0;

/**
 * Function Initialize_Timing initializes Timer0 to have a prescalar of XX and initializes the compare
 * feature for use in an ISR.  It also enables ISR's.
 *
 *  FCPU = 16 000 000 Hz,  1 us => 1 000 000 Hz,  A prescalar of 16 would give you millisecond resolution, but you dont
 *  have enough time to do those additions every microsecond, in addition the 8-bit timer will roll over every 256 microseconds.
 *  So, you need to be counting in (effectively) 4us increments.  So use a prescalar of 64. This will give you a resolution of 4us per
 *  Timer0 Bit and enough head-room to compare at with 249 for a 1kHz update.
 *
 *  Since Timer 0 will be triggering at a kHz, we may want other things to be updated at 1kHz too.
 *
 */
void Initialize_Timing()
{
    // *** MEGN540 Lab 2 ***
    // YOUR CODE HERE
    // Enable timing, setup prescalers, etc.

    _count_ms = 0;
    // NOTE
    // Setup OC0X before setting DDR pin to output

    TCNT0  = 0x00;                           // Set time to zero
    TCCR0B = ( 1 << CS00 ) | ( 1 << CS01 );  // set prescaler to 64

    TCCR0A = ( 1 << WGM01 );  // Toggle on compare match, controls OC0A behavior, on then off then on (Table 13-1) How to toggle?
    // WGM01 sets CTC mode for clearing timer

    OCR0A = 249;  // 8 bit value compared to TCNT0, used to generate output compare interrupt. set as 0xF9 = 250 to imply one millisecond?

    TIMSK0 = ( 1 << OCIE0A );  // push a 1 to this location to enable the OCR interrupt

    // Clear timer on compare match? WGM1 = 1, combined with WGM02 in TCCR0B register. CTC = clear timer on compare match. MODE 2
    // WGM02 in TCCR0B is "Waveform generation mode"

    // OCF0A = Flag for interrupt, inside TIFR0 register. Is this used here? How to toggle back and forth and count the changes
    // if( OCF0A ) {
    //     _count_ms++;  // if the output flag is triggered, count one ms
    // }
    // Do we need to do anything with DDR port for flag/output compare mode?
}

/**
 * This function gets the current time and returns it in a Time_t structure.
 * @return
 */
float Timing_Get_Time_Sec()
{
    // *** MEGN540 Lab 2 ***
    // YOUR CODE HERE
    return ( _count_ms / 1000.0f );
}
Time_t Timing_Get_Time()
{
    // *** MEGN540 Lab 2 ***
    // YOUR CODE HERE
    Time_t time = {
        .millisec = _count_ms,
        .microsec = ( TCNT0 * 4 )  // YOU NEED TO REPLACE THIS WITH A CALL TO THE TIMER0 REGISTER AND MULTIPLY APPROPRIATELY
    };

    return time;
}

/**
 * These functions return the individual parts of the Time_t struct, useful if you only care about
 * things on second or millisecond resolution.
 * @return
 */
uint32_t Timing_Get_Milli()
{
    return _count_ms;
}
uint16_t Timing_Get_Micro()
{
    // *** MEGN540 Lab 2 ***
    // YOUR CODE HERE
    return ( TCNT0 * 4 );  // YOU NEED TO REPLACE THIS WITH A CALL TO THE TIMER0 REGISTER AND MULTIPLY APPROPRIATELY
}

/**
 * This function takes a start time and calculates the time since that time, it returns it in the Time struct.
 * @param p_time_start a pointer to a start time struct
 * @return (Time_t) Time since the other time.
 */
float Timing_Seconds_Since( const Time_t* time_start_p )
{
    // *** MEGN540 Lab 2 ***

    float current_time = Timing_Get_Time_Sec();

    float delta_time = current_time - ( time_start_p->millisec / 1000.0f );
    return delta_time;
}

/** This is the Interrupt Service Routine for the Timer0 Compare A feature.
 * You'll need to set the compare flags properly for it to work.
 */
ISR( TIMER0_COMPA_vect )
{
    // *** MEGN540 Lab 2 ***
    // YOUR CODE HERE
    // YOU NEED TO RESET THE Timer0 Value to 0 again!

    // take care of upticks of both our internal and external variables.
    TCNT0 = 0x00;
    _count_ms++;
}
