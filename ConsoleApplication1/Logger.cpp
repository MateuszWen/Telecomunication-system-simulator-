#include "logger.h"

#include <iostream>
#include <fstream>
#include "Simulator.h"

using namespace std;

Logger::Logger(){}

Logger::~Logger(){}

void Logger::Info(std::string message, int text_mode, Simulator* simulator)
{
	//TODO: uwzglednienie logowania do pliku
	//TODO: Dodanie lepszego fromatowania tekstu np. sprintf,

	if (level_ != Level::Error)
	{
		if (text_mode == 0) { std::cout << "[Info] " << message << std::endl; }
	}

	/*string text = "[Info] ";
	ofstream foutput;
	ifstream finput;
	
	string file_name = "logger_SIMULATION_";
	file_name.append(to_string(simulator->number_of_simulation_)).append(".txt");
	
	finput.open(file_name);
	foutput.open(file_name, ios::app);

	if (finput.is_open())
		foutput << text.append(message.append("\n"));

	finput.close();
	foutput.close();

	file_name.clear();*/
}

void Logger::Debug(std::string message, int text_mode, Simulator* simulator)
{
	if (level_ != Level::Error)
	{
		if (text_mode == 0) { std::cout << "[Debug] " << message << std::endl; }
	}
	
	/*string text = "[Debug] ";
	ofstream foutput;
	ifstream finput;

	string file_name = "logger_SIMULATION_";
	file_name.append(to_string(simulator->number_of_simulation_)).append(".txt");
	
	finput.open(file_name);
	foutput.open(file_name, ios::app);

	if (finput.is_open())
		foutput << text.append(message.append("\n"));

	finput.close();
	foutput.close();

	file_name.clear();*/
}

void Logger::Error(std::string message, int text_mode, Simulator* simulator)
{
	
	if (text_mode == 0) { cout << "[Error] " << message << std::endl; }

	/*string text = "[Error] ";
	ofstream foutput;
	ifstream finput;

	string file_name = "logger_SIMULATION_";
	file_name.append(to_string(simulator->number_of_simulation_)).append(".txt");
	
	finput.open(file_name);
	foutput.open(file_name, ios::app);

	if (finput.is_open())
		foutput << text.append(message.append("\n"));

	finput.close();
	foutput.close();

	file_name.clear();*/
}

void Logger::save_in_file(string text, Simulator* simulator)
{
	/*try {
		ofstream foutput;
		ifstream finput;

		string file_name = "logger_SIMULATION_";
		file_name.append(to_string(simulator->number_of_simulation_)).append(".txt");
		
		finput.open(file_name);
		foutput.open(file_name, ios::app);

		if (finput.is_open())
			foutput << text.append("\n");

		finput.close();
		foutput.close();

		file_name.clear();
	}catch(...)
	{
		cout << "[Error] Problem with saving to file!\n";
	}*/

}

