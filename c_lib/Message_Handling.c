/*
         MEGN540 Mechatronics Lab
    Copyright (C) Andrew Petruska, 2021.
       apetruska [at] mines [dot] edu
          www.mechanical.mines.edu
*/

/*
    Copyright (c) 2021 Andrew Petruska at Colorado School of Mines

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

#include "Message_Handling.h"

/**
 * Function _Message_Length returns the number of bytes associated with a command string per the
 * class documentation; This is only accessable from this file.
 * @param cmd
 * @return Size of expected string. Returns 0 if unrecognized.
 */
static uint8_t _Message_Length( char cmd );

/**
 * Function Task_Message_Handling processes USB messages as necessary and sets
 * status flags to control the flow of the program.
 */
void Task_Message_Handling( float _time_since_last )
{
    // *** MEGN540  ***
    // YOUR CODE HERE. I suggest you use your peak function and a switch interface
    // Either do the simple stuff strait up, set flags to have it done later.
    // If it just is a USB thing, do it here, if it requires other hardware, do it
    // in the main and set a flag to have it done here.

    // Check to see if their is data in waiting
    if( !USB_Msg_Length() )
        return;  // nothing to process...

    // Get Your command designator without removal so if their are not enough
    // bytes yet, the command persists
    char command = USB_Msg_Peek();

    /* MEGN540 -- LAB 2 */ bool command_processed = false;

    // process command
    switch( command ) {
        case '*':
            if( USB_Msg_Length() >= _Message_Length( '*' ) ) {
                // then process your multiplication...

                // remove the command from the usb recieved buffer using the
                // usb_msg_get() function
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Multiply_And_Send( data.v1, data.v2 );

                /* MEGN540 -- LAB 2 */ command_processed = true;
            }
            break;
        case '/':
            if( USB_Msg_Length() >= _Message_Length( '/' ) ) {
                // then process your divide...
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Divide_And_Send( data.v1, data.v2 );
                /* MEGN540 -- LAB 2 */ command_processed = true;
            }
            break;
        case '+':
            if( USB_Msg_Length() >= _Message_Length( '+' ) ) {
                // then process your plus...
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Add_And_Send( data.v1, data.v2 );
                /* MEGN540 -- LAB 2 */ command_processed = true;
            }
            break;
        case '-':
            if( USB_Msg_Length() >= _Message_Length( '-' ) ) {
                // then process your minus...
                USB_Msg_Get();  // removes the first character from the received buffer,
                                // we already know it was a * so no need to save it as a
                                // variable

                // Build a meaningful structure to put your data in. Here we want two
                // floats.
                struct __attribute__( ( __packed__ ) ) {
                    float v1;
                    float v2;
                } data;

                // Copy the bytes from the usb receive buffer into our structure so we
                // can use the information
                USB_Msg_Read_Into( &data, sizeof( data ) );

                // Call MEGN540_Lab_Task Function
                Subtract_And_Send( data.v1, data.v2 );
                /* MEGN540 -- LAB 2 */ command_processed = true;
            }
            break;
        case '~':
            if( USB_Msg_Length() >= _Message_Length( '~' ) ) {
                // then process your reset by setting the task_restart flag defined in Lab1_Tasks.h
                Task_Activate( &task_restart, -1 );

                /* MEGN540 -- LAB 2 */ command_processed = true;
            }
            break;
        case 't':
            if( USB_Msg_Length() >= _Message_Length( 't' ) ) {
                USB_Msg_Get();
                uint8_t time_type = USB_Msg_Get();

                switch( time_type ) {
                    case 0x00:  // time now
                        Task_Activate( &task_send_time, -1 );
                        break;
                    case 0x01:  // time to complete a full loop iteration
                        Task_Activate( &task_time_loop, -1 );
                        break;
                    default: break;
                }

                /* MEGN540 -- LAB 2 */ command_processed = true;
            }
            break;
        case 'T':
            if( USB_Msg_Length() >= _Message_Length( 'T' ) ) {
                USB_Msg_Get();
                uint8_t time_type = USB_Msg_Get();
                float duration;

                USB_Msg_Read_Into( &duration, sizeof( duration ) );

                switch( time_type ) {
                    case 0x00:  // time now
                        if( duration > 0 ) {
                            Task_Activate( &task_send_time, duration );
                        } else {
                            Task_Cancel( &task_send_time );
                        }
                        break;
                    case 0x01:  // time to complete a full loop iteration
                        if( duration > 0 ) {
                            Task_Activate( &task_time_loop, duration );
                        } else {
                            Task_Cancel( &task_time_loop );
                        }
                        break;
                    default: break;
                }

                /* MEGN540 -- LAB 2 */ command_processed = true;
            }
            break;
        case 'e':
            if( USB_Msg_Length() >= _Message_Length( 'e' ) ) {
                USB_Msg_Get();  // remove command
                Task_Activate( &task_send_encoder_counts, -1 );
                command_processed = true;
            }
            break;
        case 'E':
            if( USB_Msg_Length() >= _Message_Length( 'E' ) ) {
                USB_Msg_Get();  // remove command
                float repeat_time;
                USB_Msg_Read_Into( &repeat_time, sizeof( repeat_time ) );

                if( repeat_time > 0 ) {
                    Task_Activate( &task_send_encoder_counts, repeat_time );
                } else {
                    Task_Cancel( &task_send_encoder_counts );
                }
                command_processed = true;
            }
            break;
        case 'b':
            if( USB_Msg_Length() >= _Message_Length( 'b' ) ) {
                USB_Msg_Get();  // remove command
                Task_Activate( &task_send_battery_voltage, -1 );
                command_processed = true;
            }
            break;
        case 'B':
            if( USB_Msg_Length() >= _Message_Length( 'B' ) ) {
                USB_Msg_Get();  // remove command
                float repeat_time;
                USB_Msg_Read_Into( &repeat_time, sizeof( repeat_time ) );

                if( repeat_time > 0 ) {
                    Task_Activate( &task_send_battery_voltage, repeat_time );
                } else {
                    Task_Cancel( &task_send_battery_voltage );
                }
                command_processed = true;
            }
            break;
        case 'p':
            if( USB_Msg_Length() >= _Message_Length( 'p' ) ) {
                USB_Msg_Get();
                if( Battery_Voltage() > 4.0 ) {
                    struct __attribute__( ( __packed__ ) ) {
                        uint16_t Left_PWM;
                        uint16_t Right_PWM;
                    } msg;
                    // uint16_t Left_PWM;
                    // uint16_t Right_PWM;
                    USB_Msg_Read_Into( &msg.Left_PWM, sizeof( msg.Left_PWM ) );
                    USB_Msg_Read_Into( &msg.Right_PWM, sizeof( msg.Right_PWM ) );
                    Set_Motor_PWM( msg.Left_PWM, msg.Right_PWM );
                    MotorPWM_Enable( true );

                    USB_Send_Msg( "chh", 'p', &msg, sizeof( msg ) );
                } else {
                    USB_Flush_Input_Buffer();
                    Send_Battery_Low_Warning( 0.0 );
                }
                command_processed = true;
            }
            break;
        case 'P':
            if( USB_Msg_Length() >= _Message_Length( 'P' ) ) {
                USB_Msg_Get();
                if( Battery_Voltage() > 4.0 ) {
                    struct __attribute__( ( __packed__ ) ) {
                        uint16_t Left_PWM;
                        uint16_t Right_PWM;
                        float duration;
                    } P_msg;
                    // uint16_t Left_PWM;
                    // uint16_t Right_PWM;
                    USB_Msg_Read_Into( &P_msg.Left_PWM, sizeof( P_msg.Left_PWM ) );
                    USB_Msg_Read_Into( &P_msg.Right_PWM, sizeof( P_msg.Right_PWM ) );
                    USB_Msg_Read_Into( &P_msg.duration, sizeof( P_msg.duration ) );

                    Set_Motor_PWM( P_msg.Left_PWM, P_msg.Right_PWM );
                    MotorPWM_Enable( true );
                    task_timed_motor_run.time_last_ran = Timing_Get_Time();
                    Task_Activate( &task_timed_motor_run, P_msg.duration );

                    USB_Send_Msg( "chhf", 'P', &P_msg, sizeof( P_msg ) );
                } else {
                    USB_Flush_Input_Buffer();
                    Send_Battery_Low_Warning( 0.0 );
                }
                command_processed = true;
            }
            break;
        case 's':
            if( USB_Msg_Length() >= _Message_Length( 's' ) ) {
                USB_Flush_Input_Buffer();
                Set_Motor_PWM( 0, 0 );
                MotorPWM_Enable( false );
                command_processed = true;
            }
            break;
        case 'S':
            if( USB_Msg_Length() >= _Message_Length( 'S' ) ) {
                USB_Flush_Input_Buffer();
                Set_Motor_PWM( 0, 0 );
                MotorPWM_Enable( false );
                command_processed = true;
            }
            break;
        case 'q':
            if( USB_Msg_Length() >= _Message_Length( 'q' ) ) {
                USB_Msg_Get();

                Task_Activate( &task_send_system_id, -1 );

                command_processed = true;
            }
            break;
        case 'Q':
            if( USB_Msg_Length() >= _Message_Length( 'Q' ) ) {
                USB_Msg_Get();

                float repeat_time;
                USB_Msg_Read_Into( &repeat_time, sizeof( repeat_time ) );

                if( repeat_time > 0 ) {
                    Task_Activate( &task_send_system_id, repeat_time );
                } else {
                    Task_Cancel( &task_send_system_id );
                }

                command_processed = true;
            }
            break;
        default:
            // What to do if you dont recognize the command character
            USB_Send_Msg( "cc", '?', &command, sizeof( command ) );
            USB_Flush_Input_Buffer();
            break;
    }

    //********* MEGN540 -- LAB 2 ************//
    if( command_processed ) {
        // RESET the WATCHDOG TIMER
        Task_Activate( &task_message_handling_watchdog, 0.1 );
    }
}

/**
 * @brief Function Task_Message_Handling_Watchdog clears the USB recieve (deleting all messages) to flush the buffer if a complete message is not recieved
 * whithin an appropriate amount of time (say 250ms)
 *
 * @param _unused_
 */
void Task_Message_Handling_Watchdog( float _unused_ )
{
    USB_Flush_Input_Buffer();
}

/**
 * Function _Message_Length returns the number of bytes associated with a
 * command string per the class documentation;
 * @param cmd
 * @return Size of expected string. Returns 0 if unreconized.
 */
static uint8_t _Message_Length( char cmd )
{
    switch( cmd ) {
        case '~': return 1; break;
        case '*': return 9; break;
        case '/': return 9; break;
        case '+': return 9; break;
        case '-': return 9; break;
        case 't': return 2; break;
        case 'T': return 6; break;
        case 'e': return 1; break;
        case 'E': return 5; break;
        case 'b': return 1; break;
        case 'B': return 5; break;
        case 'p': return 5; break;
        case 'P': return 9; break;
        case 's': return 1; break;
        case 'S': return 1; break;
        case 'q': return 1; break;
        case 'Q': return 5; break;
        case 'd': return 9; break;
        case 'D': return 13; break;
        case 'v': return 9; break;
        case 'V': return 13; break;
        default: return 0; break;
    }
}