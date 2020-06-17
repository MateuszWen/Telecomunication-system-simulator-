
#include <iostream>
#include "Simulator.h"
#include "WirelessNetwork.h"
#include "UniformGenerator.h"
#include <array>
#include <chrono>
#include <ctime>

#include "UniformGeneratorKernel.h"

int main() 
{
	int mode = -1; //info about mode (step or continuous)
	int text_mode = -1; // display loges in console or not 
	int simulation_time; 
	int phase_time;
	int how_many_simulations = 1;
	int kernel_set = 0;

	int K = 10; // count of transmiter-receiver - default
	int LR = 15; //number of possible retransmissions - default


	//program mode
	do{
		cout << "Please type correct number.\n\nChoose program mode:\nStep mode: Type 0\nContinuous mode: Type 1\n";
		//mode = 1;
		cin >> mode;
		cout << "\n\n";
	}while(!(mode == 1 || mode == 0));
	//with logger or without? 
	do {
		cout << "Please type correct number.\n\nDo you want to follow each simulation execute?:\nWith text: Type 0\nWithout text: Type 1\n";
		//text_mode = 1;
		cin >> text_mode;
		cout << "\n\n";
	} while (!(text_mode == 1 || text_mode == 0));

	//count of pairs transmitter-receiver
	do {
		cout << "How many transmitter-receiver pairs do You want to use?: ";
		//K = 8;
		cin >> K;
		cout << "\n\n";
	} while (K <= 0);

	//count of possible retransmissions 
	do {
		cout << "How many possible retransmissions?: ";
		//LR = 15;
		cin >> LR;
		cout << "\n\n";
	} while (LR <= 0);

	//time of simulation
	cout << "Time of simulation (in milliseconds): ";
	cin >> simulation_time;
	cout << "\n\n";

	//initial phase
	do {
		cout << "Initial phase (in milliseconds): ";
		//phase_time = 31000;
		cin >> phase_time;
		cout << "\n\n";
	} while(phase_time >= simulation_time);

	//how many simulations do you want to do?
	cout << "How many simulations do You want to do? ";
	//how_many_simulations = 10;
	cin >> how_many_simulations;
	cout << "\n\n";
	
	//generate start kernels for all transmitters in all simulations 
	vector<vector<double>> seeds_vector(how_many_simulations);
	int multipler = 1;
	UniformGeneratorKernel* uniform_generator_kernel = new UniformGeneratorKernel(123);
	//we need (3*K) + 1 kernels. 3 for one transmitter. For example, 8 transmitters * 3 = 24 + 1 (for TER) = 25 kernels
	for(unsigned int i=1; i<=((3*K) + 1) * 100000 * how_many_simulations; i++)
	{
		double value = uniform_generator_kernel->RandKernels();
		if (i == (100000 * multipler))
		{
			seeds_vector.at(static_cast<int>((multipler - 1) / ((3 * K) + 1))).push_back(value);
			multipler++;
		}
	}

	
	vector<int>whih_kernel_set(how_many_simulations);// contain information which kernel set chose a user for a particular simulation 
	vector<double>whih_lambda(how_many_simulations); // which lambda will by used for a particular simulation 
	vector<double>whih_probability(how_many_simulations);// which probability will by used for a particular simulation 

	
	for(int i=0; i<how_many_simulations; i++)
	{
		cout << endl << endl << "SIMULATION NUMBER: ";
		cout << i + 1 << endl;
		
		//Which kernels set do You want to use?
		//do{
			cout << "Which kernels set do You want to use for simulation ";	cout << i + 1; cout << "? You can choose 1 - "; cout << static_cast<int>(how_many_simulations); cout << " kernels set: ";
			cin >> kernel_set;
			//kernel_set = i;
			whih_kernel_set.at(i) = kernel_set - 1;
			cout << "\n";
		//} while (kernel_set-1 < 0 || kernel_set-1 >= how_many_simulations );

		//Which lambda do You want to use?
		cout << "Which lambda do You want to use for simulation ";	cout << i+1; cout << ": ";
		double lambda ;
		cin >> lambda;
		//lambda =  0.0084;
		whih_lambda.at(i) = lambda;
		cout << "\n";

		//Which probability do You want to use?
		cout << "Which probability do You want to use for simulation ";	cout << i+1; cout << ": ";
		double probability;
		cin >> probability;
		whih_probability.at(i) = probability; //probability2.at(u);
		cout << "\n";
	}

	//start execute all declare simulations
		for (int i = 0; i < how_many_simulations; i++)
		{
			cout << endl << endl << "SIMULATION NUMBER: ";
			cout << i + 1 << endl << endl;


			auto start = chrono::system_clock::now();

			Simulator* sim = new Simulator(mode, text_mode, seeds_vector.at(whih_kernel_set.at(i)), i + 1, whih_lambda.at(i), whih_probability.at(i), K, LR);
			sim->Start(simulation_time * 2/*One pass loop symbolizes 0.5 ms in real time*/, phase_time * 2);

			auto end = chrono::system_clock::now();
			chrono::duration<double> elapsed_seconds = end - start;
			time_t end_time = std::chrono::system_clock::to_time_t(end);
			cout << endl << "Duration of the simulation: ";
			cout << elapsed_seconds.count();
			cout << "s" << endl;
		}
	}
	    	
//}

//All data as CITZ, CTP, CGP, TER itd. are in WirelessNetwork.h class
//
//WARNING!
// One pass loop symbolizes 0.5 ms in real time
// for example: 1 in program -> 0.5 ms
//				2 in program -> 1 ms
//			 1000 in program -> 500 ms
// and so on
//
