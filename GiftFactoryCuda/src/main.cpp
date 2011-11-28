#include "cuda_functions.h"

int main(int argc, char **argv)
{
#ifdef _USING_CUDA_
	launch_bb();
#else
	std::cout<<"Coucou, chez moi j'ai PAS CUDA !!!"<<std::endl;
#endif
	system("PAUSE");
	return 0;
}
