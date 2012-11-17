/**
 * Type: Fragment shader
 * Author: Arnaud TANGUY
 * Brief: Fragment shader adding two textures together
 */

uniform sampler2D Texture0;
uniform sampler2D Texture1;

void main()
{
   // get texel value from texture (pixel)
   vec4 texel0 = texture2D(Texture0, gl_TexCoord[0].st);
   vec4 texel1 = texture2D(Texture1, gl_TexCoord[0].st);
   // value, minval, maxval
   gl_FragColor = clamp(texel0 + texel1, 0.0, 1.0);
}
