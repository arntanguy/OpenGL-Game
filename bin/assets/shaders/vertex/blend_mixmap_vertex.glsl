varying vec4 VertexPosition;

uniform sampler2D Heightmap;
uniform float maxHeight;
uniform float terrainSize;
uniform float waterSinus;

// Returns the height of the terrain
vec4 get_height()
{
    vec4 vertexPosition = gl_Vertex;

    //vec4 heightTexel = texture2D(Heightmap, vec2(gl_Vertex.x/terrainSize, gl_Vertex.z/terrainSize)).rgba;
    //float height = maxHeight*heightTexel.r;
    vertexPosition.y = gl_Vertex.y*sin(10.*waterSinus);
    return vertexPosition;
}

void main() {

    gl_TexCoord[0] = gl_MultiTexCoord0;
    vec4 heightVertex = get_height();
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * heightVertex;

    // Pass vertex position to the fragment shader
    VertexPosition = heightVertex;


    /**
     *  Per-vertex lighting
     *  Ambiant, diffuse, specular
     *  Computed from LIGHT0
     */

	vec3 normal, lightDir, viewVector, halfVector;
	vec4 diffuse, ambient, globalAmbient, specular = vec4(0.0);
	float NdotL,NdotHV;

	/* first transform the normal into eye space and normalize the result */
	normal = normalize(gl_NormalMatrix * gl_Normal);

	/* now normalize the light's direction. Note that according to the
	OpenGL specification, the light is stored in eye space. Also since
	we're talking about a directional light, the position field is actually
	direction */
	lightDir = normalize(vec3(gl_LightSource[0].position));

	/* compute the cos of the angle between the normal and lights direction.
	The light is directional so the direction is constant for every vertex.
	Since these two are normalized the cosine is the dot product. We also
	need to clamp the result to the [0,1] range. */

	NdotL = max(dot(normal, lightDir), 0.0);

	/* Compute the diffuse, ambient and globalAmbient terms */
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	globalAmbient = gl_LightModel.ambient * gl_FrontMaterial.ambient;

	/* compute the specular term if NdotL is  larger than zero */
	if (NdotL > 0.0) {

		NdotHV = max(dot(normal, normalize(gl_LightSource[0].halfVector.xyz)),0.0);
		specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
	}

	gl_FrontColor = globalAmbient + NdotL * diffuse + ambient + specular;

}
