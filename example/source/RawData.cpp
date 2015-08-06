#include "RawData.h"
#include <algorithm>    // std::copy

size_t RawData::guessElementSize(GLenum elementType)
{
    size_t guess = 0;
    switch (elementType) {
        case GL_BYTE:
            guess = sizeof(GLbyte);
            break;
        case GL_UNSIGNED_BYTE:
            guess = sizeof(GLubyte);
            break;
        case GL_FLOAT:
            guess = sizeof(GLfloat);
            break;
        case GL_INT:
            guess = sizeof(GLint);
            break;
        case GL_UNSIGNED_INT:
            guess = sizeof(GLuint);
            break;
        default:
            
            break;
    }
    return  guess;
}
RawData::RawData() : m_data(nullptr)
{
    
}

RawData::RawData(const void * data, size_t elementSize, size_t elementCount, size_t elementDimension, GLenum elementType) : RawData() {

    m_elementSize = (elementSize);
    m_elementCount = (elementCount);
    m_elementDimension = (elementDimension);
    m_elementType = (elementType);
    
    setData(data);
}

size_t RawData::getElementSize()
{
    return m_elementSize;
}

size_t RawData::getElementCount()
{
    return m_elementCount;
}

size_t RawData::getElementDimension()
{
    return m_elementDimension;
}

void * RawData::getData()
{
    return m_data;
}

GLenum RawData::getElementType()
{
    return m_elementType;
}

RawData::~RawData()
{
    delete [] m_data;
}

void RawData::setData(const void* value)
{
    if (m_data == nullptr) {
        m_data = new char[m_elementSize*m_elementCount];
        
    }
    if (value != nullptr) {
        std::copy((char*)value, (char*)value+m_elementSize*m_elementCount, m_data);
    }
    
    
}

void RawData::setElementCount(size_t count)
{
    m_elementCount = count;
    
    if (m_data != nullptr) {
        char* temp = m_data;
        
        m_data = new char[m_elementSize*m_elementCount];
        std::copy((char*)temp, (char*)temp+m_elementSize*m_elementCount, m_data);
        delete [] temp;
    }
}