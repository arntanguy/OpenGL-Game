varying vec4 VertexPosition;

void main() {

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_TexCoord[1] = gl_MultiTexCoord1;
	gl_Position = ftransform();
    VertexPosition = gl_Vertex;
}
