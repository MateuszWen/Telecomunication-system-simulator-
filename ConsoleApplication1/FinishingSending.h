#ifndef FINISHINGSENDING_H
#define FINISHINGSENDING_H

#include "Packet.h"

class FinishingSending : public Event, public Logger
{
public:
	Packet* packet; // indicator to serviced packet 
	WirelessNetwork* wireless_network_; // indicator to WirelessNetwork
	Simulator* simulator_;// indicator to Simulator

	FinishingSending(int execute_time_, Packet* packet, WirelessNetwork* wireless_network,	Simulator* simulator);

	void Execute() override;	
};

#endif 
