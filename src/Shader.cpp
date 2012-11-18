/**
 * Inspired by
 * http://content.gpwiki.org/index.php/OpenGL:Tutorials:Loading_and_using_GLSL_shaders
 **/

#include "Shader.h"

/**
 * @brief Creates a shader class
 * @throws runtime_error if the shaders aren't supported
 */
Shader::Shader()
{
    init();
}

Shader::Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
    init();
    loadVertexShader(vertexShaderPath);
    loadFragmentShader(fragmentShaderPath);
}

void Shader::init()
{
    if (hasShaderSupport())
    {
        //std::cout << "GL Shaders supported" << std::endl;
        //Create a mProgramHandle handle.
        mProgramHandle = glCreateProgramObjectARB();
        mVertexHandle = 0;
        mFragmentHandle = 0;
    } else {
        throw std::runtime_error("Error: GL Shaders not supported!");
    }
}

Shader::~Shader()
{
	glDeleteObjectARB(mVertexHandle);
	glDeleteObjectARB(mFragmentHandle);
	glDeleteObjectARB(mProgramHandle);

 // To free a shader we need to detach the vertex and fragment
    // shader pointers from the program object, then free each one.
    // Once that is done we can finally delete the program object.

    // If our vertex shader pointer is valid, free it
    if(mVertexHandle)
    {
        glDetachObjectARB(mProgramHandle, mVertexHandle);
        glDeleteObjectARB(mVertexHandle);
        mVertexHandle = 0;
    }

    // If our fragment shader pointer is valid, free it
    if(mFragmentHandle)
    {
        glDetachObjectARB(mProgramHandle, mFragmentHandle);
        glDeleteObjectARB(mFragmentHandle);
        mFragmentHandle = 0;
    }

    // If our program object pointer is valid, free it
    if(mProgramHandle)
    {
        glDeleteObjectARB(mProgramHandle);
        mProgramHandle = 0;
    }
}

std::string Shader::loadFile(const std::string &str)
{
    //First, let us load the vertex shader.
    std::fstream dataFile(str.c_str(), std::ios::in);
    std::string data;

    if (dataFile.is_open())
    {
        //This is to help store the file's buffer.
        std::stringstream buffer;
        //Here, get all of the file's data by streaming the file's stream into our buffer stream.
        buffer << dataFile.rdbuf();
        //Now, buffer contains the vertex shader's data
        data = buffer.str();
    }
    return data;
}

/**
 * @brief Loads a vertex shader
 *
 * @param vertexPath
 *      Path to the vertex shader
 *
 */
void Shader::loadVertexShader(const std::string &vertexPath)
{
    std::string vertexShader = loadFile(vertexPath);
    mVertexHandle = createSharedObjects(vertexShader, GL_VERTEX_SHADER);
    linkShader(mVertexHandle);
}

/**
 * @brief Loads a fragment shader
 *
 * @param fragmentPath
 *     Path to the fragment shader
 */
void Shader::loadFragmentShader(const std::string &fragmentPath)
{
    std::string fragmentShader = loadFile(fragmentPath);
    mFragmentHandle = createSharedObjects(fragmentShader, GL_FRAGMENT_SHADER);
    linkShader(mFragmentHandle);
}

/**
 * @brief Compiles the given shader
 *
 * @param shader
 *        A string containing the shader program
 * @param shaderType
 *        Type of shader (GL_FRAGMENT_SHADER, GL_VERTEX_SHADER...)
 *
 * @return
 *        The handle of the built shader
 */
GLhandleARB Shader::createSharedObjects(const std::string shader, GLenum shaderType)
{
    //Note: Creating a fragment shader is the same exact process, except that GL_FRAGMENT_SHADER is passed to
    //glCreateShaderObjectARB, and fragmentShader's code is passed to the source code call. For this reason,
    //the code to load a fragment shader is omitted, and left as an exercize for the reader.

    //Create a handle.
    GLhandleARB vertexHandle;

    //Obtain a valid handle to a vertex shader object.
    vertexHandle = glCreateShaderObjectARB(shaderType);
    if (!vertexHandle)
    {
        //We have failed creating the vertex shader object.
        std::cout<<"Failed creating vertex shader object";
    }

    //Now, compile the shader source.
    //Note that glShaderSource takes an array of chars. This is so that one can load multiple vertex shader files at once.
    //This is similar in function to linking multiple C++ files together. Note also that there can only be one "void main" definition
    //In all of the linked source files that are compiling with this funciton.
    const char *te = (shader.c_str());
    glShaderSourceARB(
            vertexHandle, //The handle to our shader
            1, //The number of files.
            &te, //An array of const char * data, which represents the source code of theshaders
            NULL); //An array of string lengths. For have null terminated strings, pass NULL.

    //Attempt to compile the shader.
    glCompileShaderARB(vertexHandle);

    //Error checking.
    int result;
    glGetObjectParameterivARB(vertexHandle, GL_OBJECT_COMPILE_STATUS_ARB, &result);
    if (!result)
    {
        //We failed to compile.
        std::cout<<"Vertex shader failed compilation." << shader <<"\n";
        //Attempt to get the length of our error log.
        int length;
        glGetObjectParameterivARB(vertexHandle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
        //Create a buffer.
        std::vector<char> buffer;
        buffer.resize(length);

        //Used to get the final length of the log.
        int final;
        glGetInfoLogARB(vertexHandle, length, &final, &buffer[0]);
        //Convert our buffer into a string.
        std::string message(&buffer[0], length);
        std::cout<<message;
        if (final > length)
        {
            //The buffer does not contain all the shader log information.
            std::cout<<"Shader Log contained more information!" << std::endl;
        }
    }
    return vertexHandle;
}

/**
 * @brief Links the shader object to the shader program
 * The shader has to be built first by calling Shader::createSharedObjects
 *
 * @param shaderHandle
 *    Handle of the shader to link
 */
void Shader::linkShader(GLhandleARB shaderHandle)
{
    //Attach the shaders. Here, assume that fragmentHandle is a handle to a fragment shader object,
    //and that vertexHandle is a handle to a vertex shader object.
    glAttachObjectARB(mProgramHandle, shaderHandle);

    //Link the mProgramHandle.
    glLinkProgramARB(mProgramHandle);

    //Now the program can be used.
}

/**
 * @brief Enables the shader. This will effectively replace the render pipeline functions by
 * the program shader
 */
void Shader::enable()
{
    glUseProgramObjectARB(mProgramHandle);
}

/**
 * @brief Restores the render pipeline
 */
void Shader::disable()
{
    glUseProgramObjectARB(0);
}

bool Shader::hasShaderSupport()
{
    return GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader;
}

/**
 * @brief Sets an uniform GLSL float value
 * WARNING: This has to be called after the shader has been enabled (by calling the enable() function)!
 *
 * @param uniformVarName
 *      The uniform variable name in the GLSL shader
 * @param value
 *      The value to set
 */
void Shader::setFloat(const std::string &uniformVarName, float value)
{
    int my_sampler_uniform_location = getVariableId(uniformVarName);
    glUniform1f(my_sampler_uniform_location, value);
}

void Shader::bindTexture(Texture& texture, const std::string &uniformLocation, int i)
{
    glActiveTexture(GL_TEXTURE0 + i);
    texture.bind();
    int my_sampler_uniform_location = getVariableId(uniformLocation);
    glUniform1i(my_sampler_uniform_location, i);
}

GLhandleARB Shader::getProgramHandle() const
{
    return mProgramHandle;
}



int Shader::getVariableId(std::string strVariable)
{
    // If we don't have an active program object, let's return -1
    if(!mProgramHandle)
        return -1;

    // This returns the variable ID for a variable that is used to find
    // the address of that variable in memory.
    return glGetUniformLocationARB(mProgramHandle, strVariable.c_str());
}
