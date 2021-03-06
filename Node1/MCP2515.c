/*
 * MCP2515.c
 *
 * Created: 04.10.2017 16:46:54
 *  Author: andersgd
 */ 
#include "MCP2515.h"

uint8_t mcp2515_read(uint8_t address){
	uint8_t result;
	PORTB &= ~(1<<PB4); //Chip select CAN-controller
	
	SPI_send(MCP_READ); // Send read command
	SPI_send(address); // Send address
	result = SPI_read(); // Read result
	
	PORTB |= (1<<PB4); // Deselect CAN - controller
	return result;
}

void mcp2515_write(uint8_t address, uint8_t cData){
	PORTB &= ~(1<<PB4); // Chip select CAN-controller
	
	SPI_send(MCP_WRITE); // Send write command
	SPI_send(address); // Send address
	SPI_send(cData); // Send data
	
	PORTB |= (1<<PB4); // Deselect CAN - controller
}

void mcp2515_reset(void){
	PORTB &= ~(1<<PB4); // Chip select CAN - controller
	
	SPI_send(MCP_RESET); // Set registers to default state
	mcp2515_bit_modify(MCP_CANCTRL, MODE_CONFIG,MODE_CONFIG); // Force the CAN - controller to enter configuration mode
	
	PORTB |= (1<<PB4); //Deselect CAN - controller

	_delay_us(10);
}

void mcp2515_set_mode(uint8_t mode){
	mcp2515_write(MCP_CANCTRL, mode); // Write mode to MCP_CANCTRL register
}

void mcp2515_request_to_send(uint8_t buffer){
	PORTB &= ~(1<<PB4); // Chip select CAN - controller
	
	SPI_send(buffer); // Send to chosen buffer
	
	PORTB |= (1<<PB4); // Deselect CAN - controller
}

uint8_t mcp2515_read_status(){
	uint8_t status;
	PORTB &= ~(1<<PB4); // Chip select CAN - controller

	SPI_send(MCP_READ_STATUS); // Read the status
	status = SPI_read();

	PORTB |= (1<<PB4); // Deselect CAN - controller
	return status;
}

void mcp2515_bit_modify(uint8_t regist, uint8_t mask, uint8_t cData){
	PORTB &= ~(1<<PB4); // Chip select CAN - controller
	
	SPI_send(MCP_BITMOD); // Ready bit modify instruction
	SPI_send(regist); // Choose register
	SPI_send(mask); // Send bit mask
	SPI_send(cData); // Modify the bits to match cData
	
	PORTB |= (1<<PB4); // Deselect CAN - controller
}

uint8_t mcp2515_init(uint8_t mode){
	uint8_t mode_value;
	SPI_master_init(); //Initialize SPI
	mcp2515_reset(); // Send reset-command

	/*Self-test*/
	mode_value = mcp2515_read(MCP_CANSTAT); // Read current mode
	if((mode_value& MODE_MASK)  != MODE_CONFIG) {
		//printf("MCP2515 is NOT in configuration mode after reset! canstat: 0x%02x\n", theValue);
		return 1;
	} 
	else{
		//printf("MCP2515 is  in configuration mode after reset! canstat: 0x%02x\n", theValue);
	}
	
	mcp2515_set_mode(mode);
	mode_value = mcp2515_read(MCP_CANSTAT);
	int mode_bits = (mode_value & MODE_MASK);
	if(mode_bits != mode){ // Check if chosen mode is the same as current mode
		printf("MCP2515 is NOT in correct mode after reset! Its config bits are %x\n", mode_bits);
		return 1;
	}
	mcp2515_write(MCP_CANINTE, MCP_RX_INT);// Enable interrups
	return 0;
}
