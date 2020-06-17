#ifndef RECEIVINGANTENNA_H
#define RECEIVINGANTENNA_H

using namespace std;

#include "Packet.h"

class Packet;


class ReceivingAntenna
{
public:
	ReceivingAntenna(int _receiver_ID);
	~ReceivingAntenna();

	bool was_collision_ = false; // is true, if packet was received correctly 
	Packet* packet_; // indicator to packet which is received by receiver  
	
private:
	int _receiver_ID; // number of receiver 


};

#endif


