#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>

using std::ifstream;
using std::ios;

Shader::Shader(std::string path, GLenum shaderType)
{
    m_content = readFile(path);
    m_shaderType = shaderType;

}

std::string Shader::readFile(std::string fileName)
{
    std::stringstream ss;
    ifstream inFile(fileName, ios::in);
    std::string contents = "";
    
    if (!inFile)
    {
        std::cerr << "Failed to load " << fileName << std::endl;
    }
    else
    {
        ss << inFile.rdbuf();
        inFile.close();
        contents = ss.str();
    }
    return contents;
}

GLuint Shader::compileShader(std::string shaderText, GLenum shaderType)
{
    GLuint shaderObject = 0;
    shaderObject = glCreateShader(shaderType);
    const char *text = shaderText.c_str();
    
    glShaderSource(shaderObject, 1, &text, NULL);  //Read source
    glCompileShader(shaderObject); // Compile shaders
    
    GLint  compiled;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        printf("Vertex shader failed to compile:\n");
        if (1) {
            GLint logMaxSize, logLength;
            glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &logMaxSize);
            printf("printing error message of %d bytes\n", logMaxSize);
            char* logMsg = new char[logMaxSize];
            glGetShaderInfoLog(shaderObject, logMaxSize, &logLength, logMsg);
            printf("%d bytes retrieved\n", logLength);
            printf("=========\n%s\n=========\n", shaderText.c_str());
            printf("error message: %s\n", logMsg);
            delete[] logMsg;
        }
        exit(1);
    }
    
    return shaderObject;
}

const std::string & Shader::getContents()
{
    return m_content;
}

void Shader::compileToContext(GLuint context)
{
    m_objects[context] = compileShader(getContents(), m_shaderType);
    
}


GLuint Shader::getObjectForContext(GLuint context)
{
    return m_objects[context];
}
void Shader::generateObjectsForContext(GLuint context)
{
    m_objects[context] = GLuint(0);
}
