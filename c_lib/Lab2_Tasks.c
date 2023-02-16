#include "Lab2_Tasks.h"

void Send_Loop_Time( float _time_since_last )
{

    Time_t last_time = {
        .millisec = _time_since_last,
        .microsec = 0  // YOU NEED TO REPLACE THIS WITH A CALL TO THE TIMER0 REGISTER AND MULTIPLY APPROPRIATELY
    };

    float delta_time = Timing_Seconds_Since( &last_time );

    struct {
        uint8_t command;
        float time;
    } data = { .command = 0, .time = delta_time };

    // send response right here if appropriate.
    USB_Send_Msg( "cBf", 't', &data, sizeof( data ) );
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