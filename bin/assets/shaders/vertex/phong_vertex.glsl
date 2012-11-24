varying vec3 normal, lightDir, eyeVec;

// Directional light
// Only works for directional light
// Per-fragment lighting
// http://www.lighthouse3d.com/tutorials/glsl-tutorial/directional-lights-i/<S-Del>
void main() {
	vec4 diffuse;
	float NdotL;

	/* first transform the normal into eye space and normalize the result */
	normal = normalize(gl_NormalMatrix * gl_Normal);

	/* now normalize the light's direction. Note that according to the
	OpenGL specification, the light is stored in eye space. Also since
	we're talking about a directional light, the position field is actually
	direction */
	lightDir = normalize(vec3(gl_LightSource[0].position));

	gl_Position = gl_ModelViewMatrix * gl_ProjectionMatrix * gl_Vertex; //ftransform();
}

