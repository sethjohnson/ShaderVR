//
//  GeometryArray.h
//  ExampleVR
//
//  Created by Seth Johnson on 7/29/15.
//
//

#ifndef __ExampleVR__GeometryArray__
#define __ExampleVR__GeometryArray__

#include <map>
#include <vector>
#include "GL/glew.h"
#include "AttributeArray.h"
#include <utility>

typedef enum  {
    ATTRIB_INDEX = -1,
    ATTRIB_COLOR = 0,
    ATTRIB_POSITION = 1,
    ATTRIB_NORMAL = 2,
    ATTRIB_TEXCOORD = 3,
    ATTRIB_TANGENT = 4,
    ATTRIB_MATERIAL = 5,
    
} AttribType;


class GeometryArray {
    std::map<AttribType, AttributeArray*> m_attributes;
    std::map<std::string, AttributeArray*> m_indices;
    std::map<std::string, std::pair<size_t, size_t>> m_indexBounds;
    
    std::map<GLuint, GLuint> m_VBO;
    std::map<GLuint, GLuint> m_VAO;
    std::map<GLuint, GLuint> m_IBO;
    

public:
    template<class T> void addAttribute(std::vector<T>&, AttribType, size_t, GLenum);
    void addIndex(std::vector<GLuint> &, std::string indexName);
    void listContents();
    void loadAttributesToObject(GLuint vao, GLuint vbo);
    void loadIndicesToObject(GLuint vao, GLuint ibo);
    size_t getStartForIndex(std::string indexName);
    size_t getCountForIndex(std::string indexName);
    void loadAttributesToContext(GLuint context);
    void loadIndicesToContext(GLuint context);

    void generateObjectsForContext(GLuint context);
    
    void bindToContext(GLuint context);
    

};


template<class T> void GeometryArray::addAttribute(std::vector<T> & array, AttribType attribType, size_t dimension, GLenum GLType)
{
    T* dataCopy = new T[array.size()];
    std::copy ( array.data(), array.data()+array.size(), dataCopy );
    m_attributes[attribType] = new AttributeArray((void*)(dataCopy), (size_t)sizeof(T), (size_t)array.size(), dimension, GLType);
    
}

#endif /* defined(__ExampleVR__GeometryArray__) */
