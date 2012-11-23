#ifndef __Shader__
#define __Shader__

#include <GL/glew.h>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include "Texture.h"
#include <SFML/Graphics.hpp>
#include "Debug.h"

/**
 * @brief Provides an easy way to load, enable and disable shaders
 * Uses the glew library, don't forget to initialize it!
 */
class Shader
{
    private:
        short mTextureUnit;
        GLenum shader_type;
        GLhandleARB mVertexHandle;
        GLhandleARB mFragmentHandle;
        GLhandleARB mProgramHandle;

        std::string loadFile(const std::string &str);
        GLhandleARB createSharedObjects(const std::string shader, GLenum shaderType);
        void linkShader(GLhandleARB);

        void init();

        int getVariableId(std::string strVariable);

    public:
        Shader();
        Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath);
        virtual ~Shader();

        void loadVertexShader(const std::string &str);
        void loadFragmentShader(const std::string &fragmentPath);
        void enable();
        void disable();

        void bindTexture(Texture& texture, const std::string &uniformLocation);

        void setFloat(const std::string &uniformVarName, float value);
        void setVec2(const std::string &uniformVarName, const sf::Vector2f& vector);
        void setVec3(const std::string &uniformVarName, const sf::Vector3f& vector);

        bool hasShaderSupport();

        GLhandleARB getProgramHandle() const;
};



#endif
