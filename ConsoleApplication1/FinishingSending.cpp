#include "FinishingSending.h"
#include "ScanConnection.h"

FinishingSending::FinishingSending(int execute_time_, Packet* packet, WirelessNetwork* wireless_network,
	Simulator* simulator)
	: Event(execute_time_, 2),
	packet(packet),
	wireless_network_(wireless_network),
	simulator_(simulator)
{
}

void FinishingSending::Execute()
{
	int transmitter_number = packet->trans_rec_number; //number of transmitter actual processed packet
	
	Logger logger;
	string text = "End of sending packet from transmitter ";
	logger.Info(text.append(to_string(transmitter_number)).append(". Simulation time: ").append(to_string(static_cast<double>(simulator_->actual_time_)/2)),simulator_->text_mode_, simulator_);

	
	//data to statistics
	if (simulator_->actual_time_ >= simulator_->phase_time) 
	{
		simulator_->statistics->add_average_delay_packet(transmitter_number, (simulator_->actual_time_ - packet->start_life_time));
	}
	simulator_->statistics->add_to_initial_phase_vector();

	
	//checking which time is when 70 packet was arrived to receiver correctly
	//double sum_of_received_packages = 0.0;
	//for (int v : simulator_->statistics->correct_sent_packets) { sum_of_received_packages = v + sum_of_received_packages; }

	//delete received packet from transmitter 
	wireless_network_->delete_first_packet_from_transmitter(transmitter_number);

	//if in transmitter is some packet, schedule his ScanConnection event 
		if(!(wireless_network_->transmitting_antennae.at(packet->trans_rec_number)->packets.empty()))
		{
			ScanConnection* sc = new ScanConnection(wireless_network_->get_front_packet_from_transmitter(transmitter_number), simulator_->actual_time_ + wireless_network_->S, 
				0, transmitter_number, wireless_network_, simulator_);
			simulator_->add_and_sort_events(sc);
		}
	
	wireless_network_->wireless_connection->set_acces_possible(true);
}
