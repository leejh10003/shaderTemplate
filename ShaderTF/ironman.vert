#version 330
varying vec3 normal,lightDir;

void main() 
{
	vec4 ecPos = gl_ModelViewMatrix * gl_Vertex;
	lightDir = normalize(gl_LightSource[0].position.xyz - ecPos.xyz);
	normal = normalize(gl_NormalMatrix * gl_Normal);
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();

}
