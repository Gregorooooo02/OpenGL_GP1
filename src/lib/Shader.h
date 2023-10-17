#ifndef OPENGLGP_SHADER_H
#define OPENGLGP_SHADER_H

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    // ID program object
    unsigned int ID;

    // Constructor which reads shaders from the disk and creates it
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    void use();
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
};


#endif //OPENGLGP_SHADER_H
