/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2023.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2023 Andrew Petruska at Colorado School of Mines

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.

*/

#ifndef BIT_OPERATIONS_H  // thisis called an include guard. It prevents the compiler from parsing this header file twice and creating re-definition errors.
#define BIT_OPERATIONS_H

/**
 * Returns if a bit is set.  BIT_IS_SET( 2, 1 ) ==> true, BIT_IS_SET( 2, 0 ) ==> false
 */
#define BIT_IS_SET( byte, bit_index ) byte & ( 1 << bit_index )

/**
 * Sets a bit in a register or variable
 */
#define SET_BIT( byte, bit_index ) byte |= ( 1 << bit_index )
/**
 * Toggles a bit in a register or variable
 */
#define TGL_BIT( byte, bit_index ) byte ^= ( 1 << bit_index )
/**
 * Clears a bit in a register or variable
 */
#define CLR_BIT( byte, bit_index ) byte &= ~( 1 << bit_index )

/**
 * Sets corresponding register or variable bits that align with the mask's binary 1s
 */
#define SET_MSK( byte, mask_to_set ) byte |= mask_to_set
/**
 * Clears corresponding register or variable bits that align with the mask's binary 1s
 */
#define CLR_MSK( byte, mask_to_clear ) byte &= ~mask_to_clear
/**
 * Toggles corresponding register or variable bits that align with the mask's binary 1s
 */
#define TGL_MSK( byte, mask_to_toggle ) byte ^= mask_to_toggle

#endif