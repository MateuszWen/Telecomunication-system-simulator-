#include "TransmittingAntenna.h"

#include "ScanConnection.h"


TransmittingAntenna::TransmittingAntenna(int transmiter_id, double kernel_R, double kernel_CGP, double kernel_CTP, Simulator* simulator)
{
	this->transmiter_id = transmiter_id;
	this->kernel_R_ = kernel_R;
	this->kernel_CGP_ = kernel_CGP;
	this->kernel_CTP_ = kernel_CTP;
	this->simulator_ = simulator;
	this->generator_R = new UniformGenerator(kernel_R_, simulator);
	this->generator_CGP = new UniformGenerator(kernel_CGP_, simulator);
	this->generator_CTP = new UniformGenerator(kernel_CTP_, simulator);
}

void TransmittingAntenna::add_Packet(Packet* packet, WirelessNetwork* wireless_network, Simulator* simulator)
{
	this->packets.push(packet);

	//if added packet is first packet in buffor, schedule his ScanConnection 
	if(packets.size() == 1)
	{
		ScanConnection* sc = new ScanConnection(packet, simulator->actual_time_ + simulator->wireless_network_->S, 0, packet->trans_rec_number, wireless_network, simulator);
		simulator->add_and_sort_events(sc);
	}
	
}

