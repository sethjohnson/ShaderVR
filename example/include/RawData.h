#ifndef RAWDATA_H
#define RAWDATA_H

#include <GL/glew.h>

class RawData {
protected:
    char * m_data;
    size_t m_elementSize;
    size_t m_elementCount;
    size_t m_elementDimension;
    GLenum m_elementType;
    static size_t guessElementSize(GLenum elementType);
public:
    RawData();
    RawData(const void * data, size_t elementSize, size_t elementCount, size_t elementDimension, GLenum elementType);
    
    ~RawData();
    virtual void setData(const void* value);
    virtual void setElementCount(size_t count);
    virtual size_t getElementSize();
    virtual size_t getElementCount();
    virtual size_t getElementDimension();
    virtual GLenum getElementType();
    virtual void * getData();

};
#endif