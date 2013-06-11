/*SerialParser.h - Arduino Serial Parser Library Header File
Created by Matthew Adams, last updated: 6/10/13
*/

#ifndef Serial_Parser_h
#define Serial_Parser_h




class SerialParser
{
public:
	SerialParser(int baud_rate);
	void end();
	void run();
	void get_data(*uint8_t id_tag, *uint8_t data);
	bool new_packet();
	
private:
	void state_1();
	void state_2();
	void state_3();
	void state_4();
	void state_5();
	void state_6();
	void state_7();
	char state;
	const uint8_t buffer_size;
	const uint8_t stored_packets;
	extern uint8_t buffer[buffer_size];
	extern uint8_t input[buffer_size];
	extern uint8_t id_tag_stored[stored_packets][2];
	bool packet_avail;
	extern uint8_t data_stored[stored_packets][3];
	uint8_t buffer_count;
	uint8_t input_count;
	uint8_t packet_count;
	uint8_t tag_id1;
	uint8_t tag_id2;
	unint8_t databyte1;
	unint8_t databyte2;
	unint8_t databyte3;
};
#endif