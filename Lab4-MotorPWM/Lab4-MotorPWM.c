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

#include "Message_Handling.h"  // for translating USB messages to microcontroller tasks
#include "SerialIO.h"          // for USB communication
#include "Task_Management.h"   // for clean task management with functors
#include "Timing.h"            // for Time understanding

// put your task includes and/or function declarations here for future population
#include "Lab1_Tasks.h"
#include "Lab2_Tasks.h"
#include "Lab3_Tasks.h"
#include "Lab4_Tasks.h"

void Initialize_Modules( float _time_not_used_ )
{
    // Initialize (reinitialize) all global variables

    // Initialize all modules
    Initialize_USB();
    Initialize_Timing();
    Initialize_Battery_Monitor();
    Initialize_Encoders();

    Initialize_MotorPWM( 400 );
    // MotorPWM_Set_Left( 200 );

    // MotorPWM_Enable( true );

    // Setup task handling
    Initialize_Task( &task_restart, Initialize_Modules /*function pointer to call*/ );

    // Setup message handling to get processed at some desired rate.
    Initialize_Task( &task_message_handling, Task_Message_Handling );
    Task_Activate( &task_message_handling, 0 /*as fast as possible*/ );

    Initialize_Task( &task_message_handling_watchdog, Task_Message_Handling_Watchdog );
    Task_Activate( &task_message_handling_watchdog, 0.1 );
    Initialize_Task( &task_time_loop, Send_Loop_Time );
    Initialize_Task( &task_send_time, Send_Time_Now );

    Initialize_Task( &task_send_encoder_counts, Send_Encoder_Counts );
    Initialize_Task( &task_send_battery_voltage, Send_Battery_Voltage );
    Initialize_Task( &task_check_battery, Check_Battery_Voltage );
    Initialize_Task( &task_send_low_battery, Send_Battery_Low_Warning );
    Task_Activate( &task_check_battery, .002 );  // every 2ms

    Initialize_Task( &task_timed_motor_run, Timed_Motor_Run );
    Initialize_Task( &task_send_system_id, Send_System_ID );
}

int main()
{
    Initialize_Modules( 0.0 );
    // call initialization stuff

    for( ;; ) {
        // main loop logic
        Task_USB_Upkeep();

        Task_Run_If_Ready( &task_message_handling );
        Task_Run_If_Ready( &task_restart );

        Task_Run_If_Ready( &task_message_handling_watchdog );
        Task_Run_If_Ready( &task_send_time );
        Task_Run_If_Ready( &task_time_loop );

        Task_Run_If_Ready( &task_send_encoder_counts );
        Task_Run_If_Ready( &task_send_battery_voltage );

        Task_Run_If_Ready( &task_check_battery );
        Task_Run_If_Ready( &task_send_low_battery );

        Task_Run_If_Ready( &task_timed_motor_run );
        Task_Run_If_Ready( &task_send_system_id );
    }

    return 0;
}
