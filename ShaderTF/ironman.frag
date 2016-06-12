#version 330
uniform sampler2D myTexture;
uniform sampler2D specularTex;

varying vec3 normal,lightDir;

void main()
{

	float NdotL;
	float NdotHV;

	vec4 color = vec4(0.0);

	NdotL = max(dot(normal,lightDir),0.0);

	vec4 ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec4 specColor = gl_FrontMaterial.specular * gl_LightSource[0].specular;
	vec4 texColor = (texture2D(myTexture, gl_TexCoord[0].st));

	if (NdotL > 0.0) 
	{
	    NdotHV = max( dot (normal, gl_LightSource[0].halfVector.xyz), 0.0 );
  	    vec4 Specular = texture2D(specularTex, gl_TexCoord[0].st) * pow( NdotHV, gl_FrontMaterial.shininess);

	    color = texColor * (ambient + (diffuse * NdotL) + Specular * specColor * 15.0); 

	}
	gl_FragColor = color;
}
