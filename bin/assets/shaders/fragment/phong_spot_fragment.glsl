// http://www.ozone3d.net/tutorials/glsl_lighting_phong_p3.php#part_3
/**
 * Spot lighting phong model
 */

varying vec3 normal, lightDir, eyeVec;

void main (void)
{
	vec4 final_color =
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

	vec3 L = normalize(lightDir);
	vec3 D = normalize(gl_LightSource[0].spotDirection);

	if (dot(-L, D) > gl_LightSource[0].spotCosCutoff)
	{
		vec3 N = normalize(normal);

		float lambertTerm = max( dot(N,L), 0.0);
		if(lambertTerm > 0.0)
		{
			final_color += gl_LightSource[0].diffuse *
			gl_FrontMaterial.diffuse *
			lambertTerm;

			vec3 E = normalize(eyeVec);
			vec3 R = reflect(-L, N);

			float specular = pow( max(dot(R, E), 0.0),
			gl_FrontMaterial.shininess );

			final_color += gl_LightSource[0].specular *
			gl_FrontMaterial.specular *
			specular;
		}
	}

	gl_FragColor = final_color;
}
