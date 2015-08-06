

#include "Texture.h"

#include "Image.h"
Texture::Texture()
{
    m_internalFormat = GL_RGB;
    m_format = GL_BGR;
    m_type = GL_UNSIGNED_BYTE;
    m_target = GL_TEXTURE_2D;
    m_magFilter = GL_LINEAR;
    m_minFilter = GL_LINEAR;
    
}

ImageTexture::ImageTexture( size_t channelCount, GLenum channelType, size_t width, size_t height, size_t depth) : Texture()
{
    Image * emptyImage = new Image(nullptr, channelCount, channelType, width, height, depth);
    m_type = channelType;
    m_sourceImage = emptyImage;
    
    
}

ImageTexture::ImageTexture(Image * sourceImage) : Texture()
{
    m_sourceImage = sourceImage;
}

EmptyTexture::EmptyTexture( size_t channelCount, GLenum channelType, size_t width, size_t height, size_t depth) : Texture()
{
    m_type = channelType;
    setDimensions(glm::ivec4(width, height, depth, 1));
}


void Texture::generateObjectsForContext(GLuint context)
{
    m_objects[context] = GLuint(0);
    glGenTextures(1, &m_objects[context]);
    
}

void Texture::loadToContext(GLuint context)
{
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, m_objects[context]);
    
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, getDimensions().x, getDimensions().y, 0, m_format, GL_UNSIGNED_BYTE, getData());
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Texture::bindToContext(GLuint context)
{
    glBindTexture(GL_TEXTURE_2D, m_objects[context]);
}

GLuint Texture::getObjectForContext(GLuint context)
{
    return m_objects[context];
}

void Texture::setInternalFormat(GLenum internalFormat)
{
    m_internalFormat = internalFormat;
}

void Texture::setFormat(GLenum format)
{
    m_format = format;
}

void Texture::setType(GLenum type)
{
    m_type = type;
}
void Texture::setTarget(GLenum target)
{
    m_target = target;
}

void Texture::setMinFilter(GLenum minFilter)
{
    m_minFilter = minFilter;
}

void Texture::setMagFilter(GLenum magFilter)
{
    m_magFilter = magFilter;
}

Image * ImageTexture::getImage()
{
    return m_sourceImage;
}

void * EmptyTexture::getData()
{
    return nullptr;
}

void * ImageTexture::getData()
{
    return getImage()->getData();
}

glm::ivec4 ImageTexture::getDimensions()
{
    return getImage()->getImageDimensions();
}

glm::ivec4 EmptyTexture::getDimensions()
{
    return m_dimensions;
}

void ImageTexture::setDimensions(glm::ivec4 dimensions)
{
    m_sourceImage->setImageDimensions(dimensions);
}

void EmptyTexture::setDimensions(glm::ivec4 dimensions)
{
    m_dimensions = dimensions;
}


