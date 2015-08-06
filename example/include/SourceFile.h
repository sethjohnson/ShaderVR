#include <string>

class SourceFile
{
public:
    void load();
    void discard();
    const std::string & getContents();
    
    static SourceFile * LoadSourceFile(std::string path);
private:
    std::string m_contents;
    std::string m_filePath;
};