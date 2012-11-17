#include "Textures.h"

Textures::Textures()
{
}

Textures::~Textures()
{
}

/**
 * @brief Add a texture to the list
 *
 * @param imgPath
 *        Path of the image to use. This image must have been loaded first through RessourcesManager
 *
 * @return
 *      true on success
 *      flase on failure (image not loaded)
 */
bool Textures::addTexture(const std::string& imgPath)
{
    sf::Image *img = RessourcesManager::getInstance().getImage(imgPath);
    if(img == NULL) return false;

    mTextures[imgPath] = img;
    return true;
}

//void Textures::bindToShader()
//{
//
//
//    shad = new Shader("vertex_single_texture.glsl", "fragment_two_texture.glsl");
//
//    try {
//        RessourcesManager::getInstance().loadImage("assets/crate.jpg");
//        RessourcesManager::getInstance().loadImage("assets/l3d.jpg");
//    } catch(std::runtime_error &e) {
//        std::cerr << "Error: " << e.what() << std::endl;
//        return false;
//    }
//
//    sf::Image *img = RessourcesManager::getInstance().getImage("assets/crate.jpg");
//    sf::Image *img2 = RessourcesManager::getInstance().getImage("assets/crate.jpg");
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->getSize().x, img->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->getPixelsPtr());
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img2->getSize().x, img2->getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, img->getPixelsPtr());
//
//	glGenTextures(2, mTextures);
//    int my_sampler_uniform_location = glGetUniformLocationARB(shad->getProgramHandle(), "tex");
//    glActiveTexture(GL_TEXTURE0 + 0);
//    glBindTexture(GL_TEXTURE_2D, mTextures[0]);
//    int my_sampler_uniform_location2 = glGetUniformLocationARB(shad->getProgramHandle(), "tex");
//    glActiveTexture(GL_TEXTURE0 + 1);
//    glBindTexture(GL_TEXTURE_2D, mTextures[1]);
//
//    glUniform1iARB(my_sampler_uniform_location, 0);
//    glUniform1iARB(my_sampler_uniform_location2, 1);
//}

