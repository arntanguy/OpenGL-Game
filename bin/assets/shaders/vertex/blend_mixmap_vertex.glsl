varying vec4 VertexPosition;

void main() {

	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
    VertexPosition = gl_ModelViewMatrix * gl_Vertex;;
}
