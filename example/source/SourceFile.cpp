#include "SourceFile.h"
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <sstream>

SourceFile *  SourceFile::LoadSourceFile(std::string path)
{
    SourceFile * sourceFile = nullptr;
    std::ifstream inFile(path, std::ios::in);
    std::stringstream ss;
    std::string contents = "";

    if (!inFile)
    {
        std::cerr << "Failed to load " << path << std::endl;
    }
    else
    {
        
        
        if (!inFile)
        {
        }
        else
        {
            ss << inFile.rdbuf();
            inFile.close();
            contents = ss.str();
        }
        std::cout << contents << std::endl;
        
        inFile.close();
        sourceFile = new SourceFile();
        sourceFile->m_filePath = path;
        sourceFile->m_contents = contents;
        inFile.close();
        std::cout << sourceFile->m_filePath <<std::endl;

        std::cout << sourceFile->m_contents <<std::endl;

    }

    return sourceFile;
}
const std::string & SourceFile::getContents()
{
    return m_contents;
}
