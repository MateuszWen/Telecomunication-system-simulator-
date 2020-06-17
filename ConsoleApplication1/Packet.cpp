#include "Packet.h"
//#include "WirelessNetwork.h"


Packet::Packet(int packed_id, int trans_rec_number, int time_of_service, int possible_retransmissions, int start_life_time)
	: packed_id(packed_id),
	trans_rec_number(trans_rec_number),
	time_of_service(time_of_service),
	possible_retransmissions(possible_retransmissions),
	start_life_time(start_life_time),
	waiting_time_that_was_setted(false)
{
}

Packet::~Packet() {}





