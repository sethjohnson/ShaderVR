#include "ShaderProgram.h"



void ShaderProgram::addShader(Shader * shader)
{
    m_shaders.push_back(shader);
}

GLuint ShaderProgram::getObjectForContext(GLuint context)
{
    return m_objects[context];
}

void ShaderProgram::generateObjectsForContext(GLuint context)
{
    m_objects[context] = GLuint(0);
}

void ShaderProgram::linkToContext(GLuint context)
{
    m_objects[context] = glCreateProgram();
    
    
    for (std::list<Shader*>::iterator shader_i=m_shaders.begin(); shader_i != m_shaders.end(); ++shader_i)
        glAttachShader(getObjectForContext(context), (*shader_i)->getObjectForContext(context));

    glLinkProgram(m_objects[context] );
}
void ShaderProgram::useForContext(GLuint context)
{
    glUseProgram(m_objects[context] );
}