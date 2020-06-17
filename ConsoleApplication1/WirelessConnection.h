#ifndef WIRELESSCONNECTION_H
#define WIRELESSCONNECTION_H

#include <vector>
#include "Packet.h"

class Packet;

using namespace std;

class WirelessConnection
{
public:
	WirelessConnection(bool acces_possible);

	bool acces_possible1() const;
	
	void set_acces_possible(bool acces_possible);
	void set_packets(const vector<Packet*>& packets);
	void delete_all_packets_from_wire_connection();
	Packet* get_front_packet_from_wire_connection();

	vector<Packet*> packets; // contain currently servicet packet (or packets if collision occured)

private:
	bool acces_possible; //contain information about availability of wireless connection 
	
};
#endif // WIRELESSCONNECTION_H

