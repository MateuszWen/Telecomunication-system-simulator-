#include "ReceivePacket.h"

ReceivePacket::ReceivePacket(int execute_time_, Simulator* simulator, Packet* packet, WirelessNetwork* wireless_network, bool was_collision)
	: Event(execute_time_, 4),
	simulator_(simulator),
	packet_(packet),
	wireless_network_(wireless_network),
	was_collision_(was_collision)
{
}

void ReceivePacket::Execute()
{
	Logger logger;

	string text = "Packets was inputed to ReceivingAntenna number ";
	logger.Info(text.append(to_string(packet_->trans_rec_number)).append(" ID packet number: ").append(to_string(packet_->packed_id)).append(". Simulation time: ").append(to_string(static_cast<double>(simulator_->actual_time_)/2)), simulator_->text_mode_, simulator_);

	int receiver_number = packet_->trans_rec_number;

	wireless_network_->receiving_antennae.at(receiver_number)->packet_ = packet_; //input received Packet to correctly ReceivingAntenna
	wireless_network_->receiving_antennae.at(receiver_number)->was_collision_ = was_collision_;//input int receiving antenna information about the packet participate in collision

	
}

