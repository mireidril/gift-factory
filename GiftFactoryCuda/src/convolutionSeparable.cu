#include "../../Dependencies/include/CUDA/cuda.h"
//#include <CUDA/cuda.h>

#include "cuda_functions.h"

// Kernel that executes on the CUDA device
__global__ void cuda_render(int lol)
{}

void launch_bb()
{
	dim3 dimGrid(1000 / 256, 1);
	dim3 dimBlock(256);
	cuda_render <<< dimGrid, dimBlock >>> (5);

	std::cout<<"Coucou, chez moi j'ai CUDA !!!"<<std::endl;
}