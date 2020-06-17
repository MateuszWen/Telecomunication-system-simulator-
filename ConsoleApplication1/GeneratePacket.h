#ifndef GENERATEPACKET_H
#define GENERATEPACKET_H

#include <list>
#include <vector>
#include "Event.h"
#include "Logger.h" 
#include "WirelessNetwork.h"
#include "Simulator.h"

using namespace std;

class WirelessNetwork;
class Simulator;

class GeneratePacket : public Event , public Logger
{
public:
	int trans_receiv_number_; // number of destination receiver 
	WirelessNetwork* wireless_network; //indicator to WirelessNetwork
	Simulator* simulator;//indicator to Simulator 


	GeneratePacket(int execute_time_, int trans_receiv_number, WirelessNetwork* wireless_network, Simulator* simulator);

	~GeneratePacket();
	
	virtual void Execute() override;
private:
};

#endif

