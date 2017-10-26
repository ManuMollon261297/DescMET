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

	//nVector.resize((size_t)(width*width*4));
	bmpImage.resize((size_t)(width*width * 4));

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
		printRGBA(nVector,width, pngImage, red, green, blue, 255);
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
		printRGBA(nVector,width, pngImage, red, green, blue, 255);
	}

	if (!readFile.eof())
	{
		recursiveDrawing(0, readFile, nVector, pngImage);
	}
}

void printRGBA(std::vector<unsigned int>& nVector,unsigned int width, std::vector<unsigned char>& pngImage, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
	unsigned int corrX, corrY, iInicial, jInicial, iFinal, jFinal, actualWidth;

	unsigned int n = 0;
	unsigned int t = 0;				// 0 <= n <= 2^(level -1) - 1       idem t

	actualWidth = width / pow(2, nVector.size());	// Wtot / ( 2 ^ (level) )
	switch (nVector[nVector.size() - 1]);
	getCorr(corrX,corrY,nVector,1,width);

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

bool getCorr(unsigned int& corrX,unsigned int& corrY, std::vector<unsigned int>& nVector, unsigned int currentDepth, unsigned int width)
{
	if (currentDepth == nVector.size()) // caso base
	{
		switch (nVector[currentDepth])
		{
		case 1:
			break;
		case 2:
			corrX += width / pow(2, currentDepth);
			break;
		case 3:
			corrY += width / pow(2, currentDepth);
			break;
		case 4:
			corrX += width / pow(2, currentDepth);
			corrY += width / pow(2, currentDepth);
			break;
		}
		currentDepth++;
		getCorr(corrX, corrY, nVector, currentDepth, width);
		return true;
	}
	else
	{
		return false;
	}
}