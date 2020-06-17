#ifndef STATISTICS_h
#define STATISTICS_h

#include <vector>
#include "Statistics.h"

#include <fstream>

class Simulator;

class InitialPhase //class is used to save x and y value for initial phase chart
{
public:
	double packet_number_;
	double average_retransmissions;

	InitialPhase(double x, double y) : packet_number_(x), average_retransmissions(y) {};

};

using namespace std;

class Statistics
{
public:
	Statistics(int transmitters_count, int time_of_simulation, int phase_time, Simulator* simulator);
	~Statistics();

	vector<int> lose_packets; //count of losses packet for each transmitters 
	vector<int> correct_sent_packets;//count of correct sent packets for each transmitters 
	vector<vector<int>> retransmissions_corrected_finished; //contain how many times, some packet was retransmitted
	vector<vector<int>> average_delay_packet; // opóŸnienia pakietów w czasie (opóŸnienia pakietów zapisywane osobno dla ka¿dego transmitera) // packet delays in time (packet delays recorded separately for each transmitter)
	vector<vector<int>> average_waiting_time; // oczekiwanie pakietów  na wys³anie (oczekiwanie pakietów zapisywane osobno dla ka¿dego transmitera) // waiting packets to send (waiting packets saved separately for each transmitter)
	vector<InitialPhase*> initial_phase_vector; //contain variables for initial phase chart
	
	

	vector<double>PER; // œrednia pakietowa stopa b³êdów  // average packet error rate
	int transmitters_count; //number of transmitters
	int time_of_simulation; // duration of the simulation
	int phase_time_; // start phase time 
	Simulator* simulator_; //indicator to Simulator 

	void Report();

	ofstream statistics_path;

	void add_to_initial_phase_vector();
	void add_lose_packets(int transmitter_number);
	void add_correct_sent_packets(int transmitter_number);
	void add_retransmissions_corrected_finished(int transmitter_number, int retransmissions);
	void add_average_delay_packet(int transmitter_number, int time);
	void add_average_waiting_time(int transmitter_number, int time);

};
#endif

