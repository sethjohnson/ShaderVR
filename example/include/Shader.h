#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include "GL/glew.h"
#include <string>

class Shader {
    std::string m_content;
    static std::string readFile(std::string fileName);
    static GLuint compileShader(std::string shaderText, GLenum shaderType);
    GLenum m_shaderType;
    std::map<GLuint, GLuint> m_objects;
public:
    Shader(std::string path, GLenum shaderType);
    const std::string & getContents();
    void compileToContext(GLuint context);
    GLuint getObjectForContext(GLuint context);
    void generateObjectsForContext(GLuint context);
};

#endif