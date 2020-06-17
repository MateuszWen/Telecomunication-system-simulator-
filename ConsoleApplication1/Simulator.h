#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "Event.h"
#include "WirelessNetwork.h"
#include "ScanConnection.h"
#include <queue>
#include "Statistics.h"

class WirelessNetwork;
class ScanConnection;

class Simulator
{
public:

    void add_and_sort_events(Event* event);
    explicit Simulator(int mode, int text_mode, vector<double> seeds_vector, int number_of_simulation, double lambda, double probability, int K, int LR);
    void Start(size_t time_of_simulation, size_t phase_time);
	
    int mode_; //contain info about user want to use step mode or not - 0 step mode, 1 not step mode
    int text_mode_;//contain info about user want to follow a logger in console - 0 doesn't want, 1 want
    int number_of_simulation_; // actual number of simualtion

    vector<double> seeds_vector_; // vector contain seeds for all transmitters
    queue<ScanConnection*> scanConnectionQueue; // is use in conditional event in Simualtion->Start(...) method, contain all events connected to scanning connectivity in current time 
    list<Event*> eventList; // contain all events to execute 
	size_t last_packet_id = 0; // counter of original number of packets, each packet has original number
    size_t actual_time_ = 0; //actual time of simualtion
    size_t phase_time = 0; // contain start phase 
    Statistics* statistics = nullptr; // indicator to Statistics object - is use to collect all necessary simulation information 
    WirelessNetwork* wireless_network_ = nullptr; // indicator to WirelessNetwork object

    string values_uniform = ""; //storage all generated values of UniformGenerator->Rand(int min, int max) function 
    string values_exponental = ""; //storage all generated values of UniformGenerator->RandEXP(double lambda) function 

	//for wireless_network
    double lambda_; // parameter for WirelessNetwork - intensity of events  
    double probability_;// parameter for WirelessNetwork - succes of transmission probability
    int K_;// parameter for WirelessNetwork - count of trans-rec pairs
    int LR_;// parameter for WirelessNetwork - number of possible retransmissions
};

#endif // SIMULATOR_H

