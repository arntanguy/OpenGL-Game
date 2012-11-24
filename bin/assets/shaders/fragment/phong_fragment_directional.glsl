varying vec3 normal, lightDir, eyeVec;

void main (void)
{
	vec4 final_color =
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) +
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

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

	gl_FragColor = final_color;
}
