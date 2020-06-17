#include "UniformGenerator.h"

#include <ostream>
#include <fstream>
#include "Simulator.h"




UniformGenerator::UniformGenerator(double kernel, Simulator* simulator)
{
    this->kernel_ = kernel;
    this->directory_uniform;//.append(to_string(transmitter_number).append("_RandMinMax().txt"));
    this->directory_exponential;// .append(to_string(transmitter_number).append("_RandEXP().txt"));
    this->simulation_ = simulator;
}

UniformGenerator::~UniformGenerator()
{

}

double UniformGenerator::Rand()
{
    double h = floor(kernel_ / Q);
    kernel_ = (A * (kernel_ - (Q * h))) - (R * h);
    if (kernel_ < 0)
        kernel_ = kernel_ + static_cast<int>(M);
    return kernel_ / M;
}
double UniformGenerator::Rand(int min, int max)
{
    double result = (Rand() * (max - min)) + min;

    simulation_->values_uniform.append(to_string(result)).append("\n");

    return result;
}
//Algorytm dla generatora rozk³adu wyk³adniczego
// Algorithm for the exponential distribution generator
double UniformGenerator::RandEXP(double lambda)
{
    auto k = Rand();

    double result = -(1.0 / lambda) * log(k);
	
    simulation_->values_exponental.append(to_string(result)).append("\n");
         
    return -(1.0 / lambda) * log(k);
}
//Algorytm dla generatora rozk³adu zero-jedynkowego
// Algorithm for the zero-one distribution generator
double UniformGenerator::RandZeroOne(double probability)
{
    auto k = Rand();
    if (k < probability)
        return 1;
    else
        return 0;
}