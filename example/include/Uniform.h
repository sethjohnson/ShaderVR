#include <string>
#include "ShaderProgram.h"
#include <list>

#include <GL/glew.h>
#include "RawData.h"

struct UniformToShaderTether
{
    UniformToShaderTether();
    ShaderProgram *m_shaderProgram;
    std::string m_uniformName;
    GLuint m_location;
    GLuint getLocationForContext(GLuint context);
    
};
class Uniform  {
protected:

    bool m_transpose;
    
    std::list<UniformToShaderTether> m_tethers;
    
    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);
    RawData * m_rawData;
public:
    Uniform();
    Uniform(const void* value, size_t count, size_t byteCount, size_t dimension = 1, bool transpose = false);

    virtual void setTranspose(bool transpose);
    
    void tetherToShader(ShaderProgram * shaderProgram, std::string uniformName);
    void uploadToContext(GLuint context);
    
    void * getDataForContext(GLuint context);
    size_t getElementCountForContext(GLuint context);
    void setDataForContext(const void * data, GLuint context);
    virtual void setData(const void * data);
    
};

class Uniform1fv : public Uniform {
public:
    Uniform1fv(const void* value, size_t count);
    Uniform1fv(float value);

    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);

};

class Uniform2fv : public Uniform {
public:
    Uniform2fv(const void* value, size_t count);
    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);
    
};

class Uniform3fv : public Uniform {
public:
    Uniform3fv(const void* value, size_t count);
    virtual void uploadToContextShader(UniformToShaderTether &ashaderTether, GLuint context);
    
};

class Uniform4fv : public Uniform {
public:

    Uniform4fv(const void* value, size_t count);
    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);
    
};


class Uniform1iv : public Uniform {
public:
    Uniform1iv(const void* value, size_t count);
    Uniform1iv(int value);

    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);
    
};

class Uniform2iv : public Uniform {
public:
    Uniform2iv(const void* value, size_t count);
    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);
    
};

class Uniform3iv : public Uniform {
public:
    Uniform3iv(const  void* value, size_t count);
    virtual void uploadToContextShader(UniformToShaderTether &ashaderTether, GLuint context);
    
};

class Uniform4iv : public Uniform {
public:
    
    Uniform4iv(const  void* value, size_t count);
    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);
    
};



class UniformMatrix2fv : public Uniform {
public:
    UniformMatrix2fv(const void* value, size_t count, bool transpose);
    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);
    
};

class UniformMatrix3fv : public Uniform {
public:
    UniformMatrix3fv(const void* value, size_t count, bool transpose);
    virtual void uploadToContextShader(UniformToShaderTether &shaderTether, GLuint context);
    
};

class UniformMatrix4fv : public Uniform {
public:
    UniformMatrix4fv(const  void* value, size_t count, bool transpose);
    virtual void uploadToContextShader(UniformToShaderTether &ashaderTether, GLuint context);
    
};


