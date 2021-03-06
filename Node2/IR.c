/*
 * IR.c
 *
 * Created: 01.11.2017 11:18:02
 *  Author: mariuesk
 */ 
#define IR0 0
#define IR1 1
#define N_READS 4

#include "IR.h"
static uint16_t readings[2][N_READS] = {{0}};
static uint8_t read_index = 0;
static uint16_t total_read[2] = {0};
static uint16_t IR0_low;
static uint16_t IR1_low;
static uint8_t goal = 0;
static uint8_t prev_goal_status = 0;

void IR_init(void){
	ADC_init();
}

void IR_calibrate(void){
	for(uint8_t i = 0; i <30; i++){
		IR_MM();
	}
	uint16_t last_read[2] = {0};
	int diff;
	do 
	{	
		last_read[IR0] = total_read[IR0];
		last_read[IR1] = total_read[IR1];
		IR_MM();
		diff = (int)(last_read-total_read[IR1]);
	} while (!(diff < 10 || diff > -10));
	IR0_low = last_read[IR0]/2;
	IR1_low = 4*last_read[IR1]/5;
}

void IR_MM(void){
	
	total_read[IR0]-= readings[IR0][read_index];
	total_read[IR1]-= readings[IR1][read_index];
	
	readings[IR0][read_index] = ADC_read_channel(IR0);
	readings[IR1][read_index] = ADC_read_channel(IR1);
	
	total_read[IR0] += readings[IR0][read_index];
	total_read[IR1] += readings[IR1][read_index];
	
	read_index++;
	if(read_index >= N_READS){
		read_index = 0;
	}	
}

uint8_t is_goal(void){

	if(total_read[IR1] < IR1_low && !prev_goal_status){
		prev_goal_status = 1;
		goal++;
	}else if(!(total_read[IR1] < IR1_low)){
		prev_goal_status = 0;
	}
	if(goal > 1){
		goal = 0;
		return 1;
	}
	return 0;
}
uint8_t is_game_over(void){
	if(total_read[IR0] < IR0_low){
		return 1; 
	}
	return 0;
}

