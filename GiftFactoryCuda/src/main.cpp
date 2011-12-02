#include "Application.hpp"

int main(int argc, char **argv)
{
#ifdef _USING_CUDA_
	std::cout<<"Utilisation de CUDA."<<std::endl;
#else
	std::cout<<"Non-utilisation de CUDA"<<std::endl;
#endif
	Application giftFactory;
	giftFactory.init();
	giftFactory.update();
    
	return EXIT_SUCCESS;
}
