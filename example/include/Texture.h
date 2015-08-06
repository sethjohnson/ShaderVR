#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>

class Image;

class Texture
{
public:
    Texture();
    
    void loadToContext(GLuint context);
    void generateObjectsForContext(GLuint context);
    void bindToContext(GLuint context);
    GLuint getObjectForContext(GLuint context);
    void setInternalFormat(GLenum internalFormat);
    void setFormat(GLenum format);
    void setType(GLenum type);
    void setTarget(GLenum target);
    void setMinFilter(GLenum minFilter);
    void setMagFilter(GLenum magFilter);
    virtual void * getData() = 0;
    virtual glm::ivec4 getDimensions() = 0;
    virtual void setDimensions(glm::ivec4 dimensions) = 0;
protected:
    std::map<GLuint, GLuint> m_objects;
    GLenum m_internalFormat;
    GLenum m_format;
    GLenum m_type;
    GLenum m_target;
    GLenum m_magFilter;
    GLenum m_minFilter;
};
class EmptyTexture : public Texture
{
public:
    EmptyTexture();
    EmptyTexture( size_t channelCount, GLenum channelType, size_t width, size_t height, size_t depth);
    EmptyTexture(size_t width, size_t height, size_t depth);
    
    virtual void * getData();
    virtual glm::ivec4 getDimensions();
    virtual void setDimensions(glm::ivec4 dimensions);

protected:
    glm::ivec4 m_dimensions;

};


class ImageTexture : public Texture
{
public:
    ImageTexture();
    ImageTexture( size_t channelCount, GLenum channelType, size_t width, size_t height, size_t depth);
    ImageTexture(size_t width, size_t height, size_t depth);
    ImageTexture(Image * sourceImage);

    virtual void * getData();
    virtual glm::ivec4 getDimensions();
    virtual void setDimensions(glm::ivec4 dimensions);

protected:
    Image * getImage();

    Image * m_sourceImage;
    
};


#endif