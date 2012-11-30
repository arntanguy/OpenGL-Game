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
 * * Example Vertex Shader to use: blend_textures_from_mixmap
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

varying vec3 lightDir, eyeVec;

//varying vec4 diffuse,ambientGlobal, ambient, ecPos;
varying vec4 ecPos;
varying vec3 normal;

const float cos_outer_cone_angle = 0.8; // 36 degrees

const float PI = 3.14159265358979323846264;
const int nbLights=2;

/**
 * Phong lighting
 */
vec4 applyLighting(int lightId)
{
    /* Normalize the halfVector to pass it to the fragment shader */
    vec3 halfVector = gl_LightSource[lightId].halfVector.xyz;
    /* Compute the diffuse, ambient and globalAmbient terms */
    vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightId].diffuse;
    vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[lightId].ambient;
    vec4 ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	vec4 final_color =
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
	(gl_LightSource[lightId].ambient * gl_FrontMaterial.ambient);

	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);

	///* compute the cos of the angle between the normal and lights direction.
	//The light is directional so the direction is constant for every vertex.
	//Since these two are normalized the cosine is the dot product. We also
	//need to clamp the result to the [0,1] range. */
	//NdotL = max(dot(normal, lightDir), 0.0);
	float lambertTerm = dot(N,L);

	if(lambertTerm > 0.0)
	{
	    /* Compute the diffuse term */
		final_color += gl_LightSource[lightId].diffuse *
		               gl_FrontMaterial.diffuse *
					   lambertTerm;

		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0),
		                 gl_FrontMaterial.shininess );
		final_color += gl_LightSource[lightId].specular *
		               gl_FrontMaterial.specular *
					   specular;
	}

    return final_color;
}

vec4 applyPointLighting(int lightId)
{
    /* Normalize the halfVector to pass it to the fragment shader */
    vec3 halfVector = gl_LightSource[lightId].halfVector.xyz;
    /* Compute the diffuse, ambient and globalAmbient terms */
    vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightId].diffuse;
    vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[lightId].ambient;
    vec4 ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;

    vec3 n,halfV,viewV,lightDir;
    float NdotL,NdotHV;
    vec4 color = ambientGlobal;
    float att;

    /* a fragment shader can't write a verying variable, hence we need
    a new variable to store the normalized interpolated normal */
    n = normalize(normal);

    // Compute the ligt direction
    vec3 lightDir2 = vec3(gl_LightSource[lightId].position-ecPos);

    /* compute the distance to the light source to a varying variable*/
    float dist = length(lightDir2);


    /* compute the dot product between normal and ldir */
    NdotL = max(dot(n,normalize(lightDir2)),0.0);

    if (NdotL > 0.0) {

        att = 1.0 / (gl_LightSource[lightId].constantAttenuation +
                gl_LightSource[lightId].linearAttenuation * dist +
                gl_LightSource[lightId].quadraticAttenuation * dist * dist);
        color += att * (diffuse * NdotL + ambient);


        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        color += att * gl_FrontMaterial.specular * gl_LightSource[lightId].specular * pow(NdotHV,gl_FrontMaterial.shininess);
    }
    return color;
}

vec4 applySpotlightLighting(int lightId)
{
    /* Normalize the halfVector to pass it to the fragment shader */
    vec3 halfVector = gl_LightSource[lightId].halfVector.xyz;

    /* Compute the diffuse, ambient and globalAmbient terms */
    vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[lightId].diffuse;
    vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[lightId].ambient;
    vec4 ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;

    vec3 n,halfV;
    float NdotL,NdotHV;
    vec4 color = ambientGlobal;
    float att,spotEffect;

    /* a fragment shader can't write a verying variable, hence we need
    a new variable to store the normalized interpolated normal */
    n = normalize(normal);

    // Compute the ligt direction
    vec3 lightDir2 = vec3(gl_LightSource[lightId].position-ecPos);

    /* compute the distance to the light source to a varying variable*/
    float dist = length(lightDir2);

    /* compute the dot product between normal and ldir */
    NdotL = max(dot(n,normalize(lightDir2)),0.0);
    if (NdotL > 0.0) {
        spotEffect = dot(normalize(gl_LightSource[lightId].spotDirection), normalize(-lightDir2));
        //if(cos(gl_LightSource[lightId].spotCutoff/(180./3.14159))<0.99)
        //    return vec4(1.,0.,0.,1.);
        if (spotEffect > cos(gl_LightSource[lightId].spotCutoff/(180./PI))) {
            spotEffect = pow(spotEffect, gl_LightSource[lightId].spotExponent);
            att = spotEffect / (gl_LightSource[lightId].constantAttenuation +
                    gl_LightSource[lightId].linearAttenuation * dist +
                    gl_LightSource[lightId].quadraticAttenuation * dist * dist);

            color += att * (diffuse * NdotL + ambient);


            halfV = normalize(halfVector);
            NdotHV = max(dot(n,halfV),0.0);
            color += att * gl_FrontMaterial.specular * gl_LightSource[lightId].specular * pow(NdotHV,gl_FrontMaterial.shininess);
        }
    }
        return color;
}

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
   //return mix(mixmapTexel, vec4(1,1,1,1), 0.5);
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

vec4 mixLightingAndTexture(vec4 textureColor, vec4 lightingColor)
{
    // Merging with the texture
    vec4 color = textureColor*2.0;
    color *= lightingColor;
	return clamp(color,0.0,1.0);
}

void main()
{
   vec4 texturedTexel = mixmapTexturing();
   vec4 lighting;
   for (int i=0; i<nbLights; i++) {
       lighting += applySpotlightLighting(i);
   }
   vec4 tx = mixLightingAndTexture(texturedTexel, lighting);
   gl_FragColor = tx;
}


