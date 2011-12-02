#include "cuda_functions.h"
#include "Application.hpp"

int main(int argc, char **argv)
{
#ifdef _USING_CUDA_
	launch_bb();
#else
	std::cout<<"Coucou, chez moi j'ai PAS CUDA !!!"<<std::endl;
#endif

	Application giftFactory;
	giftFactory.init();
	giftFactory.update();
    
	return EXIT_SUCCESS;
}
