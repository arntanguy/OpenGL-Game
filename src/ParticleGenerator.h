#ifndef __particules__
#define __particules__

#include "GL/glew.h"
#include <iostream>
#include <cstdlib>
#include "GL/gl.h"
#include "SFML/Graphics.hpp"
#include "Shader.h"

#define MAX_PARTICLES 1000
#define MAX_MODIFIERS 10

/**
 * Struct particle: represents a particle
 */
typedef struct
{   bool active; // Active
    double life; // Lifespan
    double fade; // Speed of fading
    double r, g, b; // RGB values for colour
    double x, y, z; // Location
    double xi, yi, zi; // Motion Vector
    double size; // Size of the particle, usually very small (0.1)
} Particle;


class ParticleGenerator
{
    private:
        Particle *mParticles;
        int mMaxParticles;
        sf::Vector3f mCameraLocation;

        std::vector<sf::Vector3f> mForces;

        Shader mShader;
        std::vector<float> mModifiersDistance;
        std::vector<sf::Vector3f> mColorModifiers;

        void initParticles();

    public:
        ParticleGenerator(int maxParticles, std::string vertexShader, std::string fragmentShader);
        ~ParticleGenerator();
        void render();
        void changeSize(double size);

        void addForce(sf::Vector3f f);
        bool addColorModifier(float distance, const sf::Vector3f color);
};


double myRand(double min, double max);


#endif
