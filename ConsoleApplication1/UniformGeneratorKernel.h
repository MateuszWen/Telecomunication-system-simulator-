
#ifndef UNIFORMGENERATORKERNEL_H
#define UNIFORMGENERATORKERNEL_H

#include <string>
#include <cmath>
#include <vector>

#include "Simulator.h"

using namespace std;

class UniformGeneratorKernel
{
public:

    UniformGeneratorKernel(double seed);
    ~UniformGeneratorKernel();

    //Kernel generator 
    int RandKernels();

    double get_kernel()
    {
        return kernel_;
    }

protected:

    string directory_uniform = "kernel_uniform.txt";
    string directory_exponential = "kernel_exponential.txt";

    double kernel_;
    const double M = 2147483647.0;
    static const int A = 16807;
    static const int Q = 127773;
    static const int R = 2836;
};

#endif
