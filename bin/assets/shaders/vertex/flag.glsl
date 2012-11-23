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
        float A = maxAmplitude; //* r;
        return A * sin(2.*3.14159*(v*t-r/waveLength));
    }
    return 0.;
}


// XXX: Only applies to x and z wind direction
vec4 applyWave(vec4 vertexPosition)
{
    // Calculate wavelength so that it is located on vertical vertices of the flag
    float waveLength = 2.*width/nbSquares;
    float waveDisplacement = progressive_sinusoidal_wave_value(origin.xz, vertexPosition, 10., 5., waveTime, 8.);

    vec2 v = vertexPosition.xz - origin.xz;
    float norm = sqrt(pow(v.x,2) + pow(v.y,2));


    vec3 newPos = origin+norm*windDirection;
    newPos.y = vertexPosition.y;

    vec3 n = normalize(cross(windDirection, vec3(0,1,0)));
    newPos = newPos+waveDisplacement*n;
    return vec4(newPos, 1.);
}

void main() {

    gl_TexCoord[0] = gl_MultiTexCoord0;

    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * applyWave(gl_Vertex);

    gl_FrontColor = gl_Color;

}
