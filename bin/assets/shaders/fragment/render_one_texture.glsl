uniform sampler2D tex;

void main()
{
	vec4 color = texture2D(tex,gl_TexCoord[0].st);
    //Change red channel to light red
    color.r = 0.1;
	gl_FragColor = color;
}
