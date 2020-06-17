#ifndef SCANCONNECTION_H
#define SCANCONNECTION_H

#include "Event.h"
#include "Logger.h"
#include "Packet.h"
#include  "WirelessConnection.h"

class WirelessNetwork;
class Simulator;

class ScanConnection : public Event , public Logger
{
public:
	ScanConnection();

	int how_many_free_; //how many times connectivity was free
	int trans_receiv_number_; //number of receiver where packet have to arrived 
	WirelessNetwork* wireless_network; // indicator to WirelessNetwork
	Simulator* simulator; // indicator to Simulator
	Packet* packet; // packet whih want to get access to connectivity 


	ScanConnection(Packet* packet, int execute_time_, int how_many_free, int trans_receiv_number, WirelessNetwork* wireless_network, Simulator* simulator);

	void Execute() override;
	
};
#endif 



