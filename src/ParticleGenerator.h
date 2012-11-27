#ifndef __particules__
#define __particules__

#include <iostream>
#include <cstdlib>
#include "GL/gl.h"
#include "SFML/Graphics.hpp"
#define MAX_PARTICLES 1000


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
    double xg, yg, zg; // Gravity
    double size; // Size of the particle, usually very small (0.1)
} Particle;

class ParticleGenerator
{
    private:
        Particle *mParticles;
        int mMaxParticles;
        sf::Vector3f mCameraLocation;

        void initParticles();

    public:
        ParticleGenerator(int maxParticles);
        ~ParticleGenerator();
        void render();
        void changeSize(double size);
        void setGravity(float x, float y, float z);
};


double myRand(double min, double max);


#endif
