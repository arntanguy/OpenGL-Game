#include "ParticleGenerator.h"

double myRand(double min, double max)
{
    return (double) (min + ((float) rand() / RAND_MAX * (max - min + 1.0)));
}

ParticleGenerator::ParticleGenerator(int maxParticles) {
    mMaxParticles = maxParticles;
    mParticles = new Particle[maxParticles]; initParticles();
}
ParticleGenerator::~ParticleGenerator() {
    delete mParticles;
}

void ParticleGenerator::initParticles()
{
    for(int i=0; i<mMaxParticles; i++)   // Boucle sur toutes les particules
    {
        mParticles[i].active = true;       // On rend la particule active
        mParticles[i].life = 1.0;   // Maximum de vie

        mParticles[i].fade = myRand(0.001,0.005);   // Vitesse de disparition aléatoire
        mParticles[i].r=myRand(0.0,1.0);  // Quantité aléatoire de Rouge
        mParticles[i].g=myRand(0.0,1.0);  // Quantité aléatoire de Vert
        mParticles[i].b=myRand(0.0,1.0);  // Quantité aléatoire de Bleu

        mParticles[i].xi = myRand(-1.0,1.0);   // Vitesse aléatoire
        mParticles[i].yi = myRand(-1.0,1.0);
        mParticles[i].zi = myRand(1.0,2.0);

        mParticles[i].size = 0.1;
    }
}


// This routine returns up to 3 camera directions: which way is "right", "up" and which way is the camera pointing ("look")
// in OpenGL coordinates.  In other words, this is which way the user's SCREEN is pointing in OpenGL "local" coordinates.
// (If the user is facing true north at the origin and is not rolled, these functiosn would be trivially easy because
// right would be 1,0,0, up would be 0,1,0 and look would be 0,0,1.  (NOTE: the look vector points TO the user, not
// FROM the user.)
//
// To draw a billboard centered at C, you would use these coordinates:
//
// c-rgt+up---c+rgt+up
// |                 |
// |        C        |
// c-rgt-up---c+rgt-up
//
static void camera_directions(
						float * out_rgt,		// Any can be NULL
						float * out_up ,
						float * out_look)
{
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	// Roughly speaking, a modelview matrix is made up more or less like this:
	// [ EyeX_x EyeX_y EyeX_z    a
	//   EyeY_x EyeY_y EyeY_z    b
	//   EyeZ_x EyeZ_y EyeZ_z    c
	//   um don't look down here ]
	// where a, b, c are translations in _eye_ space.  (For this reason, a,b,c is not
	// the camera location - sorry!)

	if(out_rgt) {
		out_rgt[0] = m[0];
		out_rgt[1] = m[4];
		out_rgt[2] = m[8];
	}
	if(out_up) {
		out_up[0] = m[1];
		out_up[1] = m[5];
		out_up[2] = m[9];
	}
	if(out_look) {
		out_up[0] = m[2];
		out_up[1] = m[6];
		out_up[2] = m[10];
	}
}

void ParticleGenerator::render()
{
    // Right and up direction of camera
    float r[3], u[3];
	camera_directions(r,u,NULL);
    glPushMatrix();

        int a = 0;
        for(int i=0; i<mMaxParticles; i++) // For each particle
        {
            if(mParticles[i].active)         // If it is active
            {
                float x = mParticles[i].x;   // Get its position
                float y = mParticles[i].y;
                float z = mParticles[i].z;

                /* Particle color, transparence is life */
                glColor4d(mParticles[i].r, mParticles[i].g, mParticles[i].b, mParticles[i].life);

                glBegin(GL_TRIANGLE_STRIP);
                float size = mParticles[i].size;
                    glVertex3f(size*(x+r[0]+u[0]),size*(y+r[1]+u[1]),size*(z+r[2]+u[2]));
                    glVertex3f(size*(x-r[0]+u[0]),size*(y-r[1]+u[1]),size*(z-r[2]+u[2]));
                    glVertex3f(size*(x+r[0]-u[0]),size*(y+r[1]-u[1]),size*(z+r[2]-u[2]));
                    glVertex3f(size*(x-r[0]-u[0]),size*(y-r[1]-u[1]),size*(z-r[2]-u[2]));
                glEnd();

                /* Move particles */
                mParticles[i].x += mParticles[i].xi;
                mParticles[i].y += mParticles[i].yi;
                mParticles[i].z += mParticles[i].zi;

                /* Gravity */
                std::vector<sf::Vector3f>::const_iterator it;
                for(it = mForces.begin(); it != mForces.end(); it++) {
                    mParticles[i].x += (*it).x;
                    mParticles[i].y += (*it).y;
                    mParticles[i].z += (*it).z;
                }

                //std::cerr << "m "<< x << "\t" << y << "\t" << z << std::endl;


                mParticles[i].life -= mParticles[i].fade;

                /* If particle died, we regenerate it */
                if (mParticles[i].life < 0.0)
                {   mParticles[i].life = 1.0;    // Full life
                    mParticles[i].fade = myRand(0.01,0.05);

                    mParticles[i].x = 0.0; // Put particle back to the emitter
                    mParticles[i].y = 0.0;
                    mParticles[i].z = 0.0;

                    mParticles[i].xi = myRand(-10.0,10.0);   // Random speed
                    mParticles[i].yi = myRand(-10.0,10.0);
                    mParticles[i].zi = myRand(10.0,20.0);

                    mParticles[i].r=myRand(0.0,1.0);  // Quantité aléatoire de Rouge
                    mParticles[i].g=myRand(0.0,1.0);  // Quantité aléatoire de Vert
                    mParticles[i].b=myRand(0.0,1.0);  // Quantité aléatoire de Bleu
                }
            }
        }
        //std::cerr << std::endl;
    glPopMatrix();
}

void ParticleGenerator::changeSize(double size) {
    for(int i=0; i<mMaxParticles; i++) {
        mParticles[i].size = size;
    }
}

void ParticleGenerator::addForce(sf::Vector3f f)
{
    mForces.push_back(f);
}
