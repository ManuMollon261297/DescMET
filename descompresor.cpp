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
	recursiveDrawing(width, bmpFile, nVector, bmpImage);
	unsigned error = lodepng::encode(pngFile, bmpImage, width, width);
	if (error)
	{
		std::cout << "Decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		return true;
	}
	return false;
}


void recursiveDrawing(unsigned int width_,fs::ifstream& readFile, std::vector<unsigned int>& nVector, std::vector<unsigned char>& pngImage)
{
	static unsigned int level = 0;
	static unsigned int width = width_;
	static int currentValue = 0;
	readFile >> currentValue;
	unsigned char red, green, blue;

	if (((char)currentValue) == 'H')
	{
		level++;
		nVector.push_back(0);
	}
	else if(((char)currentValue) == 'N')
	{
		readFile >> red;
		readFile >> green;
		readFile >> blue;
		printRGBA(level,nVector[nVector.size() - 1],width,pngImage,red,green,blue,255);
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
		printRGBA(level,1,width, pngImage,red,green,blue,255);
		currentValue = EOF;
	}

	if (currentValue != EOF)
	{
		recursiveDrawing(0,readFile,nVector,pngImage);
	}
}


void printRGBA(unsigned int level, unsigned int cuadrante, unsigned int width, std::vector<unsigned char>& pngImage, unsigned int red, unsigned int green, unsigned int blue,unsigned int alpha)
{

}