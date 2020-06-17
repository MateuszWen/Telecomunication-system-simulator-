#include "GeneratePacket.h"

#include "WirelessNetwork.h"

GeneratePacket::GeneratePacket(int execute_time_, int trans_receiv_number, WirelessNetwork* wireless_network, Simulator* simulator)
	: Event(execute_time_, 1),
	trans_receiv_number_(trans_receiv_number),
	wireless_network(wireless_network),
	simulator(simulator)
{
}
GeneratePacket::~GeneratePacket() {};

void GeneratePacket::Execute()
{
	Logger logger;
	string text = "Generate new packet for transmitter ";
	logger.Info(text.append(to_string(trans_receiv_number_)).append(". Simulation time: ").append(to_string(static_cast<double>(simulator->actual_time_)/2)), simulator->text_mode_, simulator);

	//create new packet for transmitter 
	Packet* pac = new Packet((simulator->last_packet_id + 1), trans_receiv_number_, (wireless_network->transmitting_antennae.at(trans_receiv_number_)->generator_CTP->Rand(1, 10)) * 2/* *2 because, 1 loop = 0.5 ms */,
		wireless_network->LR, simulator->actual_time_);

	wireless_network->transmitting_antennae.at(trans_receiv_number_)->add_Packet(pac, wireless_network, simulator);
	simulator->last_packet_id++;

	//create new event of generate packet 
	GeneratePacket* gp = new GeneratePacket((simulator->actual_time_ + ((wireless_network->transmitting_antennae.at(trans_receiv_number_)->generator_CGP->RandEXP(wireless_network->lambda_)))),
		trans_receiv_number_, wireless_network, simulator);
	simulator->add_and_sort_events(gp);
	
}

