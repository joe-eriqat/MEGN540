#include "Lab4_Tasks.h"

void Set_Motor_PWM( uint16_t Left_PWM, uint16_t Right_PWM )
{
    MotorPWM_Set_Left( Left_PWM );
    MotorPWM_Set_Right( Right_PWM );
}
void Timed_Motor_Run( float _time_since_last )
{
    // static uint8_t run_motor = 1;
    // MotorPWM_Enable( true );
    MotorPWM_Enable( false );
    Set_Motor_PWM( 0, 0 );
    Task_Cancel( &task_timed_motor_run );
}

void Send_System_ID( float _time_since_last )
{
    char command = 'q';
    if( task_send_system_id.run_period > 0 ) {
        command = 'Q';
    } else {
        command = 'q';
    }
    struct __attribute__( ( __packed__ ) ) {
        float time;
        uint16_t Left_PWM;
        uint16_t Right_PWM;
        uint16_t Left_Encoder;
        uint16_t Right_Encoder;
    } Sys_ID;
    Sys_ID.Left_PWM      = MotorPWM_Get_Left();
    Sys_ID.Right_PWM     = MotorPWM_Get_Right();
    Sys_ID.Left_Encoder  = Encoder_Counts_Left();
    Sys_ID.Right_Encoder = Encoder_Counts_Right();
    Sys_ID.time          = Timing_Get_Time_Sec();

    USB_Send_Msg( "cf4h", command, &Sys_ID, sizeof( Sys_ID ) );
}