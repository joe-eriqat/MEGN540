#include "Lab2_Tasks.h"

void Send_Loop_Time( float _time_since_last )
{
    static uint8_t actively_timing = 0;
    static float run_period_store  = 0;

    // struct {
    //     uint8_t command;
    //     float time;
    // } data2 = { .command = 0, .time = ( _time_since_last ) };
    // USB_Send_Msg( "cBf", 't', &data2, sizeof( data2 ) );

    if( actively_timing == 1 ) {
        actively_timing = 0;
        struct {
            uint8_t command;
            float time;
        } data = { .command = 1, .time = ( _time_since_last ) };
        USB_Send_Msg( "cBf", 't', &data, sizeof( data ) );
        if( run_period_store < 0 ) {
            Task_Cancel( &task_time_loop );

        } else {
            Task_Activate( &task_time_loop, run_period_store );
        }
    } else if( actively_timing == 0 ) {
        run_period_store = task_time_loop.run_period;
        actively_timing  = 1;
        Task_Activate( &task_time_loop, 0 );
    }
}

void Send_Time_Now( float _time_since_last )
{
    // float current_time = 0.0;

    struct {
        uint8_t command;
        float current_time;
    } data = { .command = 0, .current_time = Timing_Get_Time_Sec() };

    // send response right here if appropriate.
    USB_Send_Msg( "cBf", 't', &data, sizeof( data ) );

    // float ret_val = 3.0 * 6.0;

    // // send response right here if appropriate.
    // USB_Send_Msg( "cf", '*', &ret_val, sizeof( ret_val ) );
}