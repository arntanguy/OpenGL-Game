/**
 * Type: Fragment shader
 * Author: Arnaud TANGUY
 * Brief: Fragment shader blending textures from the four channels of a mixmap
 * Description:
 * The four channels of a mixmap will be used to blend the textures together:
 * r channel is used as alpha value for Texture0
 * g channel is used as alpha value for Texture1
 * b channel is used as alpha value for Texture2
 * a channel is used as alpha value for Texture3
 *
 * Example Vertex Shader to use: blend_textures_from_mixmap
 */

uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;
uniform sampler2D Mixmap;


uniform float terrainSize;
uniform float waterSinus;
uniform float waterHeight;

uniform float fogFactor;

varying vec4 VertexPosition;

vec4 mixmapTexturing()
{
   vec4 texel0 = texture2D(Texture0, gl_TexCoord[0].st*sin(waterSinus)).rgba;
   vec4 texel1 = texture2D(Texture1, gl_TexCoord[0].st).rgba;
   vec4 texel2 = texture2D(Texture2, gl_TexCoord[0].st).rgba;
   vec4 texel3 = texture2D(Texture3, gl_TexCoord[0].st).rgba;
   // Get texel on mixmap at the right UV coordinates
   // Shift half width in x and z axis because the terrain is built with the z axis in the middle
   vec4 mixmapTexel = texture2D(Mixmap, vec2(VertexPosition.x/terrainSize, VertexPosition.z/terrainSize)).rgba;

   // Water
   if(VertexPosition.y < waterHeight)
       texel0 *= mixmapTexel.r;
    else
       texel0 = texel1;
   // Sand
   texel1 = mix(texel0,  texel1, mixmapTexel.g);
   // Grass
   texel2 = mix(texel1, texel2, mixmapTexel.b);
   // rock
   return mix(texel2, texel3, mixmapTexel.a);
}

/**
 * Do the same kind of texturing as done with the mixmap,
 * But apply textture according to vertex height not mixmap color.
 * The texture will effectivly stay the same at each height
 * XXX: Not used
 */
vec4 fragmentTexturing()
{
   vec4 texel0 = texture2D(Texture0, gl_TexCoord[0].st*sin(waterSinus)).rgba;
   vec4 texel1 = texture2D(Texture1, gl_TexCoord[0].st).rgba;
   vec4 texel2 = texture2D(Texture2, gl_TexCoord[0].st).rgba;
   vec4 texel3 = texture2D(Texture3, gl_TexCoord[0].st).rgba;

   vec3 color;
   float water, sand, grass, rock;
   if(VertexPosition.y < 5.) {
      water=1.;
      sand=0.;
      grass=0.;
      rock=0.;
   } else if(VertexPosition.y < 10.) {
      water=0.5;
      sand=0.5;
      grass=0.;
      rock=0.;
   } else if(VertexPosition.y < 20.) {
      water=0.0;
      sand=0.5;
      grass=0.5;
      rock=0.;
   } else if(VertexPosition.y < 30.) {
      water=0.0;
      sand=0.0;
      grass=0.5;
      rock=0.196;
   } else {
      water=0.0;
      sand=0.0;
      grass=0.0;
      rock=1.;
   }
   // Water
   texel0 *= water;
   // Sand
   texel1 = mix(texel0,  texel1, sand);
   // Grass
   texel2 = mix(texel1, texel2, grass);
   // rock
   return mix(texel2, texel3, rock);
}

vec4 applyLights(vec4 tx)
{

    vec4 color = gl_Color*2.0;
    color *= tx;
	return clamp(color,0.0,1.0);
}

void main()
{
   vec4 texturedTexel = mixmapTexturing();
//   vec4 texturedTexel = fragmentTexturing();
   vec4 tx = applyLights(texturedTexel);

   gl_FragColor = tx;
}


