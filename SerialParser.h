/*SerialParser.h - Arduino Serial Parser Class Library Header File

Created by Matthew Adams, last updated: 6/24/13

Version 01

Methods:

	SerialParser - Class for operating finite state machine parser for Arduino serial communications

	begin(int baud_rate) - initializes Serial port communications, state machine

	run() - runs the state machine (1 state per command)

	check_packet() - returns packet availability status

	get_packet(uint8_t *id_tag, uint8_t *data) - returns oldest stored packet as two arrays (id_tag and data)

	get_state() - returns current state of machine

	get_packetcount() - returns number of stored packets

*/

#ifndef Serial_Parser_h
#define Serial_Parser_h

#include <stdint.h>

// Defines maximum number of stored packets
#define STORED_PACKETS 5

class SerialParser
{
public:
	SerialParser();
	void begin(int baud_rate);
	void run();
	void get_packet(uint8_t *id_tag, uint8_t *data);
	bool check_packet();
	uint8_t get_state();
	uint8_t get_packetcount();
	
private:
	void state_1();
	void state_2();
	void state_3();
	void state_4();
	void state_5();
	void state_6();
	void state_7();
	uint8_t state;
	bool packet_avail;
	uint8_t id_tag_stored[STORED_PACKETS][2];
	uint8_t data_stored[STORED_PACKETS][3];
	uint8_t packet_count;
	uint8_t id_tag1;
	uint8_t id_tag2;
	uint8_t databyte1;
	uint8_t databyte2;
	uint8_t databyte3;
};
#endif