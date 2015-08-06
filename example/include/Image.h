#ifndef IMAGE_H
#define IMAGE_H

#include "RawData.h"
#include <glm/glm.hpp>
#include <string>

class Image : public RawData {
    glm::ivec4 m_imageDimensions;
    
    
public:
    Image(const void* data, size_t channelCount, GLenum channelType, size_t width, size_t height, size_t depth);
    
    glm::ivec4 getImageDimensions();
    void setImageDimensions(glm::ivec4 dimensions);
    static Image *LoadFromBMP(std::string fileName);
};


#endif