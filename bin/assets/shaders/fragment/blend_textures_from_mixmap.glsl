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

/**
 * The width and height of the mixmap image, used to generate the correct UV position
 */
uniform float mixmapWidth;
uniform float mixmapHeight;


uniform float fogFactor;

varying vec4 VertexPosition;

vec4 mixmapTexturing()
{
   vec4 texel0 = texture2D(Texture0, gl_TexCoord[0].st).rgba;
   vec4 texel1 = texture2D(Texture1, gl_TexCoord[0].st).rgba;
   vec4 texel2 = texture2D(Texture2, gl_TexCoord[0].st).rgba;
   vec4 texel3 = texture2D(Texture3, gl_TexCoord[0].st).rgba;
   // Get texel on mixmap at the right UV coordinates
   vec4 mixmapTexel = texture2D(Mixmap, vec2(VertexPosition.x/mixmapWidth, VertexPosition.z/mixmapHeight)).rgba;

   texel0 *= mixmapTexel.r;
   texel1 = mix(texel0,  texel1, mixmapTexel.g);
   texel2 = mix(texel1, texel2, mixmapTexel.b);
   return mix(texel2, texel3, mixmapTexel.a);
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
   vec4 tx = applyLights(texturedTexel);

   gl_FragColor = tx;
}


