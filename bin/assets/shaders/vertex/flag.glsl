/******************************************************************************
 *     Copyright (C) 2011-2012  TANGUY Arnaud arn.tanguy@gmail.com             *
 *                                                                             *
 * This program is free software; you can redistribute it and/or modify        *
 * it under the terms of the GNU General Public License as published by        *
 * the Free Software Foundation; either version 2 of the License, or           *
 * (at your option) any later version.                                         *
 *                                                                             *
 * This program is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of              *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                *
 * GNU General Public License for more details.                                *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program; if not, write to the Free Software Foundation, Inc.,     *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.                 *
  ******************************************************************************/

/**
 * Flag shader
 * Description: Oscillating flag along wind direction
 */


uniform float waveTime;
uniform vec3 windDirection;
/**
 * Windstrength represent the wind strength between 0 and 1
 * When 0, the flag will drop
 * When 1, the flag will be fully up and flowing
 */
uniform float windStrength;
/**
 * Maximum amplitude of osciallation.
 * It will be modulated according to windStrength
 **/
uniform float maxAmplitude;
// Attach point of the flag
uniform vec3 origin;
// Used to calculate wavelength
uniform float nbSquares;
uniform float width;

/**
 * Sinusoidal wave propagating along X and Z direction in a circle
 * @param center
 *      Position of the center (x, z)
 * @param currentVertex
 *      Current vertex
 * @param rmax
 *      Maximum radius of the wave
 * @param maxAmplitude
 *      Maximum amplitude of the height displacement
 * @param v
 *      Velocity of the wave
 * @param t
 *      Current time
 * @param waveLength
 *      Wavelength of the wave
 **/
vec4 progessive_sinusoidal_wave(vec2 center, vec4 currentVertex, float rmax, float maxAmplitude, float v, float t, float waveLength)
{
    float r = sqrt(pow(currentVertex.x-center.x, 2.) + pow(currentVertex.z-center.y, 2.));
    if( r <= rmax ) {
        float d = rmax-r;
        float A = maxAmplitude * (rmax-r)/rmax;
        float newHeight = A * sin(2.*3.14159*(v*t-r/waveLength));
        currentVertex.y = newHeight;
    }
    return currentVertex;
}


float progressive_sinusoidal_wave_value(vec2 center, vec4 currentVertex, float maxAmplitude, float v, float t, float waveLength)
{
    float r = sqrt(pow(currentVertex.x-center.x, 2.) + pow(currentVertex.z-center.y, 2.));
    if(currentVertex.x != center.x) {
        return maxAmplitude * sin(2.*3.14159*(v*t-r/waveLength));
    }
    return 0.;
}


// XXX: not mathematically correct in y direction, but still quite ok
// XXX: Change way to calculate the normal to allow tilting
vec4 applyWave(vec4 vertexPosition)
{
    /**
     * Calculate the norm of vector from vertex to origin
     * and apply it to get the vector colinear to wind direction
     **/
    vec3 v = vertexPosition.xyz - origin.xyz;
    float norm = sqrt(pow(v.x,2) + pow(v.z,2));

    float weight = norm/width*(3.14-3.14*windStrength);
    // Rotates downwards or upwards
    mat3 rotationMatrix = mat3(
       cos(weight), -sin(weight), 0.,
       sin(weight), cos(weight) , 0., // column 2
       0., 0., 1.  // column 3
    );

    // Calculate wavelength so that it is located on vertical vertices of the flag
    float waveLength = 2.*width/nbSquares;
    float waveDisplacement = progressive_sinusoidal_wave_value(origin.xz, vertexPosition, windStrength*maxAmplitude, 5., waveTime, 8.);

    vec3 newWindDirection = normalize(windDirection);

    // Make the vertex collinear to wind direction
    vec3 newOrigin = origin;
    // Math cheat for the y value
    newOrigin.y = vertexPosition.y;
    // Computes new position according to wind direction
    vec3 newPos = newOrigin+norm*newWindDirection;
    // Account for wind strength to make the flag fall a bit
    newPos = rotationMatrix * newPos;

    // Normal to the flag
    vec3 n = normalize(cross(newWindDirection, vec3(0,1,0)));
    // Apply the wave along the normal direction
    newPos = newPos+waveDisplacement*n;
    return vec4(newPos, 1.);
}

void main() {

    gl_TexCoord[0] = gl_MultiTexCoord0;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * applyWave(gl_Vertex);

    gl_FrontColor = gl_Color;

}
