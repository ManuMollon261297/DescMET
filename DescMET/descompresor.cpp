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
		lengthAux[j] = bmpFile.get();
	}
	unsigned int * tempPointer = (unsigned int *)lengthAux;
	width = *tempPointer;
	std::cout << "Image Width: " << width << std::endl;
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
	static unsigned int width = width_;
	static unsigned char currentValue = 0;
	unsigned char red, green, blue;
	readFile >> currentValue;
	std::cout << nVector.size() << std::endl;

	if (!readFile.eof())
	{
		if (((char)currentValue) == 'H')
		{
			nVector.push_back(0);
		}
		else if (((char)currentValue) == 'N')
		{
			readFile >> red;
			readFile >> green;
			readFile >> blue;
			nVector[nVector.size() - 1]++;
			printRGBA(nVector,width, pngImage, red, green, blue, 255);
			while((nVector.size() != 0)&&((nVector[nVector.size() - 1] == 4)))
			{
					nVector.pop_back();
					if (nVector.size() != 0)
					{
						nVector[nVector.size() - 1]++;
					}
			}
		}
		else //caso png de un solo color
		{
			readFile >> red;
			readFile >> green;
			readFile >> blue;
			nVector.push_back(0);
			nVector[0]++;
			for (int i = 0; i < 4; i++,nVector[0]++)
			{
				printRGBA(nVector, width, pngImage, red, green, blue, 255);
			}
		}
			recursiveDrawing(0, readFile, nVector, pngImage);
	}
}

void printRGBA(std::vector<unsigned int>& nVector,unsigned int width, std::vector<unsigned char>& pngImage, unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
	unsigned int corrX = 0;
	unsigned int corrY=0;
	unsigned int actualWidth = width / pow(2, nVector.size());	// Wtot / ( 2 ^ (level) )
	getCorr(corrX,corrY,nVector,1,width);

	unsigned int  iFinal, jFinal;
	iFinal = corrX  + (actualWidth)*4;
	jFinal = corrY + (actualWidth);

	for (unsigned int i = corrX; i < iFinal; i+=4)
	{
		for (unsigned int j = corrY; j < jFinal; j++)
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
	if (currentDepth <= nVector.size()) // caso base
	{
		unsigned int switchCase;
		if (nVector.size() == currentDepth)
		{
			switchCase = nVector[currentDepth - 1];
		}
		else
		{
			switchCase = nVector[currentDepth - 1] + 1;
		}
		switch (switchCase)
		{
		case 1:
			break;
		case 2:
			corrX += (width / pow(2, currentDepth))*4;
			break;
		case 3:
			corrY += width / pow(2, currentDepth);
			break;
		case 4:
			corrX += (width / pow(2, currentDepth))*4;
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