#include "Lab3_Tasks.h"

void Send_Encoder_Counts( float _time_since_last )
{
    char cmd = task_send_encoder_counts.run_period < 0 ? 'e' : 'E';
    struct __attribute__( ( __packed__ ) ) {
        float leftCount;
        float rightCount;
    } data = { .leftCount = Encoder_Counts_Left(), .rightCount = Encoder_Counts_Right() };
    USB_Send_Msg( "cff", cmd, &data, sizeof( data ) );
}

static void Send_Battery_Low_Warning( float bat_volt )
{
    struct __attribute__( ( __packed__ ) ) {
        char let[7];
        float volt;
    } msg = { .let = { 'B', 'A', 'T', ' ', 'L', 'O', 'W' }, .volt = bat_volt };
    // Send Warning to Serial that batteries need to be charged
    USB_Send_Msg( "c7sf", '!', &msg, sizeof( msg ) );
}

void Send_Battery_Voltage( float _time_since_last )
{
    char cmd       = task_send_battery_voltage.run_period < 0 ? 'b' : 'B';
    float bat_volt = Battery_Voltage();

    if( bat_volt < 0.9 * 4 ) {
        Send_Battery_Low_Warning( bat_volt );
    } else {
        USB_Send_Msg( "cf", cmd, &bat_volt, sizeof( bat_volt ) );
    }
}

void Check_Battery_Voltage( float _time_since_last )
{
    float bat_volt = Battery_Voltage();
    if( bat_volt < 0.9 * 4 ) {
        Send_Battery_Low_Warning( bat_volt );
    }
}