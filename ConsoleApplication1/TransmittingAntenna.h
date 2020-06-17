#ifndef TRANSMITTINGANTENNA_H
#define TRANSMITTINGANTENNA_H

#include "Packet.h"
#include <queue>
#include "UniformGenerator.h"

class WirelessNetwork;
class Packet;
class UniformGenerator;
class Simulator;

class TransmittingAntenna
{
public:
	int transmiter_id; // number of transmitter 
	queue<Packet*> packets; // queue of packet to transmission 
	double kernel_R_; // start kernel for Generator of R 
	double kernel_CGP_;// start kernel for Generator of CGP 
	double kernel_CTP_;// start kernel for Generator of CTP
	Simulator* simulator_; //indicator to Simulator 
	UniformGenerator* generator_R = nullptr;//indicator to generator_R
	UniformGenerator* generator_CGP = nullptr;//indicator to generator_CGP
	UniformGenerator* generator_CTP = nullptr;//indicator to generator_CTP

	
	TransmittingAntenna(int transmiter_id, double kernel_R, double kernel_CGP, double kernel_CTP, Simulator* simulator);

	void add_Packet(Packet* packet, WirelessNetwork* wireless_network, Simulator* simulator);
};
#endif // TRANSMITTINGANTENNA_H



