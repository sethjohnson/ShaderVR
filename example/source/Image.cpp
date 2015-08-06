
#include "Image.h"
#include <iostream>
Image::Image(const void* data, size_t channelCount, GLenum channelType, size_t width, size_t height, size_t depth) : RawData()
{
    m_imageDimensions = glm::ivec4(width, height, depth, 1);
    m_elementType = channelType;
    m_elementDimension = channelCount;
    m_elementSize = guessElementSize(channelType)*m_elementDimension;
    setElementCount(m_imageDimensions.x*m_imageDimensions.y*m_imageDimensions.z*m_imageDimensions.w);
    setData(data);
    
}

Image * Image::LoadFromBMP(std::string fileName)
{
    Image * result = nullptr;
    FILE * file = std::fopen(fileName.c_str(),"rb");
    if (!file)
    {
        printf("Image could not be opened\n");
        return result;
    }
    
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int width, height;
    unsigned int imageSize;   // = width*height*3
    // Actual RGB data
    unsigned char * data;
    
    if ( std::fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        printf("Not a correct BMP file\n");
        return result;
    }
    if ( header[0]!='B' || header[1]!='M' ){
        printf("Not a correct BMP file\n");
        return result;
    }
    
    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);
    
    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)
        imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
    if (dataPos==0)
        dataPos=54; // The BMP header is done that way
    
    // Create a buffer
    data = new unsigned char [imageSize];
    
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    
    //Everything is in memory now, the file can be closed
    fclose(file);
    
  


    result = new Image(data, 3, GL_UNSIGNED_BYTE, width, height, 1);
    delete [] data;    
	std::cout << result->getImageDimensions().x << " x " << result->getImageDimensions().y << std::endl;
//    int s = 0;
//    int x = result->getImageDimensions().x;
//    int y = result->getImageDimensions().y;
//    int c = 3;
//    data = (unsigned char*)result->getData();
//    for (int i = 0; i < y; i++) {
//        for (int j = 0; j < x; j++) {
//            printf("%5i: ", s);
//            for (int k = 0; k < c; k++) {
//                std::cout << (int)data[i*x*c+ j*c + k] << ", ";
//            }
//            printf("\n"); s++;
//        }
//    }

    return result;
}

glm::ivec4 Image::getImageDimensions()
{
    return m_imageDimensions;
}

void Image::setImageDimensions(glm::ivec4 dimensions)
{
    m_imageDimensions = dimensions;
}
