#ifndef __Shader__
#define __Shader__

#include <GL/glew.h>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

/**
 * @brief Provides an easy way to load, enable and disable shaders
 */
class Shader
{
    private:
        GLenum shader_type;
        GLhandleARB mVertexHandle;
        GLhandleARB mFragmentHandle;
        GLhandleARB mProgramHandle;

        std::string loadFile(const std::string &str);
        GLhandleARB createSharedObjects(const std::string shader, GLenum shaderType);
        void linkShader(GLhandleARB);

        void init();

    public:
        Shader();
        Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
        virtual ~Shader();

        void loadVertexShader(const std::string &str);
        void loadFragmentShader(const std::string &fragmentPath);
        void enable();
        void disable();
};



#endif
