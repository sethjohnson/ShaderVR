#include "Uniform.h"

UniformToShaderTether::UniformToShaderTether()
{
    m_location = -1;
}
GLuint UniformToShaderTether::getLocationForContext(GLuint context)
{
    if (m_location == -1) {
        m_location = glGetUniformLocation(m_shaderProgram->getObjectForContext(context), m_uniformName.c_str());
    }
    return m_location;
    
}


Uniform::Uniform(const void* value, size_t count, size_t byteCount, size_t dimension, bool transpose) 
{
    m_rawData = new RawData(value, byteCount, count, dimension, GL_BYTE);
    m_transpose = transpose;

}
void * Uniform::getDataForContext(GLuint context)
{
    return m_rawData->getData();
}

void Uniform::setDataForContext(const void * data, GLuint context)
{
    return m_rawData->setData(data);
}

void Uniform::setData(const void * data)
{
    return m_rawData->setData(data);
}
size_t Uniform::getElementCountForContext(GLuint context)
{
    return m_rawData->getElementCount();
}

void Uniform::setTranspose(bool transpose)
{
    m_transpose = transpose;
}

void Uniform::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    glUseProgram(shaderTether.m_shaderProgram->getObjectForContext(context));
}
void Uniform::tetherToShader(ShaderProgram * shaderProgram, std::string uniformName)
{
    UniformToShaderTether tether;
    tether.m_shaderProgram = shaderProgram;
    tether.m_uniformName = uniformName;
    m_tethers.push_back(tether);
}

void Uniform::uploadToContext(GLuint context)
{
    for (std::list<UniformToShaderTether>::iterator it=m_tethers.begin(); it != m_tethers.end(); ++it)
        uploadToContextShader(*it, context);
}








Uniform1fv::Uniform1fv(const void* value, size_t count) : Uniform(value, count, sizeof(GLfloat)*1, 1)
{
}
Uniform1fv::Uniform1fv(float value) : Uniform(&value, 1, sizeof(GLfloat)*1, 1)
{
    
}
void Uniform1fv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniform1fv(shaderTether.getLocationForContext(context), getElementCountForContext(context), (GLfloat*)getDataForContext(context));
}
Uniform2fv::Uniform2fv(const void* value, size_t count) : Uniform(value, count, sizeof(GLfloat)*2, 2)
{
}

void Uniform2fv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniform2fv(shaderTether.getLocationForContext(context), getElementCountForContext(context), (GLfloat*)getDataForContext(context));

}

Uniform3fv::Uniform3fv(const void* value, size_t count) : Uniform(value, count, sizeof(GLfloat)*3, 3)
{
}

void Uniform3fv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniform3fv(shaderTether.getLocationForContext(context), getElementCountForContext(context), (GLfloat*)getDataForContext(context));

}

Uniform4fv::Uniform4fv(const void* value, size_t count) : Uniform(value, count, sizeof(GLfloat)*4, 4)
{
}

void Uniform4fv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniform4fv(shaderTether.getLocationForContext(context), getElementCountForContext(context), (GLfloat*)getDataForContext(context));

}



Uniform1iv::Uniform1iv(const void* value, size_t count) : Uniform(value, count, sizeof(GLint)*1, 1)
{
}
Uniform1iv::Uniform1iv(int value) : Uniform(&value, 1, sizeof(GLint)*1, 1)
{
}

void Uniform1iv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniform1iv(shaderTether.getLocationForContext(context), getElementCountForContext(context), (GLint*)getDataForContext(context));
}
Uniform2iv::Uniform2iv(const void* value, size_t count) : Uniform(value, count, sizeof(GLint)*2, 2)
{
}

void Uniform2iv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniform2iv(shaderTether.getLocationForContext(context), getElementCountForContext(context), (GLint*)getDataForContext(context));
    
}

Uniform3iv::Uniform3iv(const void* value, size_t count) : Uniform(value, count, sizeof(GLint)*3, 3)
{
}

void Uniform3iv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniform3iv(shaderTether.getLocationForContext(context), getElementCountForContext(context), (GLint*)getDataForContext(context));
    
}

Uniform4iv::Uniform4iv(const void* value, size_t count) : Uniform(value, count, sizeof(GLint)*4, 4)
{
}

void Uniform4iv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniform4iv(shaderTether.getLocationForContext(context), getElementCountForContext(context), (GLint*)getDataForContext(context));
    
}



UniformMatrix2fv::UniformMatrix2fv(const void* value, size_t count, bool transpose) : Uniform(value, count, sizeof(GLfloat)*4, 4, transpose)
{
}

void UniformMatrix2fv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniformMatrix2fv(shaderTether.getLocationForContext(context), getElementCountForContext(context), m_transpose,  (GLfloat*)getDataForContext(context));
    
}

UniformMatrix3fv::UniformMatrix3fv(const void* value, size_t count, bool transpose) : Uniform(value, count, sizeof(GLint)*9, 9, transpose)
{
}

void UniformMatrix3fv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniformMatrix3fv(shaderTether.getLocationForContext(context), getElementCountForContext(context), m_transpose, (GLfloat*)getDataForContext(context));
    
}

UniformMatrix4fv::UniformMatrix4fv(const void* value, size_t count, bool transpose) : Uniform(value, count, sizeof(GLint)*16, 16, transpose)
{
}

void UniformMatrix4fv::uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context)
{
    Uniform::uploadToContextShader(shaderTether, context);
    glUniformMatrix4fv(shaderTether.getLocationForContext(context), getElementCountForContext(context), m_transpose, (GLfloat*)getDataForContext(context));
    
}