#include "descompresor.h"

#define IMAGE_PATH "128.met"

int main()
{
	decompressImage(IMAGE_PATH);
	std::cout << "Finished" << std::endl;
	getchar();
	return 0;
}
