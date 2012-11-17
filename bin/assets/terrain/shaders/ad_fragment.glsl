#version 330

out vec4 fragColor;

uniform vec4 ambientColor;
uniform vec4 diffuseColor;
uniform sampler2D colorMap;
uniform float time;

smooth in vec3 varyingNormal;
smooth in vec3 varyingLightDir;
smooth in vec2 texCoords;

void main() {
	// dot products gives diffuse intensity
	float diff = max(0.0, dot(normalize(varyingNormal), normalize(varyingLightDir)));

	fragColor = ambientColor;

	fragColor = diff * diffuseColor;

	fragColor *= texture(colorMap, texCoords);
	
	fragColor = mix(fragColor, vec4(0.8,1.0,0.8,1.0), smoothstep(75,100,gl_FragCoord.z / gl_FragCoord.w));
}