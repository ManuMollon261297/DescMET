#include "descompresor.h"

#define IMAGE_PATH "Copyright.met"

int main()
{

	decompressImage(IMAGE_PATH);
	std::cout << "Finished" << std::endl;
	getchar();
	return 0;
}
