#ifndef RECEIVEPACKET_H
#define RECEIVEPACKET_H

#include "Event.h"
#include "Logger.h"
#include "Packet.h"
#include "Simulator.h"
#include "WirelessNetwork.h"

class ReceivePacket : public Event, public Logger 
{
public:
	Simulator* simulator_; // indicator to Simulator
	Packet* packet_;// indicator to Packet
	WirelessNetwork* wireless_network_; // indicator to WirelessNetwork
	bool was_collision_; //contain information about collision, if in connectivity was more than 1 packet, flag is setted on true, otherwise false  

	ReceivePacket(int execute_time_, Simulator* simulator, Packet* packet, WirelessNetwork* wireless_network, bool was_collision);
	void Execute() override;
};
#endif // RECEIVEPACKET_H


