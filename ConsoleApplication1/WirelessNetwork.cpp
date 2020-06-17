#include "WirelessNetwork.h"
#include <algorithm>
#include <iostream>
#include "Simulator.h"

#include "TransmittingAntenna.h"

using namespace std;

class Simulator;

WirelessNetwork::WirelessNetwork(vector<double> seeds_vector, double lambda, double probability, Simulator* simulator, int K, int LR)
{
	this->simulator_ = simulator;
	this->seeds_vector_ = seeds_vector;
	this->P_ = probability;
	this->lambda_ = lambda;
	this->wireless_connection = new WirelessConnection(true);
	this->K = K;
	this->LR = LR;
	this->generator_TER = new UniformGenerator(seeds_vector_.back(), simulator); //wireless_network contain generator for TER. TER receive the last one kernel from seeds_vector_
		for (int i = 0; i < this->K; i++)
		{
			TransmittingAntenna* ta = new TransmittingAntenna(i, seeds_vector_.at(i),seeds_vector_.at(i+(K)), seeds_vector_.at(i+(2*K)), simulator_); //each generator receive other kernel 
			transmitting_antennae.push_back(ta);
			ReceivingAntenna* ra = new ReceivingAntenna(i);
			receiving_antennae.push_back(ra);
		}
}

WirelessNetwork::~WirelessNetwork()
{
	
}

void WirelessNetwork::delete_first_packet_from_transmitter(int transmitter_number)
{
	this->transmitting_antennae.at(transmitter_number)->packets.pop();
}

Packet* WirelessNetwork::get_front_packet_from_transmitter(int transmitter_number)
{
	return this->transmitting_antennae.at(transmitter_number)->packets.front();	
}




















































