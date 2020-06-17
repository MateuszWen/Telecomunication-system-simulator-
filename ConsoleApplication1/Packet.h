#ifndef PACKET_H
#define PACKET_H

#include "Packet.h"
#include "Simulator.h"
#include "WirelessNetwork.h"

using namespace std;


class Packet
{
public:
	int packed_id; //oryginal packet number in all system 
	int trans_rec_number; // destination receiver number 
	int time_of_service; // required service time (how long time packet will be transmitted) 
	int possible_retransmissions; // how many times packet can by retransmitted 

	//data for statistics
	int start_life_time; // time when packet was created 
	bool waiting_time_that_was_setted; // info is required to calculate time from creation to first transmission 


	Packet(int packed_id, int trans_rec_number, int time_of_service, int possible_retransmissions, int start_life_time);

	~Packet();
};

#endif


