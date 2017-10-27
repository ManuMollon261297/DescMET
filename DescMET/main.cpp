#include "descompresor.h"

#define IMAGE_PATH "Raiden.met"

int main()
{

	decompressImage(IMAGE_PATH);
	std::cout << "Finished" << std::endl;
	getchar();
	return 0;
}
