/* SerialParser.cpp - Library for parser state-machine reading data from serial port.
Created by Matthew Adams, last updated: 6/10/13
*/ 

#include "Arduino.h"
#include "SerialParser.h"

const uint8_t buffer_size = 10;
const uint8_t stored_packets = 5;

void SerialParser::SerialParser(int baud_rate)
{
	// Start serial connections
	Serial.begin(baud_rate);

	// Initialize state
	char state = 1;

	// Initialize buffer
	uint8_t buffer[buffer_size] = {};

	// Initialize input
	uint8_t input[buffer_size] = {};

	// Initialize packet availability status
	bool packet_avail = false;

	// initialize tag matrix
	uint8_t id_tag_stored[stored_packets][2] = {};

	// initialize data matrix
	uint8_t data_stored[stored_packets][3] = {};
	
	//initialize buffer count
	uint8_t buffer_count = 0;

	//initialize input count
	uint8_t input_count = 0;
	
	//initialize packet count
	uint8_t packet_count = 0;

	//initialize individual tag ids
	uint8_t tag_id1 = 0;
	uint8_t tag_id2 = 0;

	//initialize individual data bytes
	unint8_t databyte1 = 0;
	unint8_t databyte2 = 0;
	unint8_t databyte3 = 0;
}

void SerialParser::run()
{
	switch (state)
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
//idle state 	
void SerialParser::state_1()
{
	if(Serial.available() || buffer_count!=0)
	{
		state = 2;
	}
}

//Get first tag ID
void SerialParser::state_2()
{
	for(uint8_t i=0; i<buffer_count ; i++)
	{
		input[i] = buffer[i]; //transfer data from buffer to input
		buffer[i] = 0; //clear buffer value after transferring to input
    }
	
	//Transfer buffer_count to input_count
	input_count = buffer_count;
	//Clear buffer count
	buffer_count = 0;
	
	//Read serial data, save as input
	while(Serial.available()>0 && input_count != buffer_size)
	{
		input[input_count] = Serial.read();
		input_count += 1;
	}
	
	//If only one input, save as tag ID 1
	if (input_count == 1)
	{
		tag_id1 = input[0];
		//clear input count
		input_count = 0;
		//Go to state 3
		state = 3;
		return;
	}
	//If two or more inputs, save as tag ID 1 and 2 and transfer rest to buffer
	else if (input_count >= 2)
	{	
		tag_id1 = input[0];
		tag_id2 = input[1];
		//Transfer remaining input to buffer
		for (i = input_count; i>2 ; i--)
			{
				buffer[buffer_count] = input[i-1];
				buffer_count++;
			}
	    //clear input count
		input_count = 0;
		//Go to state 4
		state = 4;
		return;
	}
}

//Get second tag ID
void SerialParser::state_3()
{
	//Read serial data, save as input
	while(Serial.available()>0 && input_count != buffer_size)
	{
		input[input_count] = Serial.read();
		input_count += 1;
	}
	//If one or more input, save as tag ID 2 and send rest to buffer	
	if (input_count > 1)
	{
		tag_id2 = input[0];
		//Transfer remaining input to buffer
		for (i = input_count; i>1 ; i--)
		{
			buffer[buffer_count] = input[i-1];
			buffer_count++;
		}
		//clear input count
		input_count = 0;
		//Go to state 4
		state = 4;
		return;
	}
}

//Get first databyte
void SerialParser::state_4()
{
	for(i=0; i<buffer_count ; i++)
	{
		input[i] = buffer[i]; //transfer data from buffer to input
		buffer[i] = 0; //clear buffer value after transferring to input
	}		
	
	//Transfer buffer_count to input_count
	input_count = buffer_count;
	//Clear buffer count
	buffer_count = 0;
	
	//Read serial data, save as input
	while(Serial.available()>0 && input_count != buffer_size)
	{
		input[input_count] = Serial.read();
		input_count += 1;
	}
	//if one input, save as databyte 1
	if (input_count == 1)
	{
		databyte1 = input[0];
		//clear input count
		input_count = 0;
		//Go to state 5
		state = 5;
	}
	//if two inputs, save first as databyte 1 and second as databyte 2
	else if (input_count ==2)
	{
		databyte1 = input[0];
		databyte2 = input[1];
		//clear input count
		input_count = 0;
		//Go to state 6
		state = 6;
	}
	
	else if (input_count >= 3)
	{	
		databyte1 = input[0];
		databyte2 = input[1];
		databyte3 = input[2];
		//Transfer remaining input to buffer
		for (i = input_count; i>3 ; i--)
		{
			buffer[buffer_count] = input[i-1];
			buffer_count++;
		}
		//clear input count
		input_count = 0;
		//Go to state 7
		state = 7;
		return;
	}
}

//Get second databyte
void SerialParser::state_5()
{
	for(i=0; i<buffer_count ; i++)
	{
		input[i] = buffer[i]; //transfer data from buffer to input
		buffer[i] = 0; //clear buffer value after transferring to input
	}		
	
	//Transfer buffer_count to input_count
	input_count = buffer_count;
	//Clear buffer count
	buffer_count = 0;
	
	//Read serial data, save as input
	while(Serial.available()>0 && input_count != buffer_size)
	{
		input[input_count] = Serial.read();
		input_count += 1;
	}
	//if one input, save as databyte 2
	if (input_count == 1)
	{
		databyte2 = input[0];
		//clear input count
		input_count = 0;
		//Go to state 6
		state = 6;
	}
	//if two or more inputs, save first as databyte 2 and second as databyte 3, send rest to buffer
	else if (input_count >=2)
	{
		databyte2 = input[0];
		databyte3 = input[1];
		//Transfer remaining input to buffer
		for (i = input_count; i>2 ; i--)
		{
			buffer[buffer_count] = input[i-1];
			buffer_count++;
		}
		//clear input count
		input_count = 0;
		//Go to state 7
		state = 7;
		return;
	}
}

//Get third databyte
void SerialParser::state_6()
{
	for(i=0; i<buffer_count ; i++)
	{
		input[i] = buffer[i]; //transfer data from buffer to input
		buffer[i] = 0; //clear buffer value after transferring to input
	}		
	
	//Transfer buffer_count to input_count
	input_count = buffer_count;
	//Clear buffer count
	buffer_count = 0;
	
	//Read serial data, save as input
	while(Serial.available()>0 && input_count != buffer_size)
	{
		input[input_count] = Serial.read();
		input_count += 1;
	}
	//if one input or more, save first as databyte 3, send rest to buffer
	if (input_count >= 1)
	{
		databyte3 = input[0];
		//Transfer remaining input to buffer
		for (i = input_count; i>1 ; i--)
		{
			buffer[buffer_count] = input[i-1];
			buffer_count++;
		}
		//clear input count
		input_count = 0;
		//Go to state 7
		state = 7;
		return;
	}
}
//Indicate Packet is available, save tag ids and databytes into arrays
void SerialParser::state_7()
{	
	packet_avail = true;
	//Store tag/data packet in multidimensional arrays
	id_tag_stored[packet_count][0] = id_tag1;
	id_tag_stored[packet_count][1] = id_tag2;
	
	data_stored[packet_count][0] = databyte1;
	data_stored[packet_count][1] = databyte2;
	data_stored[packet_count][2] = databyte3;
	packet_count++;
	//if packet_count builds up, just overwrite last value
	if (packet_count>stored_packets-1)
	{
		packet_count--;
	}
}

bool SerialParser::check_packet()
{
	return(packet_avail);
}

void SerialParser::get_packet(uint8_t *id_tag, uint8_t *data)
{
	//transfer stored tag/data to arrays passed in function
	id_tag[0] = id_tag_stored[0][0];
	id_tag[1] = id_tag_stored[0][1];
	
	data[0] = data_stored[0][0];
	data[1] = data_stored[0][1];
	data[2] = data_stored[0][2];
	//shift stored tag/data arrays to remove passed data/tag
	for (i=0;i<packet_count-1;i++)
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
		packet_avail = false;;
	}
}