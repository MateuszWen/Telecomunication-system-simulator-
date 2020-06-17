#include "UniformGeneratorKernel.h"

#include "Simulator.h"

//class is uses to generate start kernels for all simulations  

UniformGeneratorKernel::UniformGeneratorKernel(double kernel)
{
    this->kernel_ = kernel;
    this->directory_uniform;
    this->directory_exponential;
}

UniformGeneratorKernel::~UniformGeneratorKernel()
{
}
int UniformGeneratorKernel::RandKernels()
{
    double h = floor(kernel_ / Q);
    kernel_ = (A * (kernel_ - (Q * h))) - (R * h);
    if (kernel_ < 0)
        kernel_ = kernel_ + static_cast<int>(M);
    return kernel_;
}