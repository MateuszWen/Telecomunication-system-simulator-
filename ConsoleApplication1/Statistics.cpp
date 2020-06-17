#include "Statistics.h"

#include <iostream>
#include <numeric>
#include "Simulator.h"

using namespace std;


Statistics::Statistics(int transmitters_count, int time_of_simulation, int phase_time, Simulator* simulator)
{
	this->simulator_ = simulator;
	this->time_of_simulation = time_of_simulation/2; //divide by 2 because 1 loop = 0.5 ms <- time is saved in ms
	this->phase_time_ = phase_time/2;
	this->transmitters_count = transmitters_count;
	for (int i = 0; i < transmitters_count; i++)
	{
		PER.push_back(0);
		lose_packets.push_back(0);
		correct_sent_packets.push_back(0);
		//retransmissions_corrected_finished.at(i).push_back(0);
		retransmissions_corrected_finished.push_back(vector<int>());
		average_delay_packet.push_back(vector<int>());
		average_waiting_time.push_back(vector<int>());
		
	}
}

Statistics::~Statistics() {}

void Statistics::add_to_initial_phase_vector() 
{
	//received packets
	double sum_of_received_packages = 0.0;
	for (int v : correct_sent_packets) { sum_of_received_packages = v + sum_of_received_packages; }

	//average packet retransmissions 
	int count_of_retransmissions_in_whole_system = 0;
	for (int i = 0; i < retransmissions_corrected_finished.size(); i++)
	{
		count_of_retransmissions_in_whole_system = accumulate(retransmissions_corrected_finished.at(i).begin(), retransmissions_corrected_finished.at(i).end(), 0) + count_of_retransmissions_in_whole_system;
	}
		
	InitialPhase* ip = new InitialPhase(sum_of_received_packages, count_of_retransmissions_in_whole_system/sum_of_received_packages);
	initial_phase_vector.push_back(ip);
}


void Statistics::add_lose_packets(int transmitter_number)
{
	lose_packets.at(transmitter_number)++;
}

void Statistics::add_correct_sent_packets(int transmitter_number)
{
	correct_sent_packets.at(transmitter_number)++;
}

void Statistics::add_retransmissions_corrected_finished(int transmitter_number, int retransmissions)
{
	retransmissions_corrected_finished.at(transmitter_number).push_back(retransmissions);
}
void Statistics::add_average_delay_packet(int transmitter_number, int time)
{
	average_delay_packet.at(transmitter_number).push_back(time);
}
void Statistics::add_average_waiting_time(int transmitter_number, int time)
{
	average_waiting_time.at(transmitter_number).push_back(time);
}
void Statistics::Report()
{
	string file_name = "Final_Statistics_SIMULATION_";
	file_name.append(to_string(simulator_->number_of_simulation_)).append(".txt");
	
	statistics_path.open(file_name);
	statistics_path << "Simulation statistics:" << endl << endl;
	cout << endl << "Simulation statistics:" << endl << endl;

	string lambda = "L  = ";
	lambda.append(to_string(simulator_->wireless_network_->lambda_));
	statistics_path << lambda << endl << endl;
	cout << lambda << endl << endl;
	lambda.clear();

	string probability = "P  = ";
	probability.append(to_string(simulator_->wireless_network_->P_));
	statistics_path << probability << endl << endl;
	cout << probability << endl << endl;
	probability.clear();
	
	string retransmissions_count = "LR = ";
	retransmissions_count.append(to_string(simulator_->wireless_network_->LR));
	statistics_path << retransmissions_count << endl << endl;
	cout << retransmissions_count << endl << endl;
	retransmissions_count.clear();

	string trans_rec_count = "K  = ";
	trans_rec_count.append(to_string(simulator_->wireless_network_->K));
	statistics_path << trans_rec_count << endl << endl;
	cout << trans_rec_count << endl << endl;
	trans_rec_count.clear();

	string ctiz = "CTIZ  = ";
	ctiz.append(to_string(static_cast<double>(simulator_->wireless_network_->CTIZ)/2)).append(" [ms]");
	statistics_path << ctiz << endl << endl;
	cout << ctiz << endl << endl;
	ctiz.clear();

	string s = "S  = ";
	s.append(to_string(static_cast<double>(simulator_->wireless_network_->S)/2)).append(" [ms]");
	statistics_path << s << endl << endl;
	cout << s << endl << endl;
	s.clear();
	

	for (int i = 0; i < transmitters_count; i++)
	{

		statistics_path << endl << "Transmitter number: " << i << ":" << endl;
		statistics_path << "Lost packages - " << lose_packets[i] << endl;
		statistics_path << "Received packages - " << correct_sent_packets[i] << endl;

		cout << endl << "Transmitter number: " << i << ":" << endl;
		cout << "Lost packages - " << lose_packets[i] << endl;
		cout << "Received packages - " << correct_sent_packets[i] << endl;

		//œrednia pakietowa stopa b³êdu dla ka¿dego tranmssitera z osobna
		// average packet error rate for each tranmssiter separately
		if (correct_sent_packets[i] != 0)
		{
			PER.at(i) = lose_packets[i] / (double)correct_sent_packets[i];
			statistics_path << "PER = " << PER.at(i) << endl << endl;
			cout << "PER = " << PER.at(i) << endl << endl;
		}
	}

	//otrzymane pakiety w ca³ym systemie
	//received packets from all transmitters
	cout << endl;
	double sum_of_received_packages = 0.0;
	for (int v : correct_sent_packets) { sum_of_received_packages = v+sum_of_received_packages; }

	//utracone pakiety w ca³ym systemie
	//lost packets from all transmitters
	double sum_of_lost_packages = 0.0;
	for (int v : lose_packets) { sum_of_lost_packages = v + sum_of_lost_packages; }

	statistics_path << "Received packages in all system:" << sum_of_received_packages << endl;
	cout << "Received packages in all system:" << sum_of_received_packages << endl;

	statistics_path << "Lost packages in all system:" << sum_of_lost_packages << endl;
	cout << "Lost packages in all system:" << sum_of_lost_packages << endl;

	//œrednia pakietowa stopa b³êdu dla wszystkich transmietów razem
	//average packet error rate for all transmiets together
	cout << endl;
	double sum_error_rate = 0.0;
	for (double v : PER) { sum_error_rate = v + sum_error_rate; }
	statistics_path << "AVERAGE PER = " << sum_error_rate / transmitters_count << endl;
	cout << "AVERAGE PER = " << sum_error_rate / transmitters_count << endl;

	//maksymalna pakietowa stopa b³êdów
	//maximum packet error rate
	cout << endl;
	double max_error_rate = 0.0;
	for(double v : PER)
	{
		if (v > max_error_rate)
			max_error_rate = v;
	}
	statistics_path << "MAX PER = " << max_error_rate << endl;
	cout << "MAX PER = " << max_error_rate << endl;

	//œrednia liczba retransmisji pakietów
	//average number of packet retransmissions
	cout << endl;
	int count_of_retransmissions_in_whole_system = 0;
	for(int i=0; i<retransmissions_corrected_finished.size(); i++)
	{		
		count_of_retransmissions_in_whole_system = accumulate(retransmissions_corrected_finished.at(i).begin(), retransmissions_corrected_finished.at(i).end(), 0) + count_of_retransmissions_in_whole_system;
	}
	statistics_path << "Average number of packet retransmissions = " << count_of_retransmissions_in_whole_system / sum_of_received_packages << endl;
	cout << "Average number of packet retransmissionse = " << count_of_retransmissions_in_whole_system / sum_of_received_packages << endl;

	//przep³ywnoœæ systemu mierzon¹ liczb¹ poprawnie odebranych pakietów w jednostce czasu (suma pakietów odebranych / czas symulacji)
	//system rate measured by the function of full packet download per unit of time (sum of received packets / simulation time)
	double system_bit_rate = sum_of_received_packages / ((time_of_simulation - phase_time_)/1000); //divide by 1000, because time_of_simulation is in ms, but we want to have a result in seconds
	statistics_path << "System Bit Rate = " << system_bit_rate  << " packages per second" << endl;
	cout << "System Bit Rate = " << system_bit_rate  << " packages per second" << endl;


	//œredie opóŸnienie pakietów
	//average packet delay
	cout << endl;
	float sum_of_delay_packets = 0;
	int cout_of_packets_delays = 0;
	for(int i = 0; i<average_delay_packet.size(); i++)
	{
		sum_of_delay_packets = accumulate(average_delay_packet.at(i).begin(), average_delay_packet.at(i).end(), 0) + sum_of_delay_packets;
		cout_of_packets_delays = average_delay_packet.at(i).size();
	}
	statistics_path << "Average packet delay = " << sum_of_delay_packets/cout_of_packets_delays << endl;
	cout << "Average packet delay = " << sum_of_delay_packets / cout_of_packets_delays << endl;

	//œredie czas oczekiwania pakietu
	// average packet wait time
	cout << endl;
	double sum_of_waiting_packets = 0;
	int cout_of_packets_waitings = 0;
	for (int i = 0; i < average_waiting_time.size(); i++)
	{
		sum_of_waiting_packets = accumulate(average_waiting_time.at(i).begin(), average_waiting_time.at(i).end(), 0) + sum_of_waiting_packets;
		cout_of_packets_waitings = average_waiting_time.at(i).size();
	}
	statistics_path << "Average packet waiting = " << sum_of_waiting_packets / cout_of_packets_waitings << endl;
	cout << "Average packet waiting = " << sum_of_waiting_packets / cout_of_packets_waitings << endl;


	statistics_path.close();


	//saving initial_phase_vector to .txt
	string initial_phase_values_file_name = "initial_phase_values_SIMULATION_";
	initial_phase_values_file_name.append(to_string(simulator_->number_of_simulation_)).append(".txt");

		std::ofstream file(initial_phase_values_file_name);

		ofstream foutput3;
		ifstream finput3;
		finput3.open(initial_phase_values_file_name);
		foutput3.open(initial_phase_values_file_name, ios::app);

		if (finput3.is_open())
		{
			for (InitialPhase* ip : initial_phase_vector)
			{
				foutput3 << ip->packet_number_;
				foutput3 << "|";
				foutput3 << ip->average_retransmissions << endl;
			}
		}
		finput3.close();
		foutput3.close();


	// save Average PER to file
		/*string directory_average_PER = "average_retransmittions_P_";
		directory_average_PER.append(to_string(simulator_->wireless_network_->P_)).append(".txt");

		if (ifstream(directory_average_PER))
		{
			ofstream foutput3;
			ifstream finput3;
			finput3.open(directory_average_PER);
			foutput3.open(directory_average_PER, ios::app);

			if (finput3.is_open())
			{
				foutput3 << count_of_retransmissions_in_whole_system / sum_of_received_packages << endl;
			}

			finput3.close();
			foutput3.close();
		}
		else
		{
			std::ofstream file(directory_average_PER);

			ofstream foutput3;
			ifstream finput3;
			finput3.open(directory_average_PER);
			foutput3.open(directory_average_PER, ios::app);

			if (finput3.is_open())
			{
				foutput3 << count_of_retransmissions_in_whole_system / sum_of_received_packages << endl;
			}

			finput3.close();
			foutput3.close();
		}*/
	
}


