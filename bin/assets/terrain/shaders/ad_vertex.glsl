#version 330

in vec4 vertex;
in vec4 texture;
in vec3	normal;

uniform mat4 mvpMatrix;
uniform mat4 mvMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPosition;

smooth out vec3 varyingNormal;
smooth out vec3 varyingLightDir;
smooth out vec2 texCoords;

void main() {
	varyingNormal = normalMatrix * normal;
	
	vec4 position4 = mvMatrix * vertex;
	vec3 position3 = position4.xyz / position4.w;

	// light direction
	varyingLightDir = normalize(lightPosition - position3);

	texCoords = texture.st;

	// standard geometry vertexshader transform
	gl_Position = mvpMatrix * vertex;
}