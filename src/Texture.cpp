#include "Texture.h"

Texture::Texture()
{
};

Texture::Texture(const std::string& path)
{
    loadTexture(path);
};

Texture::~Texture()
{
    glDeleteTextures(1, &mTextureId);
}

bool Texture::loadTexture(const std::string &path)
{
    try {
        RessourcesManager::getInstance().loadImage(path);
    } catch(std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }

    sf::Image* img = RessourcesManager::getInstance().getImage(path);
    loadTexture(img, path);
}

bool Texture::loadTexture(sf::Image* img, const std::string& name)
{

    if(img->getSize().x%2 != 0 || img->getSize().y % 2 != 0) {
        std::cerr << "Warning: image size is not a multiple of 2, texture " << name << " will not be loaded!";
        return false;
    }

	glGenTextures(1, &mTextureId); //we need to create an unique ID for the texture
    // Store texture in VRAM or RAM (opengl handles it behind the scene).
    // There is no need to keep track of the image after that, opengl handles the data itself.
    mImage = img;
}

/**
 * @brief Binds the texture.
 * It has to be done before glBegin!
 */
void Texture::bind()
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mImage->getSize().x, mImage->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, mImage->getPixelsPtr());
	glBindTexture(GL_TEXTURE_2D, mTextureId);	//we activate this id to tell opengl we are going to use this texture
    glActiveTexture(mTextureId);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

GLuint Texture::getTextureId() const
{
    return mTextureId;
}