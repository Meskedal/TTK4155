/*
 * Node2.c
 *
 * Created: 18.10.2017 18:38:19
 *  Author: mariuesk
 */ 


#include "Usart_Driver.h"
#include "CAN.h"
#include "servo.h"
#include "IR.h"
#include "DAC.h"
#include "Motor.h"
#include "solenoid.h"
#include "PID.h"
#include "Pong.h"

int main(void){	
	USART_Init();
	DAC_init();
	motor_init();
	//PID_init();
    CAN_init(MODE_NORMAL);
	servo_init(F_CPU);
	IR_init();
	solenoid_init();
	CAN_message msg;
	while(1)
    {	
		if(CAN_receive(&msg)){
			if (msg.id==0){
				PlayPong(msg.data[0]);
			}
		}
	
    }
}