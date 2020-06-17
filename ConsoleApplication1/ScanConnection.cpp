#include "ScanConnection.h"

ScanConnection::ScanConnection(Packet* packet, int execute_time_, int how_many_free, int trans_receiv_number, WirelessNetwork* wireless_network,
	Simulator* simulator)
	: Event(execute_time_, 3),
	packet(packet),
	how_many_free_(how_many_free),
	trans_receiv_number_(trans_receiv_number),
	wireless_network(wireless_network),
	simulator(simulator)
{
}

void ScanConnection::Execute()
{
	simulator->scanConnectionQueue.push(this);
}




