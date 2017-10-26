#include "descompresor.h"

bool decompressImage(const char * imagePath)
{
	std::vector<unsigned int> nVector;
	std::vector<unsigned char> bmpImage;

	std::string imageName(imagePath);
	imageName.resize(imageName.size() - 4); //remuevo el ".met" del nombre
	std::string extension = EXTENSION;
	std::string const pngFile(imageName + extension); //creo el archivo
	fs::ifstream bmpFile(imagePath);

	//extraer los primeros cuatro bytes del width de la imagen
	unsigned int width;
	unsigned char lengthAux[4];
	for (int j = 0; j < 4; j++)			//Tomo los 4 primeros bytes del archivo que representan
	{									//el largo de la imagen original en pixeles
		bmpFile >> lengthAux[j];
	}
	width = ((uint32_t)lengthAux[0]);
	nVector.resize((size_t)(width*width*4));

	recursiveDrawing(width, bmpFile, nVector, bmpImage);
	unsigned error = lodepng::encode(pngFile, bmpImage, width, width);
	if (error)
	{
		std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return true;
	}
	return false;
}


void recursiveDrawing(unsigned int width_, fs::ifstream& readFile, std::vector<unsigned int>& nVector, std::vector<unsigned char>& pngImage)
{
	static unsigned int level = 0;
	static unsigned int width = width_;
	static unsigned char currentValue = 0;
	readFile >> currentValue;
	unsigned char red, green, blue;

	if (((char)currentValue) == 'H')
	{
		level++;
		nVector.push_back(0);
	}
	else if (((char)currentValue) == 'N')
	{
		readFile >> red;
		readFile >> green;
		readFile >> blue;
		printRGBA(level, nVector[nVector.size() - 1], width, pngImage, red, green, blue, 255);
		nVector[nVector.size() - 1]++;
		if (nVector[nVector.size() - 1] == 4)
		{
			nVector.pop_back();
			nVector[nVector.size() - 1]++;
			level--;
		}
	}
	else //caso png de un solo color
	{
		readFile >> red;
		readFile >> green;
		readFile >> blue;
		printRGBA(level, 1, width, pngImage, red, green, blue, 255);
		currentValue = EOF;
	}

	if (currentValue != EOF)
	{
		recursiveDrawing(0, readFile, nVector, pngImage);
	}
}

void printRGBA(unsigned int level, unsigned int cuadrante, unsigned int width, std::vector<unsigned char>& pngImage, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
	unsigned int corrX, corrY, iInicial, jInicial, iFinal, jFinal, actualWidth;

	unsigned int n, t;				// 0 <= n <= 2^(level -1) - 1       idem t

	actualWidth = width / exp(level*log(2));			// Wtot / ( 2 ^ (level) )

	switch (cuadrante)
	{
	case 1:
		corrX = 0;
		corrY = 0;
		break;
	case 2:
		corrX = 1;
		corrY = 0;
		break;
	case 3:
		corrX = 0;
		corrY = 1;
		break;
	case 4:
		corrX = 1;
		corrY = 1;
		break;
	}

	iInicial = actualWidth*( corrX + 2*n);
	jInicial = actualWidth*( corrY + 2*t);

	iFinal = iInicial + actualWidth;
	jFinal = jInicial + actualWidth;

	for (unsigned int i = iInicial; i < iFinal; i++)
	{
		for (unsigned int j = jInicial; j < jFinal; j++)
		{
			pngImage[i + j*(width * 4)] = red;
			pngImage[i + j*(width * 4) + 1] = green;
			pngImage[i + j*(width * 4) + 2] = blue;
			pngImage[i + j*(width * 4) + 3] = alpha;
		}
	}
}