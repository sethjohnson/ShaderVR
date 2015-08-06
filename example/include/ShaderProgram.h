#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <map>
#include <GL/glew.h>
#include "Shader.h"
#include <list>

class ShaderProgram
{
public:
    void generateObjectsForContext(GLuint context);
    GLuint getObjectForContext(GLuint context);
    void addShader(Shader * shader);
    void linkToContext(GLuint context);
    void useForContext(GLuint context);
protected:
    std::map<GLuint, GLuint> m_objects;
    std::list<Shader*> m_shaders;

};

#endif 