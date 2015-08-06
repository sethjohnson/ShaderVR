//
//  GeometryArray.cpp
//  ExampleVR
//
//  Created by Seth Johnson on 7/29/15.
//
//

#include "GeometryArray.h"
#include <iostream>


using std::vector;
using std::map;



void GeometryArray::listContents()
{
    for(auto const &e : m_attributes) {
        // ent1.first is the first key
        std::cout << "Attribute " <<e.first << std::endl;
    }
}


void GeometryArray::loadAttributesToObject(GLuint vao, GLuint vbo)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER,  vbo);
    
    size_t byte_size = 0;
    size_t total_bytes = 0;
    
    for(auto const &e : m_attributes) {
        byte_size = e.second->getElementSize()*e.second->getElementCount();
        total_bytes += byte_size;
        
    }

    glBufferData(GL_ARRAY_BUFFER,
                 total_bytes,
                 NULL, GL_STATIC_DRAW);
    
    byte_size = 0;
    total_bytes = 0;
    for(auto const &e : m_attributes) {
        
        byte_size = e.second->getElementSize()*e.second->getElementCount();
        glVertexAttribPointer(e.first, e.second->getElementDimension(), e.second->getElementType(), GL_FALSE, e.second->getElementSize(), (void*)total_bytes);
        glEnableVertexAttribArray(e.first);
        
        glBufferSubData(GL_ARRAY_BUFFER,
                        total_bytes,
                        byte_size,
                        e.second->getData());
        
        total_bytes += byte_size;
        
    }
    
    
    

}
void GeometryArray::loadIndicesToObject(GLuint vao, GLuint ibo)
{
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  ibo);
    
    size_t byte_size = 0;
    size_t total_bytes = 0;
    size_t total_elements = 0;
    for(auto const &e : m_indices) {
        byte_size = e.second->getElementSize()*e.second->getElementCount();
        total_bytes += byte_size;
    }
    


    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 total_bytes,
                 NULL, GL_STATIC_DRAW);

    byte_size = 0;
    total_bytes = 0;
    for(auto const &e : m_indices) {
        byte_size = e.second->getElementSize()*e.second->getElementCount();
        
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
                        total_bytes,
                        byte_size,
                        e.second->getData());
        
        m_indexBounds[e.first] = std::make_pair(total_elements, e.second->getElementCount());
        total_bytes += byte_size;
        total_elements += e.second->getElementCount();
    }
}

void GeometryArray::addIndex(std::vector<GLuint> & array, std::string indexName)
{
    GLuint* dataCopy = new GLuint[array.size()];
    std::copy ( array.data(), array.data()+array.size(), dataCopy );
    m_indices[indexName] = new AttributeArray((void*)(dataCopy), (size_t)sizeof(size_t), (size_t)array.size(), 1, GL_UNSIGNED_INT);
    
}

size_t GeometryArray::getStartForIndex(std::string indexName)
{
    return m_indexBounds[indexName].first;
}
size_t GeometryArray::getCountForIndex(std::string indexName)
{
    return m_indexBounds[indexName].second;
}

void GeometryArray::loadAttributesToContext(GLuint context)
{
    loadAttributesToObject(m_VAO[context], m_VBO[context]);
}

void GeometryArray::loadIndicesToContext(GLuint context)
{
    loadIndicesToObject(m_VAO[context], m_IBO[context]);
}

void GeometryArray::generateObjectsForContext(GLuint context)
{
    m_VBO[context] = GLuint(0);
    m_VAO[context] = GLuint(0);
    m_IBO[context] = GLuint(0);
    
    glGenVertexArrays(1, &m_VAO[context]);
    glGenBuffers(1, &m_VBO[context]);
    glGenBuffers(1, &m_IBO[context]);

  }

void GeometryArray::bindToContext(GLuint context)
{
    glBindVertexArray(m_VAO[context]);

}
