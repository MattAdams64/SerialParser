/* SerialParser.cpp - Library for an Arduino finite-state machine parser for reading data from the serial port.
Created by Matthew Adams, last updated: 6/24/13

Version 01

Structure of packets: 5 bytes, all 8 bit unsigned integers
ID tag (2 bytes) | Data (3 bytes)

Max number of stored packets is defined in header file.
*/ 

#include "Arduino.h"
#include "SerialParser.h"

SerialParser::SerialParser()
{
}

// Initialize serial communications and state
void SerialParser::begin(int baud_rate)
{
	// Start serial connections
	Serial.begin(baud_rate);
	
	// Initialize state
	state = 1;
}

// Run the parser through the finite state machine 
void SerialParser::run()
{
	switch(state)
	{
		case 1:
			state_1();
			break;
		case 2:
			state_2();
			break;
		case 3:
			state_3();
			break;
		case 4:
			state_4();
			break;
		case 5:
			state_5();
			break;
		case 6:
			state_6();
			break;
		case 7:
			state_7();
			break;
	}
}		

//Idle state 	
void SerialParser::state_1()
{
	if(Serial.available()>0)
	{
		state = 2;
		return;
	}
}

//Read first tag ID
void SerialParser::state_2()
{
	id_tag1 = Serial.read();
	state = 3;
}

//Read second tag ID
void SerialParser::state_3()
{
	if(Serial.available()>0)
	{
		id_tag2 = Serial.read();
		state = 4;
		return;
		}
}

//Read first databyte
void SerialParser::state_4()
{
	if(Serial.available()>0)
	{
		databyte1 = Serial.read();
		state = 5;
		return;
	}	
}

//Read second databyte
void SerialParser::state_5()
{
	if(Serial.available()>0)
	{
		databyte2 = Serial.read();
		state = 6;
		return;
	}
}

//Read third databyte
void SerialParser::state_6()
{
	if(Serial.available()>0)
	{
		databyte3 = Serial.read();
		state = 7;
		return;
	}
}

//Indicate Packet is available, save ID tags and databytes into arrays
void SerialParser::state_7()
{	
	
	//Store tag/data packet in multidimensional arrays
	id_tag_stored[packet_count][0] = id_tag1;
	id_tag_stored[packet_count][1] = id_tag2;
	
	data_stored[packet_count][0] = databyte1;
	data_stored[packet_count][1] = databyte2;
	data_stored[packet_count][2] = databyte3;
	packet_count++;
	//if packet_count builds up, just overwrite last value
	if (packet_count>STORED_PACKETS-1)
	{
		packet_count--;
	}
	state = 1;
	packet_avail = true;
}

//Function returns where a packet is available or not
bool SerialParser::check_packet()
{
	return packet_avail;
}

// Transfers packet information through two passed arrays
void SerialParser::get_packet(uint8_t *id_tag, uint8_t *data)
{
	//transfer stored tag/data to arrays passed in function
	id_tag[0] = id_tag_stored[0][0];
	id_tag[1] = id_tag_stored[0][1];
	
	data[0] = data_stored[0][0];
	data[1] = data_stored[0][1];
	data[2] = data_stored[0][2];
	
	//shift stored tag/data arrays to remove passed data/tag
	for (uint8_t i=0;i<packet_count-1;i++)
	{
		id_tag_stored[i][0] = id_tag_stored[i+1][0];
		id_tag_stored[i][1] = id_tag_stored[i+1][1];
		
		data_stored[i][0] = data_stored[i+1][0];
		data_stored[i][1] = data_stored[i+1][1];
		data_stored[i][2] = data_stored[i+1][2];
	}
	packet_count--;
	
	//if no more packets, make indicator false
	if (packet_count == 0)
	{
		packet_avail = false;
	}
}

// Function returns current state of machine (debugging purposes)
uint8_t SerialParser::get_state()
{
return state;
}

// Function returns number of packets available
uint8_t SerialParser::get_packetcount()
{
return packet_count;
}

