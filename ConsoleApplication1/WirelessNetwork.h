#ifndef WIRELESSNETWORK_H
#define WIRELESSNETWORK_H

#include "ReceivingAntenna.h"
#include "WirelessConnection.h"
#include <vector>
#include "TransmittingAntenna.h"
#include "Simulator.h"
#include "UniformGenerator.h"

class ReceivingAntenna;
class TransmittingAntenna;
class WirelessConnection;
class Simulator;
class UniformGenerator;

using namespace std;

//class contain all elements of a system
class WirelessNetwork
{
public:
	WirelessConnection* wireless_connection;
	vector<TransmittingAntenna*> transmitting_antennae;
	vector<ReceivingAntenna*> receiving_antennae;
	vector<Packet*> packetsACK_;
	void delete_first_packet_from_transmitter(int transmitter_number);
	Packet* get_front_packet_from_transmitter(int transmitter_number);
	Simulator* simulator_;

	int CTIZ = 2; //czas transmisji ACK // ACK transmission time
	int K; //liczba transmiterów i receiverów // number of transmitters and receivers
	int LR; //liczba mo¿liwych retransmisji pakietu // number of possible retransmissions of the packet
	int S = 1; //nas³uchiwanie kana³u // listening on the channel
	double lambda_; // intensywnoœæ L // intensity L
	double P_ ; // prawdopodobieñstwo sukcesu // probability of success


	vector<double>seeds_vector_; //contain start kernels for transmitters generators 
	UniformGenerator* generator_TER; // indicator to generator_TER
	
	WirelessNetwork(vector<double> seeds_vector, double lambda, double probability, Simulator* simulator, int K, int LR);
	~WirelessNetwork();
	
};

#endif

