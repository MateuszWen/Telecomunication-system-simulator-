#include "WirelessConnection.h"

WirelessConnection::WirelessConnection(bool acces_possible) : acces_possible(acces_possible){}


void WirelessConnection::set_packets(const vector<Packet*>& packets)
{
	this->packets = packets;
}

void WirelessConnection::delete_all_packets_from_wire_connection()
{
	this->packets.clear();
}

Packet* WirelessConnection::get_front_packet_from_wire_connection()
{
	return this->packets.front();
}

bool WirelessConnection::acces_possible1() const
{
	return acces_possible;
}

void WirelessConnection::set_acces_possible(bool acces_possible)
{
	this->acces_possible = acces_possible;
}

