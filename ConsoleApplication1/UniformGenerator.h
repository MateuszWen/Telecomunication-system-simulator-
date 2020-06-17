#ifndef UNIFORMGENERATOR_H
#define UNIFORMGENERATOR_H

#include <string>

#include "Simulator.h"

class Simulator;
using namespace std;

class UniformGenerator
{
public:

    UniformGenerator(double seed, Simulator* simulation);
    ~UniformGenerator();

    //Rozk³ad jednostajny <0,1> // Uniform distribution <0.1>
    double Rand();
    //Rozk³ad jednostajny na zdefiniowanym przedziale <min,max> // Uniform distribution over the defined range <min, max>
    double Rand(int min, int max);
    //Rozk³ad wyk³adniczy // Exponential distribution
    double RandEXP(double lambda);
    //Rozk³ad Bernoulliego 0-1 // Bernoulli distribution 0-1
    double RandZeroOne(double probability);


    double get_kernel()
    {
        return kernel_;
    }

protected:

    string directory_uniform; 
    string directory_exponential;
	
    Simulator* simulation_;
    double kernel_;
    const double M = 2147483647.0;
    static const int A = 16807;
    static const int Q = 127773;
    static const int R = 2836;
};

#endif



